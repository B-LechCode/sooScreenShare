#ifndef IIMAGECOMPRESSOR_H
#define IIMAGECOMPRESSOR_H

#include <opencv/cv.hpp>
#include <vector>

class iImageCompressor
{
public:
    iImageCompressor();
    virtual ~iImageCompressor();
    virtual std::vector<uint8_t> compress(cv::Mat& img) = 0;
};

#endif // IIMAGECOMPRESSOR_H
