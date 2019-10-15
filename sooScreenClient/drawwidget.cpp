#include "drawwidget.h"



drawWidget::drawWidget(QWidget *parent ,Qt::WindowFlags f):
   QOpenGLWidget(parent,f)
{
    setMinimumSize(640,480);
}

void drawWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key::Key_Space)
    {
        if(m_max)
        {
            this->showNormal();
            m_cursor = this->cursor();
            this->setCursor(Qt::BlankCursor);
            m_max = false;
        }
        else
        {
            this->showFullScreen();
            this->setCursor(m_cursor);
            m_max = true;
        }
    }
    else
        QWidget::keyPressEvent(event);
}



void drawWidget::paintGL()
{
    glClearColor(0,0,0,00);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if(m_drawImage.empty())
       return;

    float aspWid = static_cast<float>(width())/static_cast<float>(height());
    float aspImg = static_cast<float>(m_drawImage.cols)/static_cast<float>(m_drawImage.rows);

    float xFac = 1;
    float yFac = 1;

    if(aspWid>aspImg) //"compress" x
    {
         xFac = aspImg/aspWid;
    }
    else if(aspWid<aspImg)//"compress" y
    {
         yFac = aspWid/aspImg;
    }


    glEnable(GL_TEXTURE_2D);
    GLuint texture_id;

    if(!m_init)
        glDeleteTextures(1,&m_oldid);

    glGenTextures(1, &texture_id);
    m_oldid = texture_id;
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    if(m_drawImage.type() == CV_8UC3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_drawImage.cols , m_drawImage.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, m_drawImage.data);
    else if (m_drawImage.type() == CV_8UC4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_drawImage.cols , m_drawImage.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_drawImage.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



    glBindTexture(GL_TEXTURE_2D, texture_id);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-1*xFac, -1*yFac, -1);
        glTexCoord2f(1,1); glVertex3f(1*xFac, -1*yFac, -1);
        glTexCoord2f(1,0); glVertex3f(1*xFac, 1*yFac, -1);
        glTexCoord2f(0,0); glVertex3f(-1*xFac, 1*yFac, -1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glFlush();    
}
