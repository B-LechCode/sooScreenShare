//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef NULLIMAGECOMPRESSOR_H
#define NULLIMAGECOMPRESSOR_H
#include "iimagecompressor.h"
#include <utility>

namespace comp
{

}

/**
 * @brief The null image compressor
 *
 */
class nullImageCompressor : public IImageCompressor
{
public:
    /**
     * @brief The standard constructor
     *
     */
    nullImageCompressor();
    /**
     * @brief The destructor
     *
     */
    virtual ~nullImageCompressor();

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
        size_t matSize = imgIn.total() * imgIn.elemSize();
        if(matSize<=dataSize)
        {
            memcpy(ptrDest,imgIn.ptr(),matSize);
            ok = true;
            return matSize;
        }

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

};

#endif // NULLIMAGECOMPRESSOR_H
