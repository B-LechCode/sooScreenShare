#ifndef IMAGECOMPRESSORFACTORY_H
#define IMAGECOMPRESSORFACTORY_H

#include "opencvjpegimagecompressor.h"
#include "iimagecompressor.h"

#define cvJpeg "jpeg"

class imageCompressorFactory
{
    static std::vector<std::string> m_backends;
public:
    static const std::vector<std::string>& getAvailableBackends()
    {
        if(!m_backends.size())
        {
            m_backends.push_back(cvJpeg);
        }
        return  m_backends;
    }

    static IImageCompressor* getBackend(std::string backendName)
    {
        if(backendName == cvJpeg)
            return new opencvJpegImageCompressor();

        return nullptr;
    }
};

#endif // IMAGECOMPRESSORFACTORY_H