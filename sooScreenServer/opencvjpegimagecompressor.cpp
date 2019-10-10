#include "opencvjpegimagecompressor.h"


opencvJpegImageCompressor::opencvJpegImageCompressor():IImageCompressor()
{
    m_compressionParams.push_back(cv::IMWRITE_JPEG_QUALITY);
    m_compressionParams.push_back(95);
    m_defaultParameters[QUALITY] = parameter("The jpeg compression quality","int","95");
    setParameters(m_defaultParameters);

}

opencvJpegImageCompressor::~opencvJpegImageCompressor()
{

}

void opencvJpegImageCompressor::setParameters(parameterMap &para)
{
    IImageCompressor::setParameters(para);

    //generate openCv parameters
    m_compressionParams.clear();
    m_compressionParams.push_back(cv::IMWRITE_JPEG_QUALITY);
    m_compressionParams.push_back(std::stoi(m_parameters[QUALITY].value()));
}
