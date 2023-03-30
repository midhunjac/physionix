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
  connect(_but_export, &QPushButton::clicked,this, [this](){this->_but_export_clicked();});
  connect(_but_freeze, &QPushButton::clicked,this, [this](){this->_but_freeze_clicked();});

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
QWidget::setMouseTracking(true);

QWidget *_central_window = new  QWidget();
_central_window->setLayout(vertical_layout);


setCentralWidget(_central_window);


 _grap_plot->yAxis->setTickLabels(false);
 _grap_plot->setInteractions(QCP::iSelectPlottables |  QCP::iRangeZoom | QCP::iRangeDrag);
  connect(_grap_plot->yAxis2, SIGNAL(rangeChanged(QCPRange)), _grap_plot->yAxis, SLOT(setRange(QCPRange)));

  _grap_plot->yAxis->setVisible(true);
  _grap_plot->yAxis2->setVisible(true);
  _grap_plot->yAxis3->setVisible(true);
  _grap_plot->yAxis4->setVisible(true);
  _grap_plot->axisRect()->addAxis(QCPAxis::atRight);
  _grap_plot->axisRect()->addAxis(QCPAxis::atRight);
  _grap_plot->axisRect()->addAxis(QCPAxis::atRight);
  _grap_plot->axisRect()->addAxis(QCPAxis::atRight);
  _grap_plot->axisRect()->axis(QCPAxis::atRight, 0)->setPadding(30); // add some padding to have space for tags
  _grap_plot->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(30); // add some padding to have space for tags
  _grap_plot->axisRect()->axis(QCPAxis::atRight, 2)->setPadding(30); // add some padding to have space for tags
  _grap_plot->axisRect()->axis(QCPAxis::atRight, 3)->setPadding(30); // add some padding to have space for tags

  // create graphs:
  _graph1 = _grap_plot->addGraph(_grap_plot->xAxis, _grap_plot->axisRect()->axis(QCPAxis::atRight, 0));
  _graph2 = _grap_plot->addGraph(_grap_plot->xAxis, _grap_plot->axisRect()->axis(QCPAxis::atRight, 1));
  _graph3 = _grap_plot->addGraph(_grap_plot->xAxis, _grap_plot->axisRect()->axis(QCPAxis::atRight, 2));
  _graph4 = _grap_plot->addGraph(_grap_plot->xAxis, _grap_plot->axisRect()->axis(QCPAxis::atRight, 3));

  /* Se the the color of axis indicattors */
  _graph1->setPen(QPen(QColor(255,0,0)));
  _graph2->setPen(QPen(QColor(0, 255, 0)));
  _graph3->setPen(QPen(QColor(0,0,255)));
  _graph4->setPen(QPen(QColor(250,120,0)));


  // create tags with newly introduced AxisTag class (see axistag.h/.cpp):
  _axis1 = new AxisTag(_graph1->valueAxis());
  _axis1->setPen(_graph1->pen());
  _axis2 = new AxisTag(_graph2->valueAxis());
  _axis2->setPen(_graph2->pen());
  _axis3 = new AxisTag(_graph3->valueAxis());
  _axis3->setPen(_graph3->pen());
  _axis4 = new AxisTag(_graph4->valueAxis());
  _axis4->setPen(_graph4->pen());

  connect(&mDataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
  mDataTimer.start(40);
}


void MonitorWindow::_but_export_clicked()
{
     QString filename = QFileDialog::getSaveFileName(this,"Save","Physionix_log.csv","CSV files(.csv)",0,0);
     QFile data(filename);
     if(data.open(QFile::WriteOnly|QFile::Truncate))
     {
         QTextStream output(&data);
         for(int i=0;i<1000;i++)
         {
         output<<i;
         }
     }

}



void MonitorWindow::_but_freeze_clicked()
{
 mDataTimer.stop();
}
