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

#define HEADERSTRING_OFFSET 32
#define HEADER_SIZE 64

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_clientAddr = QHostAddress(client);
    connect(&srvr,SIGNAL(newConnection()),this,SLOT(on_newConnection()));
    srvr.listen(QHostAddress::Any,port);
    connect(&m_tmr,SIGNAL(timeout()),this,SLOT(on_timerTimeout()));
    ui->lbl->setText("Listening!");

}

void MainWindow::on_newConnection()
{
    std::cout<< "new connection!" << std::endl;
    ui->lbl->setText("Connected!");
    QTcpSocket* ptrSock = srvr.nextPendingConnection();
    m_ptrSock = ptrSock;
    connect(m_ptrSock,SIGNAL(disconnected()),this,SLOT(on_socketDisconnected()));
    srvr.close();
    m_tmr.start(20);
}

void MainWindow::on_timerTimeout()
{
    static QByteArray arr;
    static size_t hdrStart = std::string::npos;
    static size_t dataLen = std::string::npos;
    if(!m_ptrSock)
        return;
    if(m_ptrSock->bytesAvailable())
    {
        QElapsedTimer tmr;
        tmr.start();
        auto l = m_ptrSock->readAll();

        arr.append(l);
        std::cout <<" copy data " << tmr.elapsed() << std::endl;
    }

    if(hdrStart == std::string::npos)
    {
        QElapsedTimer tmr;
        tmr.start();
        //Find the Header-String!
        std::string sdf = arr.toStdString();
        size_t headerStart = sdf.find("ScreenImage");

        if(headerStart != std::string::npos)
        {
            hdrStart = headerStart;
        }
        //std::cout <<" find header " << tmr.elapsed() << std::endl;
    }
	
    if(hdrStart != std::string::npos && dataLen == std::string::npos && arr.length() >=hdrStart+HEADER_SIZE)
	{
        QElapsedTimer tmr;
        tmr.start();
        char* ptrZahl = arr.data()+hdrStart+HEADERSTRING_OFFSET;
		int zahl;
		memcpy(&zahl,ptrZahl,sizeof(int));
		dataLen = zahl;
        std::cout <<" find size " << tmr.elapsed() << std::endl;

    }

    //Check if our length is long enough to have the whole img!
        if(hdrStart != std::string::npos && dataLen != std::string::npos && (hdrStart+dataLen) <= arr.length())
        {
            QElapsedTimer tmr;
            tmr.start();
            std::vector<uint8_t> dat(dataLen);
            memcpy(dat.data(),arr.data()+hdrStart+HEADER_SIZE,dataLen);
            std::cout <<" copy " << tmr.elapsed() << std::endl;
            cv::Mat img = cv::imdecode(cv::Mat(dat),-1);
            std::cout <<" decode " << tmr.elapsed() << std::endl;

            //std::cout << arr.length() << "vor rem "<< std::endl;
            arr = arr.remove(0,hdrStart+HEADER_SIZE+dataLen);
            //std::cout << arr.length() << "nach rem "<< std::endl;

            std::cout <<" kill " << tmr.elapsed() << std::endl;

            hdrStart = std::string::npos;
            dataLen = std::string::npos;



            ui->openGLWidget->display(img);
            std::cout <<" timer draw " << tmr.elapsed() << std::endl;
        }

}

void MainWindow::on_socketDisconnected()
{
    std::cout<< "bye connection!" << std::endl;
    ui->lbl->setText("Disconnected!");
    disconnect(m_ptrSock,SIGNAL(disconnected()),this,SLOT(on_socketDisconnected()));
    /*if(m_ptrSock)
        delete m_ptrSock;*/

    srvr.listen(QHostAddress::Any,port);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

}
