//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
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
    m_screen = nullptr;
    if(m_comp) delete m_comp;
    m_comp = nullptr;
    if(m_trans) delete m_trans;
    m_trans = nullptr;
    if(m_sendbuffer) delete[] m_sendbuffer;
    m_sendbuffer = nullptr;
}

void mainWorker::init(std::string screenShotBackend,std::string imageCompressorBackend, std::string transportServerBackend)
{
    //TODO parameter change
    m_screen = screenShotFactory::getBackend(screenShotBackend); //TODO set screen size!
    //m_screen->initialize(screens[i].x,screens[i].y,screens[i].w,screens[i].h);

    m_comp   = imageCompressorFactory::getBackend(imageCompressorBackend);
    m_trans  = transportServerFactory::getBackend(transportServerBackend);
    m_trans->setObserver(static_cast<ItransportServerObserver*>(this));
    createHeader();
}

void mainWorker::run()
{
    if(!m_comp || !m_trans || !m_screen)
        return;
    bool compressOk;

    //Get Screenshot
    cv::Mat img = m_screen->operator()();

    //Compress the image
    std::vector<uint8_t> compressedImageData =  m_comp->compress(img,compressOk);

    if(!compressOk)
        return; //TODO user notification
    //std::cout << compressedImageData.size() << std::endl;
    //check size
    if((compressedImageData.size()+HEADER_SIZE)>m_bufferSize)
    {
        delete[] m_sendbuffer;
        m_bufferSize = compressedImageData.size()+HEADER_SIZE;
        m_sendbuffer = new uint8_t[m_bufferSize];
        createHeader();
    }

    //Add Header
    insertHeaderInfo(static_cast<int32_t>(compressedImageData.size()),static_cast<int32_t>(img.cols),static_cast<int32_t>(img.rows),img.type());

    memcpy(m_sendbuffer+HEADER_SIZE,compressedImageData.data(),compressedImageData.size());


    //send
    m_trans->send(reinterpret_cast<char*>(m_sendbuffer),static_cast<int64_t>(compressedImageData.size()+HEADER_SIZE));

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

void mainWorker::insertHeaderInfo(int byteCount, int width, int height, int cvType)
{
    dataHeaderHandling::dHdr* hdr = reinterpret_cast<dataHeaderHandling::dHdr*>(m_sendbuffer);
    hdr->length = byteCount;
    hdr->width = width;
    hdr->height = height;
    hdr->cvType = cvType;
}
