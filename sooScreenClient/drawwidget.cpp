//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "drawwidget.h"

drawWindow::drawWindow(QWindow *parent):
    QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate,parent)
{
    setParameters(m_defaultParameters);
    resize(640,480);
    setSurfaceType(QWindow::OpenGLSurface);
    QOpenGLWindow::show();
}

drawWindow::~drawWindow()
{

}

void drawWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key::Key_Space)
    {
        if(m_max)
        {
            this->showNormal();
            this->setCursor(m_cursor);
            m_max = false;
        }
        else
        {
            this->showFullScreen();            
            m_cursor = this->cursor();
            this->setCursor(Qt::BlankCursor);
            m_max = true;
        }
    }
    else        
        QWindow::keyPressEvent(event);
}

bool drawWindow::event(QEvent *event)
{

    switch (event->type()) {
        case QEvent::Close :
        if(m_observer)
            m_observer->drawWidgetClosing();
        break;

        //Nothing else matters!
        default:
            ;
        break;
        }
    return QOpenGLWindow::event(event);
}


void drawWindow::calcAspects()
{
    if(aspWid>aspImg) //"compress" x
    {
         xFac = aspImg/aspWid;
         yFac = 1;
    }
    else if(aspWid<aspImg)//"compress" y
    {
         xFac = 1;
         yFac = aspWid/aspImg;
    }
    else
    {
         xFac = 1;
         yFac = 1;
    }
}

void drawWindow::paintGL()
{
    if(m_drawImage.empty())
       return;

    int imgH = m_drawImage.rows;
    int imgW = m_drawImage.cols;
    int imgT = m_drawImage.type();    
	
    glEnable(GL_TEXTURE_2D);

	bool dimChange = m_lastImageHeight != imgH || m_lastImageWidth != imgW || m_lastImageType != imgT;

	//Clearing when the dimension changes and for the next draw
	if (m_doubleClear || dimChange)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		m_doubleClear = false;
	}
	
    if(dimChange)
    {
		m_doubleClear = true;		
        m_lastImageHeight = imgH;
        m_lastImageWidth  = imgW;
        m_lastImageType   = imgT;

        if (m_lastImageType == CV_8UC4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_lastImageWidth , m_lastImageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_drawImage.data);
        else if(m_lastImageType == CV_8UC3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_lastImageWidth , m_lastImageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, m_drawImage.data);
        aspImg = static_cast<float>(imgW)/static_cast<float>(imgH);
        aspWid = static_cast<float>(width())/static_cast<float>(height());
        calcAspects();
    }
    else
    {
        if(m_lastImageType == CV_8UC4)
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0 , 0, m_lastImageWidth,m_lastImageHeight, GL_BGRA, GL_UNSIGNED_BYTE, m_drawImage.data);
        else if(m_lastImageType == CV_8UC3)
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0 , 0, m_lastImageWidth,m_lastImageHeight, GL_BGR, GL_UNSIGNED_BYTE, m_drawImage.data);
    }

    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-1*xFac, -1*yFac, -1);
        glTexCoord2f(1,1); glVertex3f(1*xFac, -1*yFac, -1);
        glTexCoord2f(1,0); glVertex3f(1*xFac, 1*yFac, -1);
        glTexCoord2f(0,0); glVertex3f(-1*xFac, 1*yFac, -1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glFlush();
}

void drawWindow::initializeGL()
{

    QOpenGLWindow::initializeGL();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,00);

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


    if(m_init)
        glDeleteTextures(1,&m_oldid);

    glGenTextures(1, &m_textureId);
    m_oldid = m_textureId;
    m_init = true;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glDisable(GL_TEXTURE_2D);       

    aspWid = static_cast<float>(width())/static_cast<float>(height());
    calcAspects();
}

void drawWindow::resizeGL(int w, int h)
{
    QOpenGLWindow::resizeGL(w,h);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();
	m_doubleClear = true;
    aspWid = static_cast<float>(width())/static_cast<float>(height());
    calcAspects();    
}
