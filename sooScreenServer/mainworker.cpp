#include "mainworker.h"
#include "factories.h"
#include <opencv2/opencv.hpp>
#include "./../header.h"


/*MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_bufferSize(1920*1080*4+HEADER_SIZE)
{
    ui->setupUi(this);

    m_clientAddr = QHostAddress(client);

    m_screen = screenShotFactory::getBackend(x11,0, 0, 1920, 1080);
    m_comp   = imageCompressorFactory::getBackend(cvJpeg);


    if(m_screen && m_comp)
        sock.connectToHost(m_clientAddr, port,QAbstractSocket::WriteOnly);

    connect(&sock,SIGNAL(connected()),this,SLOT(on_socketConnected()));
    connect(&sock,SIGNAL(disconnected()),this,SLOT(on_socketDisconnected()));
    connect(&m_tmr,SIGNAL(timeout()),this,SLOT(on_timerTimeout()));
    m_tmr.setSingleShot(true);

    createHeader();

}*/

mainWorker::mainWorker():
    m_bufferSize(w*h*4+HEADER_SIZE)
{
     m_sendbuffer = new uint8_t[m_bufferSize];
}

mainWorker::~mainWorker()
{
    if(m_screen) delete m_screen;
    if(m_comp) delete m_comp;
    if(m_trans) delete m_trans;
}

void mainWorker::init()
{
    //TODO parameter change
    m_screen = screenShotFactory::getBackend(x11,x, y, w, h); //TODO set screen size!
    m_comp   = imageCompressorFactory::getBackend(cvJpeg);
    m_trans  = transportServerFactory::getBackend(qtTcpServer);
    m_trans->addObserverSubscriber(*(ItransportServerObserver*)this);
    m_trans->init();
    createHeader();
}

void mainWorker::run()
{

    bool compressOk;

    //Get Screenshot
    cv::Mat img = m_screen->operator()();

    //Compress the image
    std::vector<uint8_t> compressedImageData =  m_comp->compress(img,compressOk);

    if(!compressOk)
        return; //TODO user notification

    //check size
    if((compressedImageData.size()+HEADER_SIZE)>m_bufferSize)
    {
        delete[] m_sendbuffer;
        m_bufferSize = compressedImageData.size()+HEADER_SIZE;
        m_sendbuffer = new uint8_t[m_bufferSize];
        createHeader();
    }

    //Add Header
    insertHeaderNumBytes(static_cast<int32_t>(compressedImageData.size()));

    memcpy(m_sendbuffer+HEADER_SIZE,compressedImageData.data(),compressedImageData.size());


    //send
    m_trans->send(reinterpret_cast<char*>(m_sendbuffer),static_cast<int64_t>(compressedImageData.size()+HEADER_SIZE));

    img.release();
}

void mainWorker::end()
{

}

//Helpers
void mainWorker::createHeader()
{
    char header[] = "ScreenImage";

    memcpy(m_sendbuffer,header,sizeof(header));
    memset(m_sendbuffer+sizeof(header),0,HEADER_SIZE-sizeof(header));
}

void mainWorker::insertHeaderNumBytes(int byteCount)
{
    memcpy(m_sendbuffer+HEADERSTRING_OFFSET,&byteCount,sizeof(byteCount));
}
