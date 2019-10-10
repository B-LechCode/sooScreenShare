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

#include <iostream>

#include "screenshotfactory.h"
#include "imagecompressorfactory.h"




#define HEADER_SIZE 64
#define HEADERSTRING_OFFSET 32

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bufferSize(1600 * 900 * 4 + HEADER_SIZE)
{
    ui->setupUi(this);

    m_clientAddr = QHostAddress(client);

    m_screen = screenShotFactory::getBackend(backend_win, 0, 0, 1600, 900);
    m_comp   = imageCompressorFactory::getBackend(cvJpeg);

    m_sendbuffer = new uint8_t[m_bufferSize];
    if(m_screen && m_comp)
        sock.connectToHost(m_clientAddr, port,QAbstractSocket::WriteOnly);

    connect(&sock,SIGNAL(connected()),this,SLOT(on_socketConnected()));
    connect(&sock,SIGNAL(disconnected()),this,SLOT(on_socketDisconnected()));
    connect(&m_tmr,SIGNAL(timeout()),this,SLOT(on_timerTimeout()));
    m_tmr.setSingleShot(true);

    createHeader();

    m_work.init();

}



void MainWindow::createHeader()
{
    char header[] = "ScreenImage";

    memcpy(m_sendbuffer,header,sizeof(header));
    memset(m_sendbuffer+sizeof(header),0,HEADER_SIZE-sizeof(header));
}

void MainWindow::insertHeaderNumBytes(int byteCount)
{
    memcpy(m_sendbuffer+HEADERSTRING_OFFSET,&byteCount,sizeof(byteCount));
}

void MainWindow::on_socketConnected()
{
    //pbc();
}

void MainWindow::on_socketDisconnected()
{
    m_tmr.stop();
    m_tmr.start(1000);

}

void MainWindow::on_timerTimeout()
{
    m_work.run();
    //pbc();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_screen) delete m_screen;
    if(m_sendbuffer) delete[] m_sendbuffer;
}



void MainWindow::on_pushButton_clicked()
{
    m_tmr.start(40);
}
void MainWindow::pbc()
{

    if(!sock.isOpen())
    {
        if(m_screen)
            sock.connectToHost(m_clientAddr, port,QAbstractSocket::WriteOnly);

        sock.waitForConnected(100);

        if(!sock.isOpen())
            m_tmr.start(1000);

        return;
    }

    QElapsedTimer t2;
    int64_t rval;
    t2.start();

    IscreenShot& screen = *m_screen;


    cv::Mat imgCrop = screen();   

    std::vector<uint8_t> buf = m_comp->compress(imgCrop);

    /*if(!encRet)
    {
        qDebug() << "ERROR ENCODE!!";
    }
    else*/
    {
        if((buf.size()+HEADER_SIZE)>m_bufferSize)
        {
            delete[] m_sendbuffer;
            m_bufferSize = buf.size()+HEADER_SIZE;
            m_sendbuffer = new uint8_t[m_bufferSize];
            createHeader();
        }

        insertHeaderNumBytes(static_cast<int32_t>(buf.size()));

        memcpy(m_sendbuffer+HEADER_SIZE,buf.data(),buf.size());

        std::cout << "Size: " << buf.size() << " Buffer size: " << m_bufferSize << std::endl;

        rval = sock.write(reinterpret_cast<char*>(m_sendbuffer),static_cast<int32_t>(buf.size()+HEADER_SIZE));

        imgCrop.release();


        while(sock.bytesToWrite())
            sock.flush();
    }

    int64_t eli = t2.elapsed();
    uint64_t el = static_cast<uint64_t>(eli);


    m_sumTime += el;
    m_couterTime++;
    ui->lblCurrent->setText(QString::number(el)+" ms");
    ui->lblMean->setText(QString::number(static_cast<double>(m_sumTime)/static_cast<double>(m_couterTime))+" ms");
    if(el>m_maxTime)
    {
        m_maxTime = el;
        ui->lblMax->setText(QString::number(el)+" ms");
    }
    if(el<m_minTime)
    {
        m_minTime = el;
        ui->lblMin->setText(QString::number(el)+" ms");
    }

    int64_t diff = m_timePreference-eli;
    if(diff>0)
        m_tmr.start(static_cast<int>(diff));
    else
        m_tmr.start(0);
}
