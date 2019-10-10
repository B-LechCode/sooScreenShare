#ifndef IIMAGECOMPRESSOR_H
#define IIMAGECOMPRESSOR_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "parameter.h"

class IImageCompressor
{
public:
    IImageCompressor();
    virtual ~IImageCompressor();
    virtual parameterMap getDefaultParameters();
    virtual parameterMap getParameters();
    virtual void setParameters(parameterMap& para);
    virtual std::vector<uint8_t> compress(cv::Mat& img, bool& ok) = 0;
    virtual std::vector<uint8_t> compress(cv::Mat& img) = 0;
protected:
    parameterMap m_defaultParameters;
    parameterMap m_parameters;
};

#endif // IIMAGECOMPRESSOR_H
