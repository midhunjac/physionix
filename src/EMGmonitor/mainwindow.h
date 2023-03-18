#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MonitorWindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QToolButton>
#include <QMessageBox>
#include <QFont>


#include <iostream>

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        MonitorWindow *monitorWindow;
        QFont title_font,slogan_font;
        QLabel *title_text, *txt_viewdemo, *txt_monitor, *slogan_text;
        QPushButton *but_viewdemo, *but_monitor;


        void init_UI();


    private slots:
        void but_viewdemo_clicked();
        void but_monitor_clicked();





};

#endif // MAINWINDOW_H
