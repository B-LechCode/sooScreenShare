//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "interfaces.h"

#include <iostream>

class mainWorker:private ItransportClientObserver
{
    const int32_t x=0,y=0;
    const uint32_t w=1920,h=1080;
public:
    mainWorker();
    virtual ~mainWorker();
    void init(std::string decompBackend, std::string transportBackend, Idraw* ptrDraw);
    void run();
    void end();

    void setDecompressionBackend(std::string decompBackend);
    void setTransportBackend(std::string transportBackend);
    IImageDecompressor *decomp() const;
    ItransportClient *trans() const;

private:
    //IscreenShot* m_screen = nullptr;
    IImageDecompressor* m_decomp = nullptr;
    ItransportClient* m_trans = nullptr;
    uint8_t*    m_sendbuffer = nullptr;
    uint64_t         m_bufferSize;
    Idraw*      m_ptrDraw = nullptr;

    std::vector<uint8_t> myBuf;
    bool m_initOk;


    virtual void transportNewMessage(std::string str)
    {
        std::cout << str << std::endl;
    }    
    virtual void transportDataAvailable(const char *dat, int64_t len);


};

#endif // MAINWORKER_H
