//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef IDRAW_H
#define IDRAW_H

#include "opencv2/opencv.hpp"



/**
 * @brief The observer interface for the draw widget
 * This interface is currently used to pass the close event to the subscriber.
 */
class IdrawObserver
{
public:
    /**
     * @brief The standard constructor
     *
     */
    IdrawObserver();
    /**
     * @brief The destructor
     *
     */
    virtual ~IdrawObserver();
    /**
     * @brief The close event
     * This method is called when the draw widget is closing.
     */
    virtual void drawWidgetClosing() = 0;
};


/**
 * @brief Generic interaface for draw backends
 * This interface describes the interaction between the main worker and the draw backend.
 */
class Idraw
{
public:
    /**
     * @brief The standard constructor
     *
     */
    Idraw();
    /**
     * @brief The destructor
     *
     */
    virtual ~Idraw();
    /**
     * @brief Sets a new draw image and initiates drawing
     *
     * @param img The image to draw
     */
    virtual void display(const cv::Mat& img)=0;
    /**
     * @brief Getter method for the current observer
     *
     * @return IdrawObserver The current observer
     */
    virtual IdrawObserver *observer() const = 0;
    /**
     * @brief Setter method for the current observer
     *
     * @param observer The observer to be set
     */
    virtual void setObserver(IdrawObserver *observer) = 0;
};

#endif // IDRAW_H
