#ifndef OPENCVJPEGIMAGECOMPRESSOR_H
#define OPENCVJPEGIMAGECOMPRESSOR_H
#include "iimagecompressor.h"

#include <utility>



class opencvJpegImageCompressor : public IImageCompressor
{
public:
    opencvJpegImageCompressor();
    virtual ~opencvJpegImageCompressor();

    virtual std::vector<uint8_t> compress(cv::Mat& img, bool& ok)
    {
        compressHelper(img,ok);
        return std::move(m_buffer);
    }

    virtual std::vector<uint8_t> compress(cv::Mat& img)
    {
        bool dummyOk;
        compressHelper(img,dummyOk);
        return std::move(m_buffer);
    }
    inline void compressHelper(cv::Mat& img, bool& ok)
    {
        ok = cv::imencode(".jpg",img,m_buffer,m_compressionParams);
    }
private:
    std::vector<uint8_t> m_buffer;
    std::vector<int> m_compressionParams;
};

#endif // OPENCVJPEGIMAGECOMPRESSOR_H
