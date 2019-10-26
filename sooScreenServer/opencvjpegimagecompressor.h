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
     * @param img The raw image to compress
     * @param ok The compression status (true if ok)
     * @return std::vector<uint8_t> The compressed data
     */
    inline virtual std::vector<uint8_t> compress(cv::Mat& img, bool& ok)
    {
        ok = compressHelper(img);
        return std::move(m_buffer);
    }

    /**
     * @brief The method for image comression.
     *
     * @param img The raw image to compress
     * @return std::vector<uint8_t> The compressed data
     */
    inline virtual std::vector<uint8_t> compress(cv::Mat& img)
    {
        compressHelper(img);
        return std::move(m_buffer);
    }

private:
    /**
     * @brief Helper class for compression
     *
     * @param img Image to compress
     * @return bool Compression status (True if ok)
     */
    inline bool compressHelper(cv::Mat& img)
    {
        return  cv::imencode(".jpg",img,m_buffer,m_compressionParams);
    }
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
