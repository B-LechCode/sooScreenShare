#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QByteArray>


#include <opencv2/opencv.hpp>

#include <QElapsedTimer>

#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <bitset>

#define runNew 1

#define HEADERSTRING_OFFSET 32
#define HEADER_SIZE 64

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_draw = new drawWidget();
    m_draw->show();
    m_work.init(m_draw);
}

MainWindow::~MainWindow()
{
    delete ui;
}
