#ifndef IMAGECOMPRESSORFACTORY_H
#define IMAGECOMPRESSORFACTORY_H

#include "opencvjpegimagecompressor.h"
#include "lz4imagecompressor.h"
#include "iimagecompressor.h"

#define cvJpeg "jpeg"
#define lz4 "lz4"

class imageCompressorFactory
{
    static std::vector<std::string> m_backends;
public:
    static const std::vector<std::string>& getAvailableBackends()
    {
        if(!m_backends.size())
        {
            m_backends.push_back(cvJpeg);
            m_backends.push_back(lz4);
        }
        return  m_backends;
    }

    static IImageCompressor* getBackend(std::string backendName)
    {
        if(backendName == cvJpeg)
            return new opencvJpegImageCompressor();
        if(backendName == lz4)
            return new lz4ImageCompressor();

        return nullptr;
    }
};

#endif // IMAGECOMPRESSORFACTORY_H
