//
//  OpenGL Hw01opengl widget
//
#include "hw01opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include "Cube.h"
#include "WaveOBJ.h"
#include "Teapot.h"
#include "Sphere.h"
#include "Lamp.h"
#include <math.h>
#include <iostream>
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

//
//  Constructor
//
Hw01opengl::Hw01opengl(QWidget* parent)
    : QGLWidget(parent)
{
   mode = 0;
   init  = false;
   light = false;
   mouse = false;
   asp = 1;
   dim = 3;
   fov = 0;
   th = ph = 0;
}

//
//  Set shader
//
void Hw01opengl::setShader(int on)
{
   mode = on;
   //  Request redisplay
   updateGL();
}

//
// Set X Position
//
void Hw01opengl::setXPosition(int position)
{
    float oldY = obj->getY();
    float oldZ = obj->getZ();
    obj->translate((float)position, oldY, oldZ);
    
    std::cout << position;
    //  Request redisplay
    updateGL();
}

//
// Set Y Position
//
void Hw01opengl::setYPosition(int position)
{
    float oldX = obj->getX();
    float oldZ = obj->getZ();
    obj->translate(oldX, (float)position, oldZ);
    
    //  Request redisplay
    updateGL();
}

//
// Set Z Position
//
void Hw01opengl::setZPosition(int position)
{
    float oldX = obj->getX();
    float oldY = obj->getY();
    obj->translate(oldX, oldY, (float)position);
    
    //  Request redisplay
    updateGL();
}

void Hw01opengl::setScale(int position)
{

    obj->scale(float (position + 10) / 10 );
    updateGL();
}

//
//  Set lighting
//
void Hw01opengl::setLighting(int on)
{
   light = on;
   //  Request redisplay
   updateGL();
}

//
//  Set projection
//
void Hw01opengl::setPerspective(int on)
{
   fov = on ? 55 : 0;
   Projection();
   //  Request redisplay
   updateGL();
}

//
//  Set object
//
void Hw01opengl::setObject(int type)
{
   obj = objects[type];
   //  Request redisplay
   updateGL();
}

//
//  Initialize
//
void Hw01opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Enable Z-buffer depth testing
   glEnable(GL_DEPTH_TEST);
   //  Build shader
   if (!shader.addShaderFromSourceFile(QGLShader::Vertex,":/hw01.vert"))
      Fatal("Error compiling hw01.vert\n"+shader.log());
   if (!shader.addShaderFromSourceFile(QGLShader::Fragment,":/hw01.frag"))
      Fatal("Error compiling hw01.frag\n"+shader.log());
   if (!shader.link())
      Fatal("Error linking shader\n"+shader.log());

   // Cube
   objects.push_back(new Cube());

   // Sphere
   Sphere* sphere = new Sphere(0,0,0,2,1,1,1 );
   objects.push_back(sphere);
   
   // Teapot
   Teapot* pot = new Teapot(8);
   pot->scale(0.5);
   pot->color(0,1,1);
   objects.push_back(pot);

   // T-Rex
   WaveOBJ* tyra=0;
   try
   {
      tyra = new WaveOBJ(":/tyra.obj");
   }
   catch (QString err)
   {
      Fatal("Error loading object\n"+err);
   }
   if (tyra)
   {
      tyra->color(1,1,0);
      objects.push_back(tyra);
   }

   objects.push_back(new Lamp());
   
   //  Set initial object
   obj = objects[0];

   //  Start 100 fps timer connected to updateGL
   timer.setInterval(10);
   connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
   timer.start();
   time.start();
}

//
//  Set projection when window is resized
//
void Hw01opengl::resizeGL(int width, int height)
{
   //  Window aspect ration
   asp = height ? width / (float)height : 1;
   //  Viewport is whole screen
   glViewport(0,0,width,height);
   //  Set projection
   Projection();
}

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
//  Draw the window
//
void Hw01opengl::paintGL()
{
   //  Wall time (seconds)
   float t = 0.001*time.elapsed();

   //  Clear screen and Z-buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //  Set view
   glLoadIdentity();
   if (fov) glTranslated(0,0,-2*dim);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);

   //  Enable lighting
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]  = {0.3,0.3,0.3,1.0};
      float Diffuse[]  = {0.8,0.8,0.8,1.0};
      float Specular[] = {1.0,1.0,1.0,1.0};
      float Position[] = {(float)(3*Cos(90*t)),3.0,(float)(3*Sin(90*t)),1.0};
      //  Draw light position (no lighting yet)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }

   //  Apply shader
   if (mode)
   {
      shader.bind();
      shader.setUniformValue("time",t);
   }

   //  Draw scene
   if (obj) obj->display();

   //  Release shader
   if (mode) shader.release();

   //  Disable lighting
   glDisable(GL_LIGHTING);
   
   //  Draw Axes
   glColor3f(1,1,1);
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(1,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,1,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,1);
   glEnd();

   //  Emit angles to display
   emit angles(QString::number(th)+","+QString::number(ph));
}

//
//  Throw a fatal error and die
//
void Hw01opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Hw01opengl",message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void Hw01opengl::Projection()
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
void Hw01opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Hw01opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Hw01opengl::mouseMoveEvent(QMouseEvent* e)
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
void Hw01opengl::wheelEvent(QWheelEvent* e)
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
