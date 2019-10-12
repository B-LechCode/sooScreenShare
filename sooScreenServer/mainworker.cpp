#include "mainworker.h"
#include "factories.h"
#include <opencv2/opencv.hpp>
#include "./../header.h"
#include <stddef.h>


mainWorker::mainWorker():
    m_bufferSize(w*h*4+HEADER_SIZE)
{
     m_sendbuffer = new uint8_t[m_bufferSize];
     std::cout <<  sizeof( dataHeaderHandling::dataHeader) << std::endl;
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
    insertHeaderNumBytes(static_cast<int32_t>(compressedImageData.size()),static_cast<int32_t>(w),static_cast<int32_t>(h));

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
    dataHeaderHandling::dHdr hdr;
    memcpy(m_sendbuffer,&hdr,sizeof(hdr));

}

void mainWorker::insertHeaderNumBytes(int byteCount,int width,int height)
{
    dataHeaderHandling::dHdr* hdr = reinterpret_cast<dataHeaderHandling::dHdr*>(m_sendbuffer);
    hdr->length = byteCount;
    hdr->width = width;
    hdr->height = height;
}
