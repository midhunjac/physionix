#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fftw3.h>
#include <cstdlib>
#include <stdlib.h>
#include <functional>
#include "emg_filter.h"

// #define DEBUG

void EMGFilter::set_filter_params(EMG_filter filter)
{
    this->sampleRate = filter.sampleRate;
    this->windowSize = filter.windowSize;
    this->filterOrder = filter.filterOrder;
    this->lowPassCutoff = filter.lowPassCutoff;
    this->highPassCutoff = filter.highPassCutoff;
    this->threshold = filter.threshold;
    this->aggregateDataLimit = filter.aggregateDataLimit;
    buffer = std::vector<double>(windowSize, 0);
    running = false;
}

// Stop the EMG processing thread
void EMGFilter::stop() {
    running = false;
    if (emgThread->joinable()) {
        emgThread->join();
        delete emgThread;
        emgThread = nullptr;
    }
}

void EMGFilter::start()
{
    if(nullptr!=emgThread) return;
    emgThread = new std::thread(&EMGFilter::start_processing,this);
    
}
// Start the EMG processing thread
void EMGFilter::start_processing() {
    running = true;
    while(running){
            // add data to circular buffer
            buffer.insert(buffer.end(), emgData.begin(), emgData.end());
            buffer.erase(buffer.begin(), buffer.begin() + emgData.size());

            // Generate filter coefficients for a butterworth low-pass filter
            lowPassCoeffs = butterworthLowPassCoeffs(filterOrder, lowPassCutoff, sampleRate);
            highPassCoeffs = butterworthLowPassCoeffs(filterOrder, highPassCutoff, sampleRate);

            // Apply high-pass filter
            std::vector<double> highPassData = filterData(buffer, highPassCoeffs);
            // Apply low-pass filter
            std::vector<double> lowPassData = filterData(highPassData, lowPassCoeffs);

            // calculate FFT
            std::vector<double> fftData = calculateFFT(lowPassData);
            #ifdef DEBUG
            std::cout << "FFT data: ";
            for (int i = 0; i < fftData.size(); i++) {
                std::cout << fftData[i] << " ";
            }
            std::cout << std::endl;
            #endif

            // extract movement
            double movement = extractMovement(fftData, threshold);

            // sleep for a short time to avoid high CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
}

// Generate filter coefficients for a butterworth low-pass filter
std::vector<double> EMGFilter::butterworthLowPassCoeffs(int order, double cutoff, int sampleRate) {
    double theta_c = 2 * M_PI * cutoff / sampleRate;
    double alpha = sin(theta_c) / (2 * order);
    std::vector<double> a(order + 1, 0);
    std::vector<double> b(order + 1, 0);
    a[0] = 1 + 2 * alpha + 2 * alpha * alpha;
    a[1] = -2 * (1 + alpha) * cos(theta_c);
    a[2] = 1 - 2 * alpha + 2 * alpha * alpha;
    b[0] = alpha * alpha;
    b[1] = 2 * alpha * alpha;
    b[2] = alpha * alpha;
    std::vector<double> coeffs(order * 2 + 1, 0);

    // calculate filter coefficients using bilinear
    // transformation
    for (int i = 0; i < order - 1; i++) {
        coeffs[i] = b[i] / a[0];
        coeffs[i + order + 1] = -a[i] / a[0];
    }

    #ifdef DEBUG
    std::cout << "butterworthLowPassCoeffs: " << coeffs[0] << " " << coeffs[1] << " " << coeffs[2] << " " << coeffs[3] << " " << coeffs[4] << " " << coeffs[5] << " " << coeffs[6] << std::endl;
    #endif

    return coeffs;
}

std::vector<double> EMGFilter::butterworthHighPassCoeffs(int order, double cutoff, int sampleRate) {
    double theta_c = 2 * M_PI * sampleRate / cutoff;
    double alpha = sin(theta_c) / (2 * order);
    std::vector<double> a(order + 1, 0);
    std::vector<double> b(order + 1, 0);
    a[0] = 1 + 2 * alpha + 2 * alpha * alpha;
    a[1] = -2 * (1 + alpha) * cos(theta_c);
    a[2] = 1 - 2 * alpha + 2 * alpha * alpha;
    b[0] = alpha * alpha;
    b[1] = 2 * alpha * alpha;
    b[2] = alpha * alpha;
    std::vector<double> coeffs(order * 2 + 1, 0);

    // calculate filter coefficients using bilinear
    // transformation
    for (int i = 0; i < order - 1; i++) {
        coeffs[i] = b[i] / a[0];
        coeffs[i + order + 1] = a[i] / a[0];
    }

    #ifdef DEBUG
    std::cout << "butterworthHighPassCoeffs: " << coeffs[0] << " " << coeffs[1] << " " << coeffs[2] << " " << coeffs[3] << " " << coeffs[4] << " " << coeffs[5] << " " << coeffs[6] << std::endl;
    #endif

    return coeffs;
}


// FFT function
std::vector<double> EMGFilter::calculateFFT(const std::vector<double>&  data) {
    int n = data.size();

    // Allocate input and output arrays for FFTW
    fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);

    // Copy real-valued data into input array
    for (int i = 0; i < n; i++)
    {
        in[i][0] = data[i];
        in[i][1] = 0.0;
    }

    // Create FFTW plan and execute it
    fftw_plan plan = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // Copy complex-valued output into data array
    std::vector<double> fftData(n, 0);
    for (int i = 0; i < n; i++)
    {
        fftData[i] = out[i][0] * out[i][0] + out[i][1] * out[i][1];
    }

    // Free FFTW resources
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    return fftData;
}

