#include "mainworker.h"
#include "factories.h"
#include <opencv2/opencv.hpp>
#include <algorithm>
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
    m_decomp   = imageDecompressorFactory::getBackend(compressback);
    m_trans    = transportClientFactory::getBackend(qtTcpServer);
    m_trans->addObserverSubscriber(*reinterpret_cast<ItransportClientObserver*>(this));
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

dataHeaderHandling::dataHeader checkFrameAvail(std::vector<uint8_t>& data,size_t pos)
{
    dataHeaderHandling::dataHeader retVal;
    if(data.size()>pos+HEADER_SIZE)
    {
        dataHeaderHandling::dataHeader* ptrHeader = reinterpret_cast<dataHeaderHandling::dataHeader*>(data.data()+pos);
        retVal = *ptrHeader;

        if(!(data.size() >= (pos+HEADER_SIZE+static_cast<uint32_t>(retVal.length))))
            retVal.length = -1;
    }
    else
    {
        retVal.length = -1;
    }
    return retVal;

}

int getLastFullAvailableFrame(std::vector<uint8_t>& data,size_t& pos, dataHeaderHandling::dataHeader& header)
{   
    int num2=0;
    size_t posNow2 = 0,pos2=0;

    dataHeaderHandling::dataHeader headerNow2;
    headerNow2.length = -1;

	const std::string headerStr = std::string(HEADER_STRING);

    do
    {
        auto test = std::search(data.data()+posNow2, data.data()+data.size(), std::begin(headerStr), std::end(headerStr));
        if(test < data.data()+data.size()) //When something is found
        {
            posNow2 = test-data.data();

            headerNow2 = checkFrameAvail(data,posNow2);

            if(headerNow2.length != -1) //Frame is available!
            {
                pos2 = posNow2;
                header = headerNow2;
                num2++;
                posNow2 += header.length; //Increment the whole frame!

                if(posNow2>data.size())
                    break;
            }
        }
        else    //In case of nothing found
            break;


    }while(headerNow2.length != -1);



    pos = pos2;

    return num2;
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
    dataHeaderHandling::dataHeader    myHeader;
    int    myCount;

    if((myCount = getLastFullAvailableFrame(refBuff,myPos,myHeader)))
    {
        droppedFrames += static_cast<uint32_t>(myCount)-1;

#if DEBUG
        if(myCount>1)
            std::cout << " droppedFrames " <<droppedFrames << std::endl;
#endif
        bool ok;
        cv::Mat img = m_decomp->decompress(refBuff.data()+myPos+HEADER_SIZE,myHeader,ok);

        m_ptrDraw->display(img);


        if(workingWithMemberBuffer)
        {
            if(refBuff.size() == (myPos+HEADER_SIZE+myHeader.length))
                myBuf.clear();//We're done: erase everything
            else
                myBuf.erase(myBuf.begin(),myBuf.begin()+myPos+HEADER_SIZE+myHeader.length); //We're only partially done, trim the member buffer
        }
        else if(refBuff.size() > (myPos+HEADER_SIZE+myHeader.length))
                myBuf.insert(myBuf.end(),data.begin()+myPos+HEADER_SIZE+myHeader.length,data.end()); //We have more than one frame in our input buffer, save the unprocessed to the member buffer!
    }
    else if(!workingWithMemberBuffer)
    {
        myBuf.insert(myBuf.end(),data.begin(),data.end()); //We need to save the incomplete data!
    }
}
