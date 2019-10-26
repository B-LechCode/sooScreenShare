//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IIMAGEDECOMPRESSOR_H
#define IIMAGEDECOMPRESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "./../parameter.h"
#include "./../header.h"
#include "./../iparameter.h"


/**
 * @brief Generic interaface for image decompression backends
 * This interface describes the interaction between the main worker and the decompression backend.
 */
class IImageDecompressor : public Iparameter
{
public:
    /**
    * @brief The standard constructor
    *
    */
    IImageDecompressor();
    /**
    * @brief The destructor
    *
    */
    virtual ~IImageDecompressor();
    /**
     * @brief The method for image decomression.
     *
     * @param ptrDat Pointer to the raw data
     * @param hdr    Header of the received data
     * @param ok     Return reference for decompression status (True if ok)
     * @return cv::Mat The decompressed image
     */
    virtual cv::Mat decompress(uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr, bool& ok) = 0;
    /**
     * @brief The method for image decomression.
     *
     * @param ptrDat Pointer to the raw data
     * @param hdr    Header of the received data
     * @return cv::Mat The decompressed image
     */
    virtual cv::Mat decompress(uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr) = 0;

};

#endif // IIMAGEDECOMPRESSOR_H
