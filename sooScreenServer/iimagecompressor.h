//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IIMAGECOMPRESSOR_H
#define IIMAGECOMPRESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "./../parameter.h"
#include "./../iparameter.h"

class IImageCompressor : public Iparameter
{
public:
    IImageCompressor();
    virtual ~IImageCompressor(); 
    virtual std::vector<uint8_t> compress(cv::Mat& img, bool& ok) = 0;
    virtual std::vector<uint8_t> compress(cv::Mat& img) = 0;
};

#endif // IIMAGECOMPRESSOR_H
