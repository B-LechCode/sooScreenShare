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
#include <opencv/cv.hpp>

#include <QKeyEvent>

#include "idraw.h"

class drawWidget:public QOpenGLWidget,public Idraw
{
    Q_OBJECT
    IdrawObserver* m_observer = nullptr;
    cv::Mat m_drawImage;
    bool m_init = false;
    GLuint m_oldid=0;
    bool m_max = false;
    QCursor m_cursor;

public:

    drawWidget(QWidget *parent = nullptr,Qt::WindowFlags f=Qt::WindowFlags());
    virtual ~drawWidget();

    inline virtual void display(const cv::Mat& img)
    {
       m_drawImage = img;
       update();
    }

    void keyPressEvent(QKeyEvent* event);

    virtual IdrawObserver *observer() const
    {
        return m_observer;
    }
    virtual void setObserver(IdrawObserver *observer)
    {
        m_observer = observer;
    }

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void paintGL();
};


#endif //DRAWWIDGET_H
