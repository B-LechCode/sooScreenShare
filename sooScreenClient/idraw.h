//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IDRAW_H
#define IDRAW_H

#include "opencv2/opencv.hpp"



class IdrawObserver
{
public:
    IdrawObserver();
    virtual ~IdrawObserver();
    virtual void drawWidgetClosing() = 0;
};


class Idraw
{
public:
    Idraw();
    virtual ~Idraw();
    virtual void display(const cv::Mat& img)=0;
    virtual IdrawObserver *observer() const = 0;
    virtual void setObserver(IdrawObserver *observer) = 0;
};

#endif // IDRAW_H
