//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
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
    end();
    if(m_sendbuffer)
        delete[] m_sendbuffer;
}

void mainWorker::init(std::string decompBackend,std::string transportBackend,Idraw* ptrDraw)
{
    if(!ptrDraw)
        return;
    m_ptrDraw = ptrDraw;
    //TODO parameter change    
    setDecompressionBackend(decompBackend);
    setTransportBackend(transportBackend);
}

void mainWorker::run()
{
    if(!m_initOk)
        return;
    m_trans->init();
}

void mainWorker::end()
{    
    if(m_decomp) delete m_decomp;
    m_decomp = nullptr;
    if(m_trans) delete m_trans;
    m_trans = nullptr;
}

void mainWorker::setDecompressionBackend(std::string decompBackend)
{
    m_initOk = false;
    if(m_decomp)
        delete m_decomp;
    m_decomp   = imageDecompressorFactory::getBackend(decompBackend);

    if(m_decomp)
        m_initOk = true;
}

void mainWorker::setTransportBackend(std::string transportBackend)
{
    m_initOk = false;
    if(m_trans)
        delete m_trans;
    m_trans    = transportClientFactory::getBackend(transportBackend);


    if(m_trans)
    {
        m_trans->addObserverSubscriber(*reinterpret_cast<ItransportClientObserver*>(this));

        m_initOk = true;
    }
}

IImageDecompressor *mainWorker::decomp() const
{
    return m_decomp;
}

ItransportClient *mainWorker::trans() const
{
    return m_trans;
}

dataHeaderHandling::dataHeader checkFrameAvail(uint8_t* data,size_t dataLen,size_t pos)
//dataHeaderHandling::dataHeader checkFrameAvail(std::vector<uint8_t>& data,size_t pos)
{
    dataHeaderHandling::dataHeader retVal;
    if(dataLen>pos+HEADER_SIZE)
    {
        dataHeaderHandling::dataHeader* ptrHeader = reinterpret_cast<dataHeaderHandling::dataHeader*>(data+pos);
        retVal = *ptrHeader;

        if(!(dataLen >= (pos+HEADER_SIZE+static_cast<uint32_t>(retVal.length))))
            retVal.length = -1;
    }
    else
    {
        retVal.length = -1;
    }
    return retVal;

}

int getLastFullAvailableFrame(uint8_t* data,size_t dataLen,size_t& pos, dataHeaderHandling::dataHeader& header)
{   
    int num2=0;
    size_t posNow2 = 0,pos2=0;

    dataHeaderHandling::dataHeader headerNow2;
    headerNow2.length = -1;

	const std::string headerStr = std::string(HEADER_STRING);

    do
    {
        auto ptrPacketStart = std::search(data+posNow2, data+dataLen, std::begin(headerStr), std::end(headerStr));
        if(ptrPacketStart < data+dataLen) //When something is found
        {
            posNow2 = static_cast<size_t>(ptrPacketStart-data);

            headerNow2 = checkFrameAvail(data,dataLen,posNow2);

            if(headerNow2.length != -1) //Frame is available!
            {
                pos2 = posNow2;
                header = headerNow2;
                num2++;
                posNow2 += static_cast<size_t>(header.length); //Increment the whole frame!

                if(posNow2>dataLen) //Break at data end
                    break;
            }
        }
        else    //In case of nothing found
            break;


    }while(headerNow2.length != -1);



    pos = pos2;

    return num2;
}

void mainWorker::transportDataAvailable(const char *dat, int64_t len)
{ 
    static uint32_t droppedFrames = 0;

    uint8_t* refBuff = reinterpret_cast<uint8_t*>(const_cast<char*>(dat));
    uint8_t*  ptrmyBuff = myBuf.data();
    size_t  dataLen = static_cast<size_t>(len);

    bool workingWithMemberBuffer = false;

    if(myBuf.size()>0) //only use member buffer if necesary, better process in place!
    {
        size_t oldSize = myBuf.size();
        myBuf.resize(oldSize+dataLen);
        ptrmyBuff = myBuf.data();
        memcpy(ptrmyBuff+oldSize,dat,dataLen);
        refBuff = ptrmyBuff;
        workingWithMemberBuffer = true;
        dataLen = myBuf.size();
    }



    size_t myPos;
    dataHeaderHandling::dataHeader    myHeader;
    int    myCount;

    if((myCount = getLastFullAvailableFrame(refBuff,dataLen,myPos,myHeader)))
    {
        droppedFrames += static_cast<uint32_t>(myCount)-1;

#if DEBUG
        if(myCount>1)
            std::cout << " droppedFrames " <<droppedFrames << std::endl;
#endif
        bool ok;
        cv::Mat img = m_decomp->decompress(refBuff+myPos+HEADER_SIZE,myHeader,ok);

        m_ptrDraw->display(img);

        size_t processedFrameSize = myPos+HEADER_SIZE+static_cast<size_t>(myHeader.length);
        if(workingWithMemberBuffer)
        {
            if(dataLen == processedFrameSize)
                myBuf.clear();//We're done: erase everything
            else
                myBuf.erase(myBuf.begin(),myBuf.begin()+static_cast<int>(processedFrameSize)); //We're only partially done, trim the member buffer
        }
        else if(dataLen > processedFrameSize)
        {
            //We have more than one frame in our input buffer, save the unprocessed to the member buffer!
            size_t oldSize = myBuf.size();
            myBuf.resize(oldSize+(dataLen-processedFrameSize));
            ptrmyBuff = myBuf.data();
            memcpy(ptrmyBuff+oldSize,dat+processedFrameSize,(dataLen-(processedFrameSize)));
        }
    }
    else if(!workingWithMemberBuffer)
    {
        //We need to save the incomplete data!
        size_t oldSize = myBuf.size();
        myBuf.resize(oldSize+dataLen);
        ptrmyBuff = myBuf.data();
        memcpy(ptrmyBuff+oldSize,dat,dataLen);
    }
}
