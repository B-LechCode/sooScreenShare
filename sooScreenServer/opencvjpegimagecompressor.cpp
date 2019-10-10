#include "opencvjpegimagecompressor.h"


opencvJpegImageCompressor::opencvJpegImageCompressor():IImageCompressor()
{
    m_compressionParams.push_back(cv::IMWRITE_JPEG_QUALITY);
    m_compressionParams.push_back(95);
    m_defaultParameters[quality] = parameter("The jpeg compression quality","int","95");
    setParameters(m_defaultParameters);

}

opencvJpegImageCompressor::~opencvJpegImageCompressor()
{

}
