#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QByteArray>


#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>

#include <QElapsedTimer>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
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

    m_work.init(ui->openGLWidget);    
}

MainWindow::~MainWindow()
{
    delete ui;
}
