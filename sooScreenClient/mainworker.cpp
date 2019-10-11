#include "mainworker.h"
#include "factories.h"
#include <opencv2/opencv.hpp>

#define DEBUG 0

#define HEADER_SIZE 64
#define HEADERSTRING_OFFSET 32

mainWorker::mainWorker():
    m_bufferSize(w*h*4+HEADER_SIZE),
    m_initOk(false)
{
     m_sendbuffer = new uint8_t[m_bufferSize];
}

mainWorker::~mainWorker()
{
    //if(m_screen) delete m_screen;
    if(m_decomp) delete m_decomp;
    if(m_trans) delete m_trans;
}

void mainWorker::init(Idraw* ptrDraw)
{
    if(!ptrDraw)
        return;
    m_ptrDraw = ptrDraw;
    //TODO parameter change
    //m_screen = screenShotFactory::getBackend(x11,x, y, w, h); //TODO set screen size!
    m_decomp   = imageDecompressorFactory::getBackend(cvJpeg);
    m_trans  = transportClientFactory::getBackend(qtTcpServer);
    m_trans->addObserverSubscriber(*(ItransportClientObserver*)this);
    m_trans->init();

    m_initOk = true;
}

void mainWorker::run()
{
    if(!m_initOk)
        return;

}

void mainWorker::end()
{

}

void mainWorker::transportDataAvailable(std::vector<uint8_t> data)
{
    static size_t hdrStart = std::string::npos;
    static size_t dataLen = std::string::npos;

    std::vector<uint8_t>* ptrRef = &data;
    bool workingWithMemberBuffer = false;
    if(myBuf.size()>0) //only "reasemble" if necesary!
    {
        myBuf.insert(myBuf.end(),data.begin(),data.end());
        ptrRef = &myBuf;
        workingWithMemberBuffer = true;
    }

    std::vector<uint8_t>& refBuff = *ptrRef;


    if(hdrStart == std::string::npos)
    {
        QElapsedTimer tmr;
        tmr.start();
        //Find the Header-String!
        std::string sdf = std::string(reinterpret_cast<char*>(refBuff.data()),refBuff.size());
        size_t headerStart = sdf.find("ScreenImage");

        if(headerStart != std::string::npos)
        {
            hdrStart = headerStart;
        }
    }

    if(hdrStart != std::string::npos && dataLen == std::string::npos && refBuff.size() >=hdrStart+HEADER_SIZE)
    {
        QElapsedTimer tmr;
        tmr.start();
        char* ptrZahl = reinterpret_cast<char*>(refBuff.data())+hdrStart+HEADERSTRING_OFFSET;
        int zahl;
        memcpy(&zahl,ptrZahl,sizeof(int));
        dataLen = zahl;
#if DEBUG
        std::cout <<" find size " << tmr.elapsed() << std::endl;
#endif
    }

    //Check if our length is long enough to have the whole img!
        if(hdrStart != std::string::npos && dataLen != std::string::npos && (hdrStart+dataLen) <= refBuff.size())
        {
            QElapsedTimer tmr;
            tmr.start();

#if DEBUG
            std::cout <<" copy " << tmr.elapsed() << std::endl;
#endif
            auto t = cv::Mat (1, dataLen, CV_8UC1,refBuff.data()+hdrStart+HEADER_SIZE);
            cv::Mat img = cv::imdecode(t,-1);
#if DEBUG
            std::cout <<" decode " << tmr.elapsed() << std::endl;
#endif
            if(refBuff.size() == (hdrStart+HEADER_SIZE+dataLen) && workingWithMemberBuffer)
                myBuf.clear();
            else if (workingWithMemberBuffer)
                myBuf.erase(myBuf.begin(),myBuf.begin()+hdrStart+HEADER_SIZE+dataLen);
#if DEBUG
            std::cout <<" kill " << tmr.elapsed() << std::endl;
#endif
            hdrStart = std::string::npos;
            dataLen = std::string::npos;


            m_ptrDraw->display(img);

#if DEBUG
            std::cout <<" timer draw " << tmr.elapsed() << std::endl;
#endif
        }
        else
        {
            myBuf.insert(myBuf.end(),data.begin(),data.end());
        }


}
