//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef ISCREENSHOT_H
#define ISCREENSHOT_H
#include "./../iparameter.h"
#include <opencv2/opencv.hpp>
#include <vector>

/**
 * @brief The structure of a single screen
 *
 */
struct screenDef{

    /**
     * @brief The value constructor
     *
     * @param x The x offset
     * @param y The y offset
     * @param w The width
     * @param h The height
     */
    screenDef(int32_t x,int32_t y, uint32_t w,uint32_t h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    int32_t x; /**< Offset coordinates */
    int32_t y; /**< Offset coordinates */
    uint32_t w; /**< The screen size */
    uint32_t h; /**< The screen size */
};

/**
 * @brief Generic interface for screenshot backends
 * This interface describes the interaction between the main worker and the screenshot backend.
 */
class IscreenShot : public Iparameter{
public:
    /**
     * @brief The standard constructor
     *
     */
    IscreenShot();
    /**
     * @brief The destructor
     *
     */
    virtual ~IscreenShot();
    /**
     * @brief The initializer method
     *
     */
    virtual void initialize()=0;
    /**
     * @brief The currently active screens
     *
     * @return std::vector<screenDef> The screens
     */
    virtual std::vector<screenDef> getScreens()=0;
    /**
     * @brief The grab operator
     * This operator grabs the screen with mouse pointer
     * @return cv::Mat The grabbed image
     */
    virtual cv::Mat operator()() = 0;
protected:
        int32_t m_x; /**< Offset x */
        int32_t m_y; /**< Offset y */
        uint32_t m_w; /**< Width */
        uint32_t m_h; /**< Height */
};

#endif // ISCREENSHOT_H
