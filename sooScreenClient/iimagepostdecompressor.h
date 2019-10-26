//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IIMAGEPOSTDECOMPRESSOR_H
#define IIMAGEPOSTDECOMPRESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "./../parameter.h"
#include "./../header.h"
#include "./../iparameter.h"


/**
 * @brief Generic interaface for image post decompression backends
 * This interface describes the interaction between the main worker and the post decompression backend.
 */
class IImagePostDecompressor : public Iparameter
{
public:
    /**
    * @brief The standard constructor
    *
    */
    IImagePostDecompressor();
    /**
    * @brief The destructor
    *
    */
    virtual ~IImagePostDecompressor();
    /**
     * @brief The method for image decomression.
     *
     * @param data  The Data
     * @param hdr    Header of the received data
     * @param ok     Return reference for decompression status (True if ok)
     * @return cv::Mat The decompressed image
     */
    virtual cv::Mat decompress(cv::Mat& data,dataHeaderHandling::dataHeader hdr, bool& ok) = 0;
    /**
     * @brief The method for image decomression.
     *
     * @param data  The Data
     * @param hdr    Header of the received data
     * @return cv::Mat The decompressed image
     */
    virtual cv::Mat decompress(cv::Mat& data,dataHeaderHandling::dataHeader hdr) = 0;

};

#endif // IIMAGEPOSTDECOMPRESSOR_H
