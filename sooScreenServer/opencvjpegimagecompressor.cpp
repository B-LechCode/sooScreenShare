//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "opencvjpegimagecompressor.h"


opencvJpegImageCompressor::opencvJpegImageCompressor():IImageCompressor()
{
    m_defaultParameters[QUALITY] = parameter("The jpeg compression quality","int","95");
    setParameters(m_defaultParameters);
}

opencvJpegImageCompressor::~opencvJpegImageCompressor()
{

}

void opencvJpegImageCompressor::initParameters()
{
    //generate openCv parameters
    m_compressionParams.clear();
    m_compressionParams.push_back(cv::IMWRITE_JPEG_QUALITY);
    bool ok;
    int compQual = m_parameters[QUALITY].valueInt(ok);
    if (ok)
        m_compressionParams.push_back(compQual);
}

void opencvJpegImageCompressor::parameterMapChangedEvent()
{
    initParameters();
}

void opencvJpegImageCompressor::parameterChangedEvent(const std::string &key)
{
    initParameters();
}
