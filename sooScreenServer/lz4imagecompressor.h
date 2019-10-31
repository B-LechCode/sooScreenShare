//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef LZ4IMAGECOMPRESSOR_H
#define LZ4IMAGECOMPRESSOR_H
#include "iimagecompressor.h"

#include <utility>
#include <lz4.h>


/**
 * @brief The LZ4 image compressor
 *
 */
class lz4ImageCompressor : public IImageCompressor
{
public:
    /**
     * @brief The standard constructor
     *
     */
    lz4ImageCompressor();
    /**
     * @brief The destructor
     *
     */
    virtual ~lz4ImageCompressor();

    /**
     * @brief The method for image comression.
     *
     * @param ptrDest Destination data Pointer
     * @param dataSize The size of the destination data field in bytes
     * @param imgIn The raw image to compress
     * @param ok The compression status (true if ok)
     * @return size_t Size of the Data
     */
    inline virtual size_t compress(uint8_t* ptrDest,size_t dataSize,cv::Mat& imgIn, bool& ok)
    {
        int src_size = imgIn.rows*imgIn.cols*imgIn.channels();
        int compSize = LZ4_compress_default(reinterpret_cast<char*>(imgIn.ptr()),reinterpret_cast<char*>(ptrDest),src_size,static_cast<int>(dataSize));
        ok = compSize>0;
        return  static_cast<size_t>(compSize);

    }

    /**
     * @brief The method for image comression.
     *
     * @param ptrDest Destination data Pointer
     * @param dataSize The size of the destination data field in bytes
     * @param imgIn The raw image to compress
     * @return size_t Size of the Data
     */
    inline virtual size_t compress(uint8_t* ptrDest,size_t dataSize,cv::Mat& imgIn)
    {
       bool ok;
       return compress(ptrDest,dataSize,imgIn, ok);
    }

    /**
     * @brief The method for image comression.
     *
     * @param img The raw image to compress
     * @param ok The compression status (true if ok)
     * @return std::vector<uint8_t> The compressed data
     */
//    inline virtual std::vector<uint8_t> compress(cv::Mat& img, bool& ok)
//    {
//        ok = compressHelper(img);
//        return std::move(m_buffer);
//    }

    /**
     * @brief The method for image comression.
     *
     * @param img The raw image to compress
     * @return std::vector<uint8_t> The compressed data
     */
//    inline virtual std::vector<uint8_t> compress(cv::Mat& img)
//    {
//        compressHelper(img);
//        return std::move(m_buffer);
//    }
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

#endif // LZ4IMAGECOMPRESSOR_H
