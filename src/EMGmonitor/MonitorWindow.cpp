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
	ui->setupUi(this);

	but_stop = new QPushButton("Stop", this);
	but_close = new QPushButton("Close", this);


	plot_widget = new QWidget();
	mPlot = new QCustomPlot(plot_widget);

	QHBoxLayout* plot_layout = new QHBoxLayout();

	plot_layout->addWidget(mPlot);
	plot_layout->addStretch(1);

	//plot_widget->setFixedSize(QSize(this->width(),this->height()));
	QHBoxLayout* grid_layout = new QHBoxLayout;
	grid_layout->addStretch(1);
	grid_layout->addWidget(but_close);
	grid_layout->addWidget(but_stop);
	grid_layout->addStretch(1);

	QVBoxLayout* la = new QVBoxLayout;
	la->addLayout(plot_layout);
	la->addLayout(grid_layout);


	QWidget* another = new  QWidget();
	another->setLayout(la);

	setCentralWidget(another);




	//setCentralWidget(mPlot);
	

	
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