//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QOpenGLWidget>

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
 * @brief
 *
 */
class drawWidget:public QOpenGLWidget,public Idraw
{
    Q_OBJECT
    IdrawObserver* m_observer = nullptr; /**< TODO: describe */
    cv::Mat m_drawImage; /**< TODO: describe */
    bool m_init = false; /**< TODO: describe */
    GLuint m_oldid=0; /**< TODO: describe */
    bool m_max = false; /**< TODO: describe */
    QCursor m_cursor; /**< TODO: describe */

public:

    /**
     * @brief
     *
     * @param parent
     * @param f
     */
    drawWidget(QWidget *parent = nullptr,Qt::WindowFlags f=Qt::WindowFlags());
    /**
     * @brief
     *
     */
    virtual ~drawWidget();

    /**
     * @brief
     *
     * @param img
     */
    inline virtual void display(const cv::Mat& img)
    {
       m_drawImage = img;
       update();
    }

    /**
     * @brief
     *
     * @param event
     */
    void keyPressEvent(QKeyEvent* event);

    /**
     * @brief
     *
     * @return IdrawObserver
     */
    virtual IdrawObserver *observer() const
    {
        return m_observer;
    }
    /**
     * @brief
     *
     * @param observer
     */
    virtual void setObserver(IdrawObserver *observer)
    {
        m_observer = observer;
    }

protected:
    /**
     * @brief
     *
     * @param event
     */
    virtual void closeEvent(QCloseEvent *event);
    /**
     * @brief The redefined paintGL method.
     *
     * Paints the draw image on the widget
     */
    virtual void paintGL();
};


#endif //DRAWWIDGET_H
