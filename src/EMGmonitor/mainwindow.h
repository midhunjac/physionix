#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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

        QFont title_font,slogan_font;
        QLabel *title_text, *txt_viewdemo, *txt_monitor, *slogan_text;
        QPushButton *but_viewdemo, *but_monitor;


        void init_UI();


    private slots:
        // void timerEvent(); //depreciated: used to plot sinewave to curves





};

#endif // MAINWINDOW_H
