#include "mainworker.h"
#include "factories.h"
#include <opencv2/opencv.hpp>


#define HEADER_SIZE 64
#define HEADERSTRING_OFFSET 32

mainWorker::mainWorker():
    m_bufferSize(w*h*4+HEADER_SIZE)
{
     m_sendbuffer = new uint8_t[m_bufferSize];
}

mainWorker::~mainWorker()
{
    //if(m_screen) delete m_screen;
    if(m_decomp) delete m_decomp;
    if(m_trans) delete m_trans;
}

void mainWorker::init()
{
    //TODO parameter change
    //m_screen = screenShotFactory::getBackend(x11,x, y, w, h); //TODO set screen size!
    m_decomp   = imageDecompressorFactory::getBackend(cvJpeg);
    m_trans  = transportClientFactory::getBackend(qtTcpServer);
    m_trans->addObserverSubscriber(*(ItransportClientObserver*)this);
    m_trans->init();
    createHeader();
}

void mainWorker::run()
{

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
