//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef LZ4IMAGECOMPRESSOR_H
#define LZ4IMAGECOMPRESSOR_H
#include "iimagecompressor.h"

#include <utility>
#include <lz4.h>

namespace comp
{
    //#define QUALITY "JPEG_QUALITY"
}

class lz4ImageCompressor : public IImageCompressor
{
public:
    lz4ImageCompressor();
    virtual ~lz4ImageCompressor();

    virtual void setParameters(parameterMap& para);

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
        int src_size = img.rows*img.cols*img.channels();
        m_buffer = std::vector<uint8_t>(src_size);
        int compSize = LZ4_compress_default(reinterpret_cast<char*>(img.ptr()),reinterpret_cast<char*>(m_buffer.data()),src_size,m_buffer.size());
        m_buffer.resize(compSize);
        return  compSize>0;
    }
private:
    virtual void parameterMapChangedEvent();
    virtual void parameterChangedEvent(const std::string& key);
    std::vector<uint8_t> m_buffer;

};

#endif // LZ4IMAGECOMPRESSOR_H
