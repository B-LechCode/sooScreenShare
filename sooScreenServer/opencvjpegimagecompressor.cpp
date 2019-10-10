#include "opencvjpegimagecompressor.h"


opencvJpegImageCompressor::opencvJpegImageCompressor():IImageCompressor()
{
    m_compressionParams.push_back(cv::IMWRITE_JPEG_QUALITY);
    m_compressionParams.push_back(95);
}

opencvJpegImageCompressor::~opencvJpegImageCompressor()
{

}
