//
//  OpenGL Hw02opengl widget
//
#include "hw02opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include "WaveOBJ.h"
#include "Lamp.h"
#include "Cube.h"
#include "Teapot.h"
#include <math.h>
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
#include <QVector3D>

//
//  Draw vertex in polar coordinates
//
static void Vertex(double th,double ph)
{
   glVertex3d(Sin(th)*Cos(ph),Cos(th)*Cos(ph),Sin(ph));
}

//
//  Draw a ball at (x,y,z) radius r
//
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  Bands of latitude
   for (int ph=-90;ph<90;ph+=10)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=20)
      {
         Vertex(th,ph);
         Vertex(th,ph+10);
      }
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();
}

//
//  Constructor
//
Hw02opengl::Hw02opengl(QWidget* parent)
    : QGLWidget(parent)
{
   mode = 0;
   init  = false;
   mouse = false;
   asp = 1;
   dim = 3;
   fov = 0;
   th = ph = 0;
   x0 = y0 = 0;
   z0 = 1;
   zh = 0;
}

//
//  Reset view
//
void Hw02opengl::reset()
{
   th = ph = 0;
   dim = 3;
   Projection();
   //  Request redisplay
   updateGL();
}

//
//  Light animation
//
void Hw02opengl::setLightMove(bool on)
{
   move = on;
   updateGL();
}

//
//  Set shader
//
void Hw02opengl::setShader(int sel)
{
   mode = sel;
   //  Request redisplay
   updateGL();
}

//
//  Set X position
//
void Hw02opengl::setX(int X)
{
   x0 = X;
   obj->translate(x0,y0,z0);
   //  Request redisplay
   updateGL();
}

//
//  Set Y position
//
void Hw02opengl::setY(int Y)
{
   y0 = Y;
   obj->translate(x0,y0,z0);
   //  Request redisplay
   updateGL();
}

//
//  Set Zoom
//
void Hw02opengl::setZ(int Z)
{
   z0 = Z;
   obj->translate(x0,y0,z0);
   //  Request redisplay
   updateGL();
}

//
//  Set projection
//
void Hw02opengl::setPerspective(int on)
{
   fov = on ? 55 : 0;
   Projection();
   //  Request redisplay
   updateGL();
}

//
//  Set object
//
void Hw02opengl::setObject(int type)
{
   obj = objects[type];
   //  Request redisplay
   updateGL();
}

//
//  Initialize
//
void Hw02opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Load shaders
   Shader(1,":/hw02a.vert",":/hw02a.frag");
   Shader(2,":/hw02a.vert",":/hw02b.frag");

   // Cube
   objects.push_back(new Cube());

   // Teapot
   Teapot* pot = new Teapot(8);
   pot->scale(0.5);
   pot->color(0,1,1);
   objects.push_back(pot);

   // Fish
   objects.push_back(new Lamp());

   //  Set initial object
   obj = objects[0];

   //  Start 100 fps timer connected to updateGL
   move = true;
   timer.setInterval(10);
   connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
   timer.start();
   time.start();
}

//
//  Set projection when window is resized
//
void Hw02opengl::resizeGL(int width, int height)
{
   //  Window aspect ration
   asp = height ? width / (float)height : 1;
   //  Viewport is whole screen
   glViewport(0,0,width,height);
   //  Set projection
   Projection();
}

//
//  Draw the window
//
void Hw02opengl::paintGL()
{
   //  Wall time (seconds)
   float t = 0.001*time.elapsed();
   if (move) zh = fmod(90*t,360);

   //  Clear screen and Z-buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

   //  Set view
   glLoadIdentity();
   if (fov) glTranslated(0,0,-2*dim);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);

   //  Translate intensity to color vectors
   float Ambient[]  = {0.3,0.3,0.3,1.0};
   float Diffuse[]  = {0.8,0.8,0.8,1.0};
   float Specular[] = {1.0,1.0,1.0,1.0};
   float Position[] = {(float)(3*Cos(zh)),2.0,(float)(3*Sin(zh)),1.0};

   //  Draw light position (no lighting yet)
   glColor3f(1,1,1);
   ball(Position[0],Position[1],Position[2] , 0.1);
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);

   //  Apply shader
   if (mode)
   {
      shader[mode].bind();
      //  Dimensions
      QVector3D loc(x0,y0,1/z0);
      shader[mode].setUniformValue("time",t);
   }

   //  Draw scene
   glPushMatrix();
   if (obj) obj->display();
   glPopMatrix();

   //  Release shader
   if (mode) shader[mode].release();
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   
   //  Emit angles to display
   emit angles(QString::number(th)+","+QString::number(ph));
   //  Emit centers to display
   emit position("X: " + QString::number((int)x0) + ", \tY: " + QString::number((int)y0) + ", \tZ: " + QString::number((int)z0));
   //  Emit Zoom position
   emit zoom(QString::number((int)dim));
}

//
//  Throw a fatal error and die
//
void Hw02opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Hw02opengl",message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void Hw02opengl::Projection()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (fov)
   {
      float zmin = dim/4;
      float zmax = 4*dim;
      float ydim = zmin*tan(fov*M_PI/360);
      float xdim = ydim*asp;
      glFrustum(-xdim,+xdim,-ydim,+ydim,zmin,zmax);
   }
   else
      glOrtho(-dim*asp, +dim*asp, -dim, +dim, -dim, +dim);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/******************************************************************/
/*************************  Mouse Events  *************************/
/******************************************************************/
//
//  Mouse pressed
//
void Hw02opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Hw02opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Hw02opengl::mouseMoveEvent(QMouseEvent* e)
{
   if (mouse)
   {
      QPoint d = e->pos()-pos;  //  Change in mouse location
      th = (th+d.x())%360;      //  Translate x movement to azimuth
      ph = (ph+d.y())%360;      //  Translate y movement to elevation
      pos = e->pos();           //  Remember new location
      updateGL();               //  Request redisplay
   }
}

//
//  Mouse wheel
//
void Hw02opengl::wheelEvent(QWheelEvent* e)
{
   //  Zoom out
   if (e->delta()<0)
      dim += 0.1;
   //  Zoom in
   else if (dim>1)
      dim -= 0.1;
   //  Request redisplay
   Projection();
   updateGL();
}

//
//  Load shader
//
void Hw02opengl::Shader(int k,QString vert,QString frag)
{
   //  Vertex shader
   if (vert.length() && !shader[k].addShaderFromSourceFile(QGLShader::Vertex,vert))
      Fatal("Error compiling "+vert+"\n"+shader[k].log());
   //  Fragment shader
   if (frag.length() && !shader[k].addShaderFromSourceFile(QGLShader::Fragment,frag))
      Fatal("Error compiling "+frag+"\n"+shader[k].log());
   //  Link
   if (!shader[k].link())
      Fatal("Error linking shader\n"+shader[k].log());
}
