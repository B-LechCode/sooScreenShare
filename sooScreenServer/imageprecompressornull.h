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
     * @brief The method for image comression.
     *
     * @param img The raw image to compress
     * @param ok The compression status (true if ok)
     * @return std::vector<uint8_t> The compressed data
     */
    virtual cv::Mat compress(cv::Mat& img, bool& ok)
    {
        ok = true;
        return  img;
    }
    /**
     * @brief The method for image comression.
     *
     * @param img The raw image to compress
     * @return std::vector<uint8_t> The compressed data
     */
    virtual cv::Mat compress(cv::Mat& img)
    {
        bool ok;
        return compress(img, ok);
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
