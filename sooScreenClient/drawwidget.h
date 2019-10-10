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
#define USEGL 1



#if USEGL
class drawWidget:public QOpenGLWidget
{
    Q_OBJECT
    cv::Mat m_drawImage;

public:

    drawWidget(QWidget *parent = nullptr,Qt::WindowFlags f=Qt::WindowFlags());
    //virtual ~drawWidget();

    inline void display(const cv::Mat& img)
    {
        m_drawImage = img;
        update();
    }

protected:

    virtual void paintGL();
};
#else
class drawWidget:public QWidget //QOpenGLWidget
{
    Q_OBJECT
    QPixmap m_drawPixmap;
    cv::Mat m_drawImage;
    inline QImage cvMatToQImage(const cv::Mat &inMat)
    {
        switch (inMat.type())
        {
            // 8-bit, 4 channel
        case CV_8UC4:
        {

            //cv::cvtColor(inMat,im,cv::COLOR_RGBA2RGB);
            QImage image(inMat.data,
                inMat.cols, inMat.rows,
                static_cast<int>(inMat.step),
                QImage::Format_RGB32);

            return image;
        }

        // 8-bit, 3 channel
        case CV_8UC3:
        {
            QElapsedTimer tmr;
            tmr.start();
                QImage image(inMat.data,
                inMat.cols, inMat.rows,
                static_cast<int>(inMat.step),
                QImage::Format_RGB888);
            std::cout << " convert slow soos " << tmr.elapsed() << std::endl;
            return image;
        }
        }
        return QImage();
    }
public:

    drawWidget(QWidget *parent = nullptr,Qt::WindowFlags f=Qt::WindowFlags());
    virtual ~drawWidget();

    inline void display(const cv::Mat &inMat)
    {
        /*if(m_drawImage)
            delete m_drawImage;*/

        m_drawPixmap.convertFromImage(cvMatToQImage(inMat));
        update();
    }

    inline void paintEvent(QPaintEvent* ev)
    {
        QElapsedTimer tmr;
        tmr.start();
        QPainter painter(this);

        painter.drawPixmap(this->rect(),m_drawPixmap);
        std::cout <<" draw time paint event " << tmr.elapsed() << std::endl;
    }


};
#endif

#endif //DRAWWIDGET_H
