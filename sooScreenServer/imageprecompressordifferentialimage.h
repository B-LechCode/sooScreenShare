//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IMAGEPRECOMPRESSORDIFFERENTIALIMAGE_H
#define IMAGEPRECOMPRESSORDIFFERENTIALIMAGE_H
#include "iimageprecompressor.h"

class ImagePreCompressorDifferentialImage: public IImagePreCompressor
{
public:
    ImagePreCompressorDifferentialImage();
    virtual ~ImagePreCompressorDifferentialImage();
    /**
     * @brief The method for image precomression.
     *
     * @param img The raw image to compress
     * @param ok The compression status (true if ok)
     * @return cv::Mat The compressed data
     */
    virtual cv::Mat compress(cv::Mat& img,imageType& type, bool& ok);
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
    bool m_FirstFrame;
    int m_numDifferentialFrames;
    int m_maxNumOfDifferentialFrames;
    cv::Mat m_keyImage;
    cv::Mat m_diffImage;
};

#endif // IMAGEPRECOMPRESSORDIFFERENTIALIMAGE_H
