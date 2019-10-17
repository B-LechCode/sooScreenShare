//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "interfaces.h"
#define defaultBufferSize  1920*1200*4
#include <iostream>

class mainWorker:private ItransportServerObserver
{    
public:
    mainWorker();
    virtual ~mainWorker();
    void init();
    void run();
    void end();
private:
    IscreenShot* m_screen = nullptr;
    IImageCompressor* m_comp = nullptr;
    ItransportServer* m_trans = nullptr;
    uint8_t*    m_sendbuffer = nullptr;
    uint64_t         m_bufferSize;

    void createHeader();
    void insertHeaderNumBytes(int byteCount,int width,int height,int cvType);
    virtual void transportNewMessage(std::string str)
    {
        std::cout << str << std::endl;
    }

};

#endif // MAINWORKER_H
