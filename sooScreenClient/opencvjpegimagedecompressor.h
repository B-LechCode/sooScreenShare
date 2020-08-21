//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef OPENCVJPEGIMAGEDECOMPRESSOR_H
#define OPENCVJPEGIMAGEDECOMPRESSOR_H
#include "iimagedecompressor.h"

#include <utility>

namespace comp
{

}

/**
 * @brief The jpeg image decompressor
 *
 */
class opencvJpegImageDecompressor : public IImageDecompressor
{
public:
    /**
     * @brief The standard constructor
     *
     */
    opencvJpegImageDecompressor();
    /**
     * @brief The destructor
     *
     */
    virtual ~opencvJpegImageDecompressor();


    /**
     * @brief The method for image decomression.
     *
     * @param ptrDat Pointer to the raw data
     * @param hdr    Header of the received data
     * @param ok     Return reference for decompression status (True if ok)
     * @return cv::Mat The decompressed image
     */
    inline virtual cv::Mat decompress(uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr, bool& ok)
    {
        cv::Mat img;        
        ok = decompressHelper(img,ptrDat,hdr);
        return img;
    }

    /**
    * @brief The method for image decomression.
    *
    * @param ptrDat Pointer to the raw data
    * @param hdr    Header of the received data
    * @return cv::Mat The decompressed image
    */
    inline virtual cv::Mat decompress(uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr)
    {        
        cv::Mat img;
        decompressHelper(img,ptrDat,hdr);
        return img;
    }

private:
    /**
     * @brief Helper class for decompression
     *
     * @param img Image to be written
     * @param ptrDat Pointer to the raw data
     * @param hdr Header of the received data
     * @return bool Decompression status (True if ok)
     */
    inline bool decompressHelper(cv::Mat& img,uint8_t* ptrDat,dataHeaderHandling::dataHeader hdr)
    {
        auto t = cv::Mat (1, hdr.length, CV_8UC1,ptrDat);
        img = cv::imdecode(t,-1);
        return !img.empty();
    }
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

#endif // OPENCVJPEGIMAGEDECOMPRESSOR_H
