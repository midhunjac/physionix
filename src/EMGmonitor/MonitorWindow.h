#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "axistag.h"

namespace Ui {
class MainWindow;
}

class MonitorWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MonitorWindow(QWidget *parent = 0);
  ~MonitorWindow();


private slots:
  //for future

private:
   QPushButton *_but_freeze, *_but_resume, *_but_export;
  QPointer<QCPGraph> _graph1, _graph2, _graph3, _graph4;
  QCPAxis *xAxis, *yAxis, *xAxis2, *yAxis2, *yAxis3, *yAxis4;
  AxisTag *_axis1, *_axis2, *_axis3, *_axis4;
  QWidget *plot_widget;
  QDockWidget *dock;
  Ui::MainWindow *ui;
  QCustomPlot *_grap_plot;
  QTimer mDataTimer;
};


#endif // MONITORWINDOW_H
