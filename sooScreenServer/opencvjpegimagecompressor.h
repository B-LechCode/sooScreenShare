//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef OPENCVJPEGIMAGECOMPRESSOR_H
#define OPENCVJPEGIMAGECOMPRESSOR_H
#include "iimagecompressor.h"
#include <utility>

namespace comp
{
    #define QUALITY "JPEG_QUALITY"
}

/**
 * @brief The jpeg image compressor
 *
 */
class opencvJpegImageCompressor : public IImageCompressor
{
public:
    /**
     * @brief The standard constructor
     *
     */
    opencvJpegImageCompressor();
    /**
     * @brief The destructor
     *
     */
    virtual ~opencvJpegImageCompressor();

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
        cv::imencode(".jpg",imgIn,m_buffer,m_compressionParams);
        if(dataSize>=m_buffer.size())
        {
            memcpy(ptrDest,m_buffer.data(),m_buffer.size());
            ok = m_buffer.size() > 0;
            return m_buffer.size();
        }
        //on error:
        ok = false;
        return 0;
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

private:    
    /**
     * @brief Translates the parameter values to the opencv usable format
     *
     */
    void initParameters();
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
    std::vector<uint8_t> m_buffer; /**< Compressed image buffer */
    std::vector<int> m_compressionParams; /**< The compression parameters */
};

#endif // OPENCVJPEGIMAGECOMPRESSOR_H
