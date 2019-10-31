//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IMAGEPOSTDECOMPRESSORNULL_H
#define IMAGEPOSTDECOMPRESSORNULL_H
#include "iimagepostdecompressor.h"

class ImagePostDecompressorNull : public IImagePostDecompressor
{
public:
    ImagePostDecompressorNull();
    virtual ~ImagePostDecompressorNull();
    /**
     * @brief The method for image decomression.
     *
     * @param data  The Data
     * @param hdr    Header of the received data
     * @param ok     Return reference for decompression status (True if ok)
     * @return cv::Mat The decompressed image
     */
    virtual cv::Mat decompress(cv::Mat& data,dataHeaderHandling::dataHeader hdr, bool& ok)
    {
        ok = true;
        return decompress(data,hdr);
    }
    /**
     * @brief The method for image decomression.
     *
     * @param data  The Data
     * @param hdr    Header of the received data
     * @return cv::Mat The decompressed image
     */
    virtual cv::Mat decompress(cv::Mat& data,dataHeaderHandling::dataHeader hdr)
    {
        return  data;
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


#endif // IMAGEPOSTDECOMPRESSORNULL_H
