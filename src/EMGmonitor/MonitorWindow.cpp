#include "MonitorWindow.h"
#include "ui_mainwindow.h"


MonitorWindow::~MonitorWindow()
{
	delete ui;
}

MonitorWindow::MonitorWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  _grap_plot(0),
  _axis1(0),
  _axis2(0),
  _axis3(0),
  _axis4(0)
{
  _grap_plot = new QCustomPlot(this);
  ui->setupUi(this);

  /*Initiliaze buttons */
  _but_freeze = new QPushButton("Freeze", this);
  _but_resume = new QPushButton("Resume", this);
  _but_export = new QPushButton("Export data",this);


  QHBoxLayout *plot_layout = new QHBoxLayout();

  plot_layout->addWidget(_grap_plot);
  //plot_layout->addStretch(1);

  //plot_widget->setFixedSize(QSize(this->width(),this->height()));
  QHBoxLayout *grid_layout = new QHBoxLayout;
  grid_layout->addStretch(1);
  grid_layout->addWidget(_but_freeze);
  grid_layout->addWidget(_but_resume);
  grid_layout->addWidget(_but_export);
  grid_layout->addStretch(1);

QVBoxLayout *vertical_layout = new QVBoxLayout;
vertical_layout->addLayout(plot_layout);
vertical_layout->addLayout(grid_layout);


QWidget *_central_window = new  QWidget();
_central_window->setLayout(vertical_layout);


setCentralWidget(_central_window);


 _grap_plot->yAxis->setTickLabels(false);

  connect(_grap_plot->yAxis2, SIGNAL(rangeChanged(QCPRange)), _grap_plot->yAxis, SLOT(setRange(QCPRange)));
  _grap_plot->yAxis->setVisible(true);
  _grap_plot->yAxis2->setVisible(true);
  _grap_plot->yAxis3->setVisible(true);
  _grap_plot->yAxis4->setVisible(true);

  _grap_plot->axisRect()->addAxis(QCPAxis::atLeft);
  _grap_plot->axisRect()->addAxis(QCPAxis::atLeft);
  _grap_plot->axisRect()->addAxis(QCPAxis::atLeft);
  _grap_plot->axisRect()->addAxis(QCPAxis::atLeft);

  _grap_plot->axisRect()->axis(QCPAxis::atLeft, 0)->setPadding(30); 
  _grap_plot->axisRect()->axis(QCPAxis::atLeft, 1)->setPadding(30); 
  _grap_plot->axisRect()->axis(QCPAxis::atLeft, 2)->setPadding(30); 
  _grap_plot->axisRect()->axis(QCPAxis::atLeft, 3)->setPadding(30); 

  /*create graphs*/
  _graph1 = _grap_plot->addGraph(_grap_plot->xAxis, _grap_plot->axisRect()->axis(QCPAxis::atLeft, 0));
  _graph2 = _grap_plot->addGraph(_grap_plot->xAxis, _grap_plot->axisRect()->axis(QCPAxis::atLeft, 1));
  _graph3 = _grap_plot->addGraph(_grap_plot->xAxis, _grap_plot->axisRect()->axis(QCPAxis::atLeft, 2));
  _graph4 = _grap_plot->addGraph(_grap_plot->xAxis, _grap_plot->axisRect()->axis(QCPAxis::atLeft, 3));

  /* Se the the color of axis indicattors */
  _graph1->setPen(QPen(QColor(255,0,0))); //Red
  _graph2->setPen(QPen(QColor(0, 255, 0))); //green
  _graph3->setPen(QPen(QColor(0,0,255))); //blue
  _graph4->setPen(QPen(QColor(250,120,0))); //orange


  /*create the value tags.This is taken from the new adopted library.*/
  _axis1 = new AxisTag(_graph1->valueAxis());
  _axis1->setPen(_graph1->pen());
  _axis2 = new AxisTag(_graph2->valueAxis());
  _axis2->setPen(_graph2->pen());
  _axis3 = new AxisTag(_graph3->valueAxis());
  _axis3->setPen(_graph3->pen());
  _axis4 = new AxisTag(_graph4->valueAxis());
  _axis4->setPen(_graph4->pen());

  /* some random signal for debugging. might be useful later*/
  connect(&mDataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
  mDataTimer.start(40);
}

