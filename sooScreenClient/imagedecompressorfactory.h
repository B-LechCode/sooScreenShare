#ifndef IMAGECOMPRESSORFACTORY_H
#define IMAGECOMPRESSORFACTORY_H

#include "opencvjpegimagedecompressor.h"
#include "lz4imagedecompressor.h"
#include "iimagedecompressor.h"

#define cvJpeg "jpeg"
#define lz4 "lz4"

class imageDecompressorFactory
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

    static IImageDecompressor* getBackend(std::string backendName)
    {
        if(backendName == cvJpeg)
            return new opencvJpegImageDecompressor();
        if(backendName == lz4)
            return new lz4ImageDecompressor();
        return nullptr;
    }
};

#endif // IMAGECOMPRESSORFACTORY_H
