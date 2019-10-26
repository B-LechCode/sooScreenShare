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
 * @brief Generic interaface for image compression backends
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
     * @param img The raw image to compress
     * @param ok The compression status (true if ok)
     * @return std::vector<uint8_t> The compressed data
     */
    virtual std::vector<uint8_t> compress(cv::Mat& img, bool& ok) = 0;
    /**
     * @brief The method for image comression.
     *
     * @param img The raw image to compress
     * @return std::vector<uint8_t> The compressed data
     */
    virtual std::vector<uint8_t> compress(cv::Mat& img) = 0;
};

#endif // IIMAGECOMPRESSOR_H
