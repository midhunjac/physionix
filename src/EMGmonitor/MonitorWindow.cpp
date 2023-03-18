#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	mPlot(0),
	mTag1(0),
	mTag2(0)
{
	ui->setupUi(this);

	mPlot = new QCustomPlot(this);
	setCentralWidget(mPlot);

	
	mPlot->yAxis->setTickLabels(false);
	connect(mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)), mPlot->yAxis, SLOT(setRange(QCPRange))); 
	mPlot->yAxis2->setVisible(true);
	mPlot->axisRect()->addAxis(QCPAxis::atRight);
	mPlot->axisRect()->axis(QCPAxis::atRight, 0)->setPadding(30);
	mPlot->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(30); 

	
	mGraph1 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 0));
	mGraph2 = mPlot->addGraph(mPlot->xAxis, mPlot->axisRect()->axis(QCPAxis::atRight, 1));
	mGraph1->setPen(QPen(QColor(250, 120, 0)));
	mGraph2->setPen(QPen(QColor(0, 180, 60)));

	
	mTag1 = new AxisTag(mGraph1->valueAxis());
	mTag1->setPen(mGraph1->pen());
	mTag2 = new AxisTag(mGraph2->valueAxis());
	mTag2->setPen(mGraph2->pen());

	connect(&mDataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
	mDataTimer.start(40);
}