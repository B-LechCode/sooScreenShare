#include "mainworker.h"
#include "factories.h"
#include <opencv2/opencv.hpp>

#define DEBUG 1

#include "./../header.h"

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

int checkFrameAvail(std::vector<uint8_t>& data,size_t pos)
{
    int retVal = -1;
    if(data.size()>pos+HEADER_SIZE)
    {
        char* ptrZahl = reinterpret_cast<char*>(data.data())+pos+HEADERSTRING_OFFSET;
        int zahl;
        memcpy(&zahl,ptrZahl,sizeof(int));

        if(data.size() >= (pos+HEADER_SIZE+zahl))
            retVal = zahl;
    }
    return retVal;

}

int getLastFullAvailableFrame(std::vector<uint8_t>& data,size_t& pos, int& length)
{
    std::string in = std::string(reinterpret_cast<char*>(data.data()),data.size());
    int num = 0;
    size_t posNow = 0;
    int lengthNow = -1;
    do
    {
        length = lengthNow;
        pos = posNow-1;
        posNow = in.find("ScreenImage",posNow);
        lengthNow = checkFrameAvail(data,posNow);

        posNow++;
        num += static_cast<int>(posNow != std::string::npos && lengthNow>0);
    }while(lengthNow != -1);

    /*std::cout << " numHdr " <<num << std::endl;
    std::cout << " pos " <<pos << std::endl;
    std::cout << " length " <<length << std::endl;*/
    return num;
}

void mainWorker::transportDataAvailable(std::vector<uint8_t> data)
{ 
    static uint32_t droppedFrames = 0;

    std::vector<uint8_t>* ptrRef = &data;
    bool workingWithMemberBuffer = false;

    if(myBuf.size()>0) //only use member buffer if necesary, better process in place!
    {
        myBuf.insert(myBuf.end(),data.begin(),data.end());
        ptrRef = &myBuf;
        workingWithMemberBuffer = true;
    }

    std::vector<uint8_t>& refBuff = *ptrRef;

    size_t myPos;
    int    myLength, myCount;

    if((myCount = getLastFullAvailableFrame(refBuff,myPos,myLength)))
    {
        droppedFrames += myCount-1;

#if DEBUG
        if(myCount>1)
            std::cout << " droppedFrames " <<droppedFrames << std::endl;
#endif
        auto t = cv::Mat (1, myLength, CV_8UC1,refBuff.data()+myPos+HEADER_SIZE);
        cv::Mat img = cv::imdecode(t,-1);
        m_ptrDraw->display(img);


        if(workingWithMemberBuffer)
        {
            if(refBuff.size() == (myPos+HEADER_SIZE+myLength))
                myBuf.clear();//We're done: erase everything
            else
                myBuf.erase(myBuf.begin(),myBuf.begin()+myPos+HEADER_SIZE+myLength); //We're only partially done, trim the member buffer
        }
        else if(refBuff.size() > (myPos+HEADER_SIZE+myLength))
                myBuf.insert(myBuf.end(),data.begin()+myPos+HEADER_SIZE+myLength,data.end()); //We have more than one frame in our input buffer, save the unprocessed to the member buffer!
    }
    else if(!workingWithMemberBuffer)
    {
        myBuf.insert(myBuf.end(),data.begin(),data.end()); //We need to save the incomplete data!
    }
}
