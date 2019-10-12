#ifndef HEADER_H
#define HEADER_H

#define HEADER_SIZE 64
#define HEADERSTRING_OFFSET 32

//#define compressback cvJpeg
#define compressback lz4

class dataHeaderHandling
{
public:
    typedef struct dataHeader {
       char headerString[16] = "ScreenImage"; //16
       char reserved[16];     //32
       int  length;           //36
       int  width;            //40
       int  height;           //44
       int  cvType;           //48 //TODO: channels?
       char reserved2[16];    //64
    } dHdr;
};

#endif
