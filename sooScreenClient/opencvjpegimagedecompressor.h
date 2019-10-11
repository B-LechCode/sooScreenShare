#ifndef OPENCVJPEGIMAGECOMPRESSOR_H
#define OPENCVJPEGIMAGECOMPRESSOR_H
#include "iimagedecompressor.h"

#include <utility>

namespace comp
{
    #define QUALITY "JPEG_QUALITY"
}

class opencvJpegImageDecompressor : public IImageDecompressor
{
public:
    opencvJpegImageDecompressor();
    virtual ~opencvJpegImageDecompressor();

    virtual void setParameters(parameterMap& para);

    inline virtual cv::Mat decompress(cv::Mat& dat, bool& ok)
    {
        cv::Mat img;
        decompressHelper(img,dat);
        ok = true; //TODO: check for ok...
        return img;
    }

    inline virtual cv::Mat decompress(cv::Mat& dat)
    {
        cv::Mat img;
        decompressHelper(img,dat);
        return img;
    }
    inline void decompressHelper(cv::Mat& img,cv::Mat& dat)
    {
        img = cv::imdecode(cv::Mat(dat),-1);
    }
private:

};

#endif // OPENCVJPEGIMAGECOMPRESSOR_H
