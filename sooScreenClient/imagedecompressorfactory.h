#ifndef IMAGECOMPRESSORFACTORY_H
#define IMAGECOMPRESSORFACTORY_H

#include "opencvjpegimagedecompressor.h"
#include "iimagedecompressor.h"

#define cvJpeg "jpeg"

class imageDecompressorFactory
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

    static IImageDecompressor* getBackend(std::string backendName)
    {
        if(backendName == cvJpeg)
            return new opencvJpegImageDecompressor();

        return nullptr;
    }
};

#endif // IMAGECOMPRESSORFACTORY_H
