#ifndef OPENCVJPEGIMAGECOMPRESSOR_H
#define OPENCVJPEGIMAGECOMPRESSOR_H
#include "iimagecompressor.h"

#include <utility>

namespace comp
{
    #define QUALITY "JPEG_QUALITY"
}

class opencvJpegImageCompressor : public IImageCompressor
{
public:
    opencvJpegImageCompressor();
    virtual ~opencvJpegImageCompressor();

    virtual void setParameters(parameterMap& para);

    inline virtual std::vector<uint8_t> compress(cv::Mat& img, bool& ok)
    {
        ok = compressHelper(img);
        return std::move(m_buffer);
    }

    inline virtual std::vector<uint8_t> compress(cv::Mat& img)
    {
        compressHelper(img);
        return std::move(m_buffer);
    }
    inline bool compressHelper(cv::Mat& img)
    {
        return  cv::imencode(".jpg",img,m_buffer,m_compressionParams);
    }
private:
    std::vector<uint8_t> m_buffer;
    std::vector<int> m_compressionParams;
};

#endif // OPENCVJPEGIMAGECOMPRESSOR_H
