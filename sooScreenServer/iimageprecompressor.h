//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IIMAGEPRECOMPRESSOR_H
#define IIMAGEPRECOMPRESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "./../header.h"
#include "./../parameter.h"
#include "./../iparameter.h"
/**
 * @brief Generic interface for image precompression backends
 * This interface describes the interaction between the main worker and the precompression backend.
 */
class IImagePreCompressor : public Iparameter
{
public:
    /**
     * @brief The standard constructor
     *
     */
    IImagePreCompressor();
    /**
     * @brief The destructor
     *
     */
    virtual ~IImagePreCompressor();
    /**
     * @brief The method for image comression.
     *
     * @param img The raw image to compress
     * @param ok The compression status (true if ok)
     * @return std::vector<uint8_t> The compressed data
     */
    virtual cv::Mat compress(cv::Mat& img,imageType& type ,bool& ok) = 0;
    /**
     * @brief The method for image comression.
     *
     * @param img The raw image to compress
     * @return std::vector<uint8_t> The compressed data
     */
    virtual cv::Mat compress(cv::Mat& img,imageType& type) = 0;
};

#endif // IIMAGEPRECOMPRESSOR_H
