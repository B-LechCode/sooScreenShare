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

class drawWidget:public QOpenGLWidget,public Idraw
{
    Q_OBJECT
    cv::Mat m_drawImage;
    bool m_init = false;
    GLuint m_oldid=0;
    bool m_max = false;

public:

    drawWidget(QWidget *parent = nullptr,Qt::WindowFlags f=Qt::WindowFlags());
    //virtual ~drawWidget();

    inline virtual void display(const cv::Mat& img)
    {
        m_drawImage = img;
       update();
    }

    void keyPressEvent(QKeyEvent* event);

protected:

    virtual void paintGL();
};


#endif //DRAWWIDGET_H
