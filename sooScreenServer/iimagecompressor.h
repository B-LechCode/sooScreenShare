//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IIMAGECOMPRESSOR_H
#define IIMAGECOMPRESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "./../parameter.h"
#include "./../iparameter.h"

/**
 * @brief Generic interface for image compression backends
 * This interface describes the interaction between the main worker and the compression backend.
 */
class IImageCompressor : public Iparameter
{
public:
    /**
     * @brief The standard constructor
     *
     */
    IImageCompressor();
    /**
     * @brief The destructor
     *
     */
    virtual ~IImageCompressor();
    /**
     * @brief The method for image comression.
     *
     * @param ptrDest Destination data Pointer
     * @param dataSize The size of the destination data field in bytes
     * @param imgIn The raw image to compress
     * @param ok The compression status (true if ok)
     * @return size_t Size of the Data
     */
    virtual size_t compress(uint8_t* ptrDest,size_t dataSize,cv::Mat& imgIn, bool& ok) = 0;
    /**
     * @brief The method for image comression.
     *
     * @param ptrDest Destination data Pointer
     * @param dataSize The size of the destination data field in bytes
     * @param imgIn The raw image to compress
     * @return size_t Size of the Data
     */
    virtual size_t compress(uint8_t* ptrDest,size_t dataSize,cv::Mat& imgIn) = 0;
};

#endif // IIMAGECOMPRESSOR_H
