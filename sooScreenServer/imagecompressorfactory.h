//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IMAGECOMPRESSORFACTORY_H
#define IMAGECOMPRESSORFACTORY_H

#include "opencvjpegimagecompressor.h"
#include "lz4imagecompressor.h"
#include "iimagecompressor.h"

#define cvJpeg "jpeg"
#define lz4 "lz4"

class imageCompressorFactory
{
    imageCompressorFactory(){}
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
