//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IMAGEPRECOMPRESSORNULL_H
#define IMAGEPRECOMPRESSORNULL_H

#include "iimageprecompressor.h"
/**
 * @brief The null image precompressor
 *
 */
class ImagePrecompressorNull : public IImagePreCompressor
{
public:
    /**
     * @brief The standard constructor
     *
     */
    ImagePrecompressorNull();
    /**
     * @brief The destructor
     *
     */
    virtual ~ImagePrecompressorNull();
    /**
     * @brief The method for image precomression.
     *
     * @param img The raw image to compress
     * @param ok The compression status (true if ok)
     * @return cv::Mat The compressed data
     */
    virtual cv::Mat compress(cv::Mat& img,imageType& type, bool& ok)
    {
        ok = true;
        type = imageType::keyFrame;
        return  img;
    }
    /**
     * @brief The method for image comression.
     *
     * @param img The raw image to compress
     * @return cv::Mat The compressed data
     */
    virtual cv::Mat compress(cv::Mat& img,imageType& type)
    {
        bool ok;
        return compress(img,type,ok);
    }
private:
    /**
     * @brief The changed event of the underlying parameter map
     *
     */
    virtual void parameterMapChangedEvent();
    /**
     * @brief The changed event of a key/value pair
     *
     * @param key The key of the changed parameter
     */
    virtual void parameterChangedEvent(const std::string& key);
};

#endif // IMAGEPRECOMPRESSORNULL_H
