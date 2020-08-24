//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "mainworker.h"
#include "factories.h"
#include <opencv2/opencv.hpp>
#include "./../header.h"
#include <stddef.h>


mainWorker::mainWorker():
    m_bufferSize(0), m_sendbuffer(nullptr)
{
     std::cout <<  sizeof( dataHeaderHandling::dataHeader) << std::endl;
}

mainWorker::~mainWorker()
{
    if(m_screen) delete m_screen;
    m_screen = nullptr;
    if(m_preComp) delete m_preComp;
    m_preComp = nullptr;
    if(m_comp) delete m_comp;
    m_comp = nullptr;
    if(m_trans) delete m_trans;
    m_trans = nullptr;
    if(m_sendbuffer) delete[] m_sendbuffer;
    m_sendbuffer = nullptr;
}

void mainWorker::init(std::string screenShotBackend,std::string imagePreCompressorBackend,std::string imageCompressorBackend, std::string transportServerBackend)
{   
    m_screen = screenShotFactory::getBackend(screenShotBackend);
    m_preComp = imagePreCompressorFactory::getBackend(imagePreCompressorBackend);
    m_comp   = imageCompressorFactory::getBackend(imageCompressorBackend);
    m_trans  = transportServerFactory::getBackend(transportServerBackend);
    m_trans->setObserver(static_cast<ItransportServerObserver*>(this));

    cv::Mat data_size_screenshot = (*m_screen)();
    m_bufferSize = data_size_screenshot.elemSize()*data_size_screenshot.total() + HEADER_SIZE;
    m_sendbuffer = new uint8_t[m_bufferSize];
    createHeader();
}

void mainWorker::run()
{
    if(!m_comp || !m_preComp || !m_trans || !m_screen)
        return;
    static size_t lastKeyframeSize = 0;
    bool compressOk;

    //Get Screenshot
    cv::Mat img = m_screen->operator()();
    imageType tp;
    cv::Mat cImg = m_preComp->compress(img,tp,compressOk);

    //check if buffer is too small for the screenshot and increase buffer size if necessary
    if(size_t data_size = cImg.elemSize()*cImg.total() + HEADER_SIZE > m_bufferSize){
        m_bufferSize = data_size;
        delete[] m_sendbuffer;
        m_sendbuffer = new uint8_t[m_bufferSize];
        createHeader();
    }

    if(!compressOk)
        return; //TODO user notification

    //Compress the image
    size_t compressedSize = m_comp->compress(m_sendbuffer+HEADER_SIZE,m_bufferSize-HEADER_SIZE,cImg,compressOk);

    if(!compressOk)
        return; //TODO user notification

    std::cout << tp << " ";
    std::cout << "Data Size: " << compressedSize << std::endl;

    if (tp == imageType::incrementalFrame)
    {
        if(compressedSize > lastKeyframeSize*1.2)
        {
            std::cout << "Resetting precompression! " << std::endl;
            m_preComp->reset();
            cImg = m_preComp->compress(img,tp,compressOk);
            compressedSize = m_comp->compress(m_sendbuffer+HEADER_SIZE,m_bufferSize-HEADER_SIZE,cImg,compressOk);
            lastKeyframeSize = compressedSize;
            std::cout << tp << " ";
            std::cout << "Data Size: " << compressedSize << std::endl;
        }
    }
    else if (tp == imageType::keyFrame)
    {
        lastKeyframeSize = compressedSize;
    }


    //check size for buffer
    /*if((compressedImageData.size()+HEADER_SIZE)>m_bufferSize)
    {
        delete[] m_sendbuffer;
        m_bufferSize = compressedImageData.size()+HEADER_SIZE;
        m_sendbuffer = new uint8_t[m_bufferSize];
        createHeader();
    }*/ //TODO: reimplement meeting requirements!

    //Add Header
    insertHeaderInfo(static_cast<int32_t>(compressedSize),static_cast<int32_t>(img.cols),static_cast<int32_t>(img.rows),img.type(),tp);

    //send
    m_trans->send(reinterpret_cast<char*>(m_sendbuffer),static_cast<int64_t>(compressedSize+HEADER_SIZE));

    img.release();
}

void mainWorker::end()
{
    if(m_screen) delete m_screen;
    m_screen = nullptr;
    if(m_comp) delete m_comp;
    m_comp = nullptr;
    if(m_trans) delete m_trans;
    m_trans = nullptr;
}

IscreenShot *mainWorker::screen() const
{
    return m_screen;
}

IImageCompressor *mainWorker::comp() const
{
    return m_comp;
}

IImagePreCompressor *mainWorker::preComp() const
{
    return m_preComp;
}

ItransportServer *mainWorker::trans() const
{
    return m_trans;
}

//Helpers
void mainWorker::createHeader()
{    
    dataHeaderHandling::dHdr hdr;
    memcpy(m_sendbuffer,&hdr,sizeof(hdr));

}

void mainWorker::insertHeaderInfo(int byteCount, int width, int height, int cvType,imageType tp)
{
    dataHeaderHandling::dHdr* hdr = reinterpret_cast<dataHeaderHandling::dHdr*>(m_sendbuffer);
    hdr->length = byteCount;
    hdr->width = width;
    hdr->height = height;
    hdr->cvType = cvType;
    hdr->imageDataType = static_cast<int32_t>(tp);
}
