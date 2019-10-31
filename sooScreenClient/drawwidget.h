//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

//#include <QOpenGLWidget>
//#include <QGLWidget>
#include <QOpenGLWindow>

#include <QOpenGLFunctions>

#include <GL/glu.h>
#include <GL/gl.h>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPixmap>
#include <QElapsedTimer>
#include <iostream>
#include <opencv2/opencv.hpp>

#include <QKeyEvent>

#include "idraw.h"

/**
 * @brief The widget to draw the image with OpenGl support.
 *
 */
class drawWindow:public QOpenGLWindow,public Idraw
{
    Q_OBJECT
    IdrawObserver* m_observer = nullptr; /**< The pointer to the current set observer */
    cv::Mat m_drawImage; /**< The last set draw image */
    bool m_init = false; /**< Flag for the init state (Is true after the first paintGL run) */
    GLuint m_oldid=0; /**< Stores the pointer to the last drawn texture for deletion in next draw attempt */
    bool m_max = false; /**< Flag indicating the maximized state */
    QCursor m_cursor; /**< The previously set cursor */

public:

    /**
     * @brief Standard constructor
     *
     * @param parent The parent widget
     * @param f The window flags
     */
    drawWindow(QWindow *parent = nullptr);
    /**
     * @brief The destructor
     *
     */
    virtual ~drawWindow() override;

    /**
     * @brief Sets a new draw image and initiates drawing
     *
     * @param img The image to draw
     */
    inline virtual void displayImage(const cv::Mat& img) override
    {        
       m_drawImage = img;
       update();
    }

    /**
     * @brief Reimplemented key press event
     *
     * The event is used to trigger entering/leaving the fullscreen mode by pressing the space bar.
     *
     * @param event The key event
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Getter method for the current observer
     *
     * @return IdrawObserver The current observer
     */
    virtual IdrawObserver *observer() const override
    {
        return m_observer;
    }
    /**
     * @brief Setter method for the current observer
     *
     * @param observer The observer to be set
     */
    virtual void setObserver(IdrawObserver *observer) override
    {
        m_observer = observer;
    }

protected:
    /**
     * @brief The close event for this Widget
     *
     * This event is used to close the main window after our own closing
     *
     * @param event The close event
     */    
    virtual bool event(QEvent *event) override;

    /**
     * @brief The redefined paintGL method.
     *
     * Paints the draw image on the widget with OpenGl support.
     */
    virtual void paintGL() override;




};


#endif //DRAWWIDGET_H
