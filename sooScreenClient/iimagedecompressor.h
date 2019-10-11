#ifndef IIMAGECOMPRESSOR_H
#define IIMAGECOMPRESSOR_H

#include <opencv/cv.hpp>
#include <vector>

#include "parameter.h"

class IImageDecompressor
{
public:
    IImageDecompressor();
    virtual ~IImageDecompressor();
    virtual parameterMap getDefaultParameters();
    virtual parameterMap getParameters();
    virtual void setParameters(parameterMap& para);

    virtual cv::Mat decompress(std::vector<uint8_t>& dat, bool& ok) = 0;
    virtual cv::Mat decompress(std::vector<uint8_t>& dat) = 0;
protected:
    parameterMap m_defaultParameters;
    parameterMap m_parameters;
};

#endif // IIMAGECOMPRESSOR_H
