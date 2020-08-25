//sooScreenShare by Simon Wezstein (B-LechCode), 2019
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
    m_initOk(false)
{

}

mainWorker::~mainWorker()
{
    end();
}

void mainWorker::init(std::string decompBackend,std::string postDecompBackend,std::string transportBackend,/*Idraw* ptrDraw*/ std::string imageConsumerBackend)
{
//    if(!ptrDraw)
//        return;
//    m_ptrDraw = ptrDraw;



    setDecompressionBackend(decompBackend);
    setPostDecompressionBackend(postDecompBackend);
    setTransportBackend(transportBackend);
    setImageConsumerBackend(imageConsumerBackend);
}

void mainWorker::end()
{    
    if(m_decomp) delete m_decomp;
    m_decomp = nullptr;
    if(m_postDecomp) delete m_postDecomp;
    m_postDecomp = nullptr;
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

void mainWorker::setPostDecompressionBackend(std::string postDecompBackend)
{
    m_initOk = false;
    if(m_postDecomp)
        delete m_postDecomp;
    m_postDecomp   = imagePostDecompressorFactory::getBackend(postDecompBackend);

    if(m_postDecomp)
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
        m_trans->setObserver(static_cast<ItransportClientObserver*>(this));

        m_initOk = true;
    }
}

void mainWorker::setImageConsumerBackend(std::string consumerBackend)
{
    m_initOk = false;
    if(m_ptrDraw)
        delete m_ptrDraw;
    m_ptrDraw    = imageConsumerFactory::getBackend(consumerBackend);
}

IImagePostDecompressor *mainWorker::postDecomp() const
{
    return m_postDecomp;
}

Idraw *mainWorker::imageConsumer() const
{
    return m_ptrDraw;
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

    do
    {
        const std::string headerString(HEADER_STRING);
        auto ptrPacketStart = std::search(data+posNow2, data+dataLen, std::begin(headerString), std::end(headerString));
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
    uint8_t*  ptrmyBuff = m_receiveBuffer.data();
    size_t  dataLen = static_cast<size_t>(len);

    bool workingWithMemberBuffer = false;

    if(m_receiveBuffer.size()>0) //only use member buffer if necesary, better process in place!
    {
        size_t oldSize = m_receiveBuffer.size();
        m_receiveBuffer.resize(oldSize+dataLen);
        ptrmyBuff = m_receiveBuffer.data();
        memcpy(ptrmyBuff+oldSize,dat,dataLen);
        refBuff = ptrmyBuff;
        workingWithMemberBuffer = true;
        dataLen = m_receiveBuffer.size();
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
        cv::Mat imgPost = m_postDecomp->decompress(img,myHeader,ok);
        m_ptrDraw->displayImage(imgPost);

        size_t processedFrameSize = myPos+HEADER_SIZE+static_cast<size_t>(myHeader.length);
        if(workingWithMemberBuffer)
        {
            if(dataLen == processedFrameSize)
                m_receiveBuffer.clear();//We're done: erase everything
            else
                m_receiveBuffer.erase(m_receiveBuffer.begin(),m_receiveBuffer.begin()+static_cast<int>(processedFrameSize)); //We're only partially done, trim the member buffer
        }
        else if(dataLen > processedFrameSize)
        {
            //We have more than one frame in our input buffer, save the unprocessed to the member buffer!
            size_t oldSize = m_receiveBuffer.size();
            m_receiveBuffer.resize(oldSize+(dataLen-processedFrameSize));
            ptrmyBuff = m_receiveBuffer.data();
            memcpy(ptrmyBuff+oldSize,dat+processedFrameSize,(dataLen-(processedFrameSize)));
        }
    }
    else if(!workingWithMemberBuffer)
    {
        //We need to save the incomplete data!
        size_t oldSize = m_receiveBuffer.size();
        m_receiveBuffer.resize(oldSize+dataLen);
        ptrmyBuff = m_receiveBuffer.data();
        memcpy(ptrmyBuff+oldSize,dat,dataLen);
    }
}
