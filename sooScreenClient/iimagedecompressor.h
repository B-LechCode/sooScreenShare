//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IIMAGECOMPRESSOR_H
#define IIMAGECOMPRESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "./../parameter.h"
#include "./../header.h"
#include "./../iparameter.h"

class IImageDecompressor : public Iparameter
{
public:
    IImageDecompressor();
    virtual ~IImageDecompressor();
    virtual cv::Mat decompress(uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr, bool& ok) = 0;
    virtual cv::Mat decompress(uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr) = 0;

};

#endif // IIMAGECOMPRESSOR_H
