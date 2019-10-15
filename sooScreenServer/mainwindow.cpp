#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QElapsedTimer>
#include <cstdint>
#include <cstring>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&m_tmr,SIGNAL(timeout()),this,SLOT(on_timerTimeout()));
    m_tmr.setSingleShot(true);

    m_work.init();

}


void MainWindow::on_timerTimeout()
{
    QElapsedTimer tmr;
    tmr.start();
    m_work.run();
    std::cout << tmr.elapsed() << std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{
    m_tmr.start(33);
}
