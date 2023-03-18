#include "mainwindow.h"
#include <QPixmap>
#include "mainwindow.h"
#include <stdint.h>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent){

    init_UI();


}
MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::init_UI()
{

    QFont title_font("Arial", 20, QFont::Bold);
    QFont slogan_font("Arial",10,10,true);

    title_text = new QLabel("Physionix", this);
    title_text->setFont(title_font);
    slogan_text = new QLabel("Your physio",this);
    slogan_text->setFont(slogan_font);

    but_viewdemo = new QPushButton("Watch", this);
    but_monitor = new QPushButton("Start", this);

    txt_viewdemo = new QLabel("View tutorial video", this);
    txt_monitor = new QLabel("Start monitoring", this);



     QGridLayout *title_layout = new QGridLayout;
     title_layout->addWidget(title_text,0,0,Qt::AlignCenter);
     title_layout->addWidget(slogan_text,1,0,Qt::AlignCenter);


    QGridLayout *button_layout = new QGridLayout;
    button_layout->setHorizontalSpacing(10);
    button_layout->setVerticalSpacing(100);
    button_layout->addWidget(but_viewdemo, 0, 0, Qt::AlignLeft);
    button_layout->addWidget(txt_viewdemo, 0, 1);
    button_layout->addWidget(but_monitor, 1, 0, Qt::AlignLeft);
    button_layout->addWidget(txt_monitor, 1, 1);



    QHBoxLayout *grid_layout = new QHBoxLayout;
    grid_layout->addStretch(1);
    grid_layout->addLayout(button_layout);
    grid_layout->addStretch(1);





    QVBoxLayout *main_window_layout = new QVBoxLayout;
    main_window_layout->setSpacing(10);

    main_window_layout->addLayout(title_layout, Qt::AlignCenter);
    main_window_layout->addLayout(grid_layout, Qt::AlignCenter);



    QWidget *window = new QWidget();
    window->setLayout(main_window_layout);

    setCentralWidget(window);

    connect(but_monitor, &QPushButton::clicked,
            this, [this](){this->but_monitor_clicked();});
    connect(but_viewdemo, &QPushButton::clicked,
            this, [this](){this->but_viewdemo_clicked();});


}

void MainWindow::but_monitor_clicked()
{
     monitorWindow = new MonitorWindow();
     monitorWindow->show();

}
void MainWindow::but_viewdemo_clicked()
{

}
