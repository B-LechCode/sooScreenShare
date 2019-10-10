#ifndef IIMAGECOMPRESSOR_H
#define IIMAGECOMPRESSOR_H

#include <opencv/cv.hpp>
#include <vector>

class IImageCompressor
{
public:
    IImageCompressor();
    virtual ~IImageCompressor();
    virtual std::vector<uint8_t> compress(cv::Mat& img, bool& ok) = 0;
    virtual std::vector<uint8_t> compress(cv::Mat& img) = 0;
};

#endif // IIMAGECOMPRESSOR_H
