#ifndef HEADER_H
#define HEADER_H
#include <cstdint>
#define HEADER_SIZE 64
#define HEADERSTRING_OFFSET 32
#define HEADER_STRING "ScreenImage"
//#define compressback cvJpeg
#define compressback lz4

class dataHeaderHandling
{
public:
    typedef struct dataHeader {
       char headerString[16] = HEADER_STRING; //16
       char reserved[16];     //32
       int32_t  length;           //36
       uint32_t  width;            //40
       uint32_t  height;           //44
       int32_t  cvType;           //48 //TODO: channels?
       char reserved2[16];    //64
    } dHdr;
};

#endif
