//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef OPENCVJPEGIMAGECOMPRESSOR_H
#define OPENCVJPEGIMAGECOMPRESSOR_H
#include "iimagedecompressor.h"

#include <utility>

namespace comp
{
    #define QUALITY "JPEG_QUALITY"
}

class opencvJpegImageDecompressor : public IImageDecompressor
{
public:
    opencvJpegImageDecompressor();
    virtual ~opencvJpegImageDecompressor();


    inline virtual cv::Mat decompress(uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr, bool& ok)
    {
        cv::Mat img;        
        ok = decompressHelper(img,ptrDat,hdr);
        return img;
    }

    inline virtual cv::Mat decompress(uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr)
    {        
        cv::Mat img;
        decompressHelper(img,ptrDat,hdr);
        return img;
    }
    inline bool decompressHelper(cv::Mat& img,uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr)
    {
        auto t = cv::Mat (1, hdr.length, CV_8UC1,ptrDat);
        img = cv::imdecode(t,-1);
        return !img.empty();
    }
private:
    virtual void parameterMapChangedEvent();
    virtual void parameterChangedEvent(const std::string& key);
};

#endif // OPENCVJPEGIMAGECOMPRESSOR_H