// Filter data using IIR filter
std::vector<double> EMGFilter::filterData(const std::vector<double>& data, const std::vector<double>& coeffs) {
    std::vector<double> filteredData(data.size(), 0);
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < coeffs.size(); j++) {
            if (i - j >= 0) {
                filteredData[i] += data[i - j] * coeffs[j];
            }
        }
    }

    #ifdef DEBUG
    std::cout << "filterData: " << filteredData[0] << " " << filteredData[1] << " " << filteredData[2] << " " << filteredData[3] << " " << filteredData[4] << " " << filteredData[5] << " " << filteredData[6] << std::endl;
    #endif
    
    return filteredData;
}

// Extract movement from FFT data
double EMGFilter::extractMovement(const std::vector<double>& fftData, double threshold) {
    double movement = 0;
    for (int i = 0; i < fftData.size(); i++) {
        if (fftData[i] > threshold) {
            movement += fftData[i];
        }
    }
    movement /= fftData.size();
    movement -= 2*10e4;
    movement = abs(movement);
    std::cout<<"movement: "<< movement <<std::endl;

    newstate =  deducestate(movement);
    movementData.push_back(newstate);
    if (movementData.size() > aggregateDataLimit){
        newstate = aggregateMovement(movementData);
        movementData.clear();
    }

    if (currentstate != newstate)
        movementdetect(newstate);
    if (newstate == RELAXED)
        movementdetect(RELAXED);
    return movement;
}

STATES EMGFilter::aggregateMovement(std::vector<double>& movementData) {
    int n = movementData.size();
    int maxcount = 0;
    int element_having_max_freq;
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (movementData[i] == movementData[j])
                count++;
        }
        if (count > maxcount) {
            maxcount = count;
            element_having_max_freq = movementData[i];
        }
    }
    return deducestate(element_having_max_freq);
}

STATES EMGFilter::deducestate(double movement)
{
    if (movement < RELAXED_MAX && movement >= RELAXED_MIN)
        return RELAXED;
    else if (movement < FLEXED_MAX && movement >= FLEXED_MIN)
        return FLEXED;
    else if (movement < ROTATING_MAX && movement >= ROTATING_MIN)
        return ROTATING;
    else {
        return UNKNOWN;
    }
        
}

double EMGFilter::getMovement() {
    return movement;
}

