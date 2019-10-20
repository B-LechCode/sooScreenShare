//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef OPENCVJPEGIMAGECOMPRESSOR_H
#define OPENCVJPEGIMAGECOMPRESSOR_H
#include "iimagecompressor.h"

#include <utility>

namespace comp
{
    #define QUALITY "JPEG_QUALITY"
}

class opencvJpegImageCompressor : public IImageCompressor
{
public:
    opencvJpegImageCompressor();
    virtual ~opencvJpegImageCompressor();    

    inline virtual std::vector<uint8_t> compress(cv::Mat& img, bool& ok)
    {
        ok = compressHelper(img);
        return std::move(m_buffer);
    }

    inline virtual std::vector<uint8_t> compress(cv::Mat& img)
    {
        compressHelper(img);
        return std::move(m_buffer);
    }
    inline bool compressHelper(cv::Mat& img)
    {
        return  cv::imencode(".jpg",img,m_buffer,m_compressionParams);
    }
private:
    void initParameters();
    virtual void parameterMapChangedEvent();
    virtual void parameterChangedEvent(const std::string& key);
    std::vector<uint8_t> m_buffer;
    std::vector<int> m_compressionParams;
};

#endif // OPENCVJPEGIMAGECOMPRESSOR_H
