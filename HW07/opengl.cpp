//
//  OpenGL widget
//
#include "opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include <math.h>

//
//  Constructor
//
Opengl::Opengl(QWidget* parent)
    : QGLWidget(parent)
{
   mode = 0;
   init  = false;
   mouse = false;
   x0 = y0 = 0;
   zoom = 1;
}

//
//  Reset view
//
void Opengl::reset()
{
   x0 = y0 = 0;
   zoom = 1;
   frac = 0;
   //  Request redisplay
   updateGL();
}

//
//  Set shader
//
void Opengl::setFrac(int pct)
{
   frac = pct;
   //  Request redisplay
   updateGL();
}

//
//  Set shader
//
void Opengl::setShader(int sel)
{
   mode = sel;
   //  Request redisplay
   updateGL();
}

//
//  Load image to texture unit
//
void Opengl::LoadImage(int unit,const QString file)
{
   //  Load image
   QImage img(file);
   //  Select texture unit
   glActiveTexture(unit);
   //  Bind texture
   unsigned int tex;
   glGenTextures(1,&tex);
   glBindTexture(GL_TEXTURE_2D,tex);
   //  Copy image to texture
   QImage rgba = QGLWidget::convertToGLFormat(img);
   w = rgba.width();
   h = rgba.height();
   glTexImage2D(GL_TEXTURE_2D,0,4,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.bits());
   //  Set pixel interpolation
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

//
//  Initialize
//
void Opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Load shader
   Shader(shader,"",":/blackwhite.frag");

   //  Load images
   LoadImage(GL_TEXTURE0,":/20090602.png");
}

//
//  Set projection
//
void Opengl::resizeGL(int width, int height)
{
   //  Window aspect ratio
   float asp = width/(float)height;
   //  Viewport is entire window
   glViewport(0,0,width,height);
   //  Set Projection
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-asp, +asp, -1, +1, -1, +1);
   //  Set Projection
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

//
//  Draw the window
//
void Opengl::paintGL()
{
   //  Image aspect ratio
   float asp = w / (float)h;

   //  Enable shader
   shader.bind();

   //  Set pixel increments
   shader.setUniformValue("img0",0);
   shader.setUniformValue("frac",(float)(frac/100.0));
   shader.setUniformValue("mode",mode);

   //  Set ModelView
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glScaled(zoom,zoom,1);
   glTranslated(x0,y0,0);
   //  Draw to screen
   glClear(GL_COLOR_BUFFER_BIT);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex2f(-asp,-1);
   glTexCoord2f(1,0); glVertex2f(+asp,-1);
   glTexCoord2f(1,1); glVertex2f(+asp,+1);
   glTexCoord2f(0,1); glVertex2f(-asp,+1);
   glEnd();

   //  Done with shader
   shader.release();
}

//
//  Throw a fatal error and die
//
void Opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Opengl",message);
   QApplication::quit();
}

/******************************************************************/
/*************************  Mouse Events  *************************/
/******************************************************************/
//
//  Mouse pressed
//
void Opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Opengl::mouseMoveEvent(QMouseEvent* e)
{
   if (mouse)
   {
      QPoint d = e->pos()-pos;      //  Change in mouse location
      x0 += d.x()/(zoom*width());   //  Translate x movement to azimuth
      y0 -= d.y()/(zoom*height());  //  Translate y movement to elevation
      pos = e->pos();               //  Remember new location
      updateGL();                   //  Request redisplay
   }
}

//
//  Mouse wheel
//
void Opengl::wheelEvent(QWheelEvent* e)
{
   //  Zoom out
   if (e->delta()<0)
      zoom *= 1.05;
   //  Zoom in
   else if (zoom>1)
      zoom /= 1.05;
   //  Request redisplay
   updateGL();
}

//
//  Load shader
//
void Opengl::Shader(QGLShaderProgram& shader,QString vert,QString frag)
{
   //  Vertex shader
   if (vert.length() && !shader.addShaderFromSourceFile(QGLShader::Vertex,vert))
      Fatal("Error compiling "+vert+"\n"+shader.log());
   //  Fragment shader
   if (frag.length() && !shader.addShaderFromSourceFile(QGLShader::Fragment,frag))
      Fatal("Error compiling "+frag+"\n"+shader.log());
   //  Link
   if (!shader.link())
      Fatal("Error linking shader\n"+shader.log());
}
