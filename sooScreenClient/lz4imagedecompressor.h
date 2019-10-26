//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef LZ4IMAGEDECOMPRESSOR_H
#define LZ4IMAGEDECOMPRESSOR_H
#include "iimagedecompressor.h"

#include <utility>
#include <lz4.h>
namespace comp
{
    #define QUALITY "JPEG_QUALITY"
}

/**
 * @brief The LZ4 image decompressor
 *
 */
class lz4ImageDecompressor : public IImageDecompressor
{
public:
    /**
     * @brief The standard constructor
     *
     */
    lz4ImageDecompressor();
    /**
     * @brief The destructor
     *
     */
    virtual ~lz4ImageDecompressor();


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
        img = cv::Mat (hdr.height,hdr.width,hdr.cvType);
        int dst_size = img.rows*img.cols*img.channels();
        int decSize = LZ4_decompress_safe(reinterpret_cast<char*>(ptrDat),reinterpret_cast<char*>(img.data),hdr.length,dst_size);
        return decSize==dst_size;
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

#endif // LZ4IMAGEDECOMPRESSOR_H
