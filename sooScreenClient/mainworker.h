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
    void init();
    void run();
    void end();
private:
    //IscreenShot* m_screen = nullptr;
    IImageDecompressor* m_decomp = nullptr;
    ItransportClient* m_trans = nullptr;
    uint8_t*    m_sendbuffer = nullptr;
    uint64_t         m_bufferSize;
    void createHeader();
    void insertHeaderNumBytes(int byteCount);

    virtual void transportNewMessage(std::string str)
    {
        std::cout << str << std::endl;
    }

};

#endif // MAINWORKER_H