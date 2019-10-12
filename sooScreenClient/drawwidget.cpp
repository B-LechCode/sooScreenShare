#include "drawwidget.h"
#if USEGL
/**/
drawWidget::drawWidget(QWidget *parent ,Qt::WindowFlags f):
   QOpenGLWidget(parent,f)
{

}


bool init = false;

GLuint oldid=0;
void drawWidget::paintGL()
{
    glClearColor(1.0,1.0,1.0,00);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(m_drawImage.empty())
       return;

    glEnable(GL_TEXTURE_2D);
    GLuint texture_id;

    if(!init)
        glDeleteTextures(1,&oldid);

    glGenTextures(1, &texture_id);
    oldid = texture_id;
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_drawImage.cols , m_drawImage.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, m_drawImage.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



    glBindTexture(GL_TEXTURE_2D, texture_id);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex3f(-1, -1, -1);
        glTexCoord2f(1,1); glVertex3f(1, -1, -1);
        glTexCoord2f(1,0); glVertex3f(1, 1, -1);
        glTexCoord2f(0,0); glVertex3f(-1, 1, -1);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glFlush();    
}


#else
drawWidget::drawWidget(QWidget *parent ,Qt::WindowFlags f):
    QWidget(parent,f)
{

}

drawWidget::~drawWidget()
{

}
#endif
