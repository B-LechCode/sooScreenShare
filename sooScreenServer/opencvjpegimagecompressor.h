#ifndef OPENCVJPEGIMAGECOMPRESSOR_H
#define OPENCVJPEGIMAGECOMPRESSOR_H


#include <utility>

#include "iimagecompressor.h"

class opencvJpegImageCompressor : public iImageCompressor
{
public:
    opencvJpegImageCompressor():iImageCompressor(){}
    virtual ~opencvJpegImageCompressor();
    virtual std::vector<uint8_t> compress(cv::Mat& img)
    {
        cv::imencode(".jpg",img,m_buffer,m_compressionParams);
        return std::move(m_buffer);
    }
private:
    std::vector<uint8_t> m_buffer;
    std::vector<int> m_compressionParams;
};

#endif // OPENCVJPEGIMAGECOMPRESSOR_H
