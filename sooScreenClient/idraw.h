#ifndef IDRAW_H
#define IDRAW_H

#include "opencv2/opencv.hpp"

class Idraw
{
public:
    Idraw();
    virtual ~Idraw();
    virtual void display(const cv::Mat& img)=0;
};

#endif // IDRAW_H
