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

    virtual void setParameters(parameterMap& para)
    {
        //insert into local para map
        auto itPara =  para.begin();
        while(itPara != para.end())
        {
            m_parameters[itPara->first] = itPara->second;
            itPara++;
        }

        //generate openCv parameters
        m_compressionParams.clear();
        m_compressionParams.push_back(cv::IMWRITE_JPEG_QUALITY);
        m_compressionParams.push_back(std::stoi(m_parameters[QUALITY].value()));
    }

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
