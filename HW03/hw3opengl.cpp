//
//  OpenGL Hw3opengl widget
//
#include "hw3opengl.h"
#include <cstdlib>
#include <QtOpenGL>
#include <QMessageBox>
#include "Cube.h"
#include "WaveOBJ.h"
#include "Teapot.h"
#include "Sphere.h"
#include "lamp.h"
#include <math.h>
#include <sstream>
#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))

using namespace std;

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
   //  Undo transofrmations
   glPopMatrix();
}

//
//  Constructor
//
Hw3opengl::Hw3opengl(QWidget* parent)
    : QGLWidget(parent)
{
    //cerr << "Constructing QGLWidget" << endl;

    mode = 0;
    init  = false;
    mouse = false;
    asp = 1;
    dim = 3;
    fov = 55;
    th = ph = 0;
    x0 = y0 = 0;
    z0 = 1;
    zh = 0;
    //cerr << "Done constructing QGLWidget" << endl;
}

//
//  Reset view
//
void Hw3opengl::reset()
{
   th = ph = 0;
   dim = 3;
   Projection();
   //  Request redisplay
   //updateGL();
}

//
//  Light animation
//
void Hw3opengl::setLightMove(bool on)
{
   move = on;
   //updateGL();
}

//
//  Set shader
//
void Hw3opengl::setShader(int sel)
{
   mode = sel;
   //  Request redisplay
   //updateGL();
}

//
//  Set light position
//
void Hw3opengl::setPos(int Zh)
{
   zh = Zh;
   //  Request redisplay
   //updateGL();
}

//
//  Set light elevation
//
void Hw3opengl::setElev(int Z)
{
   z0 = 0.02*Z;
   //  Request redisplay
   //updateGL();
}

//
//  Set projection
//
void Hw3opengl::setPerspective(int on)
{
   fov = on ? 55 : 0;
   Projection();
   //  Request redisplay
   //updateGL();
}

//
//  Set object
//
void Hw3opengl::setObject(int type)
{
   obj = objects[type];
   //  Request redisplay
   //updateGL();
}

void Hw3opengl::setDivs(int d)
{
    divs = d;
    if (init)
    {
        sphere->rebuild(divs);
    }
}

//
//  Initialize
//
void Hw3opengl::initializeGL()
{
   if (init) return;
   init = true;

   //cerr << "Initializing OpenGL" << endl;

   //  Load shaders
   Shader(1,"",":/ex04a.frag");
   Shader(2,":/ex04b.vert",":/ex04b.frag");
   Shader(3,":/ex04bi.vert",":/ex04b.frag");
   Shader(4,":/ex04bm.vert",":/ex04b.frag");
   Shader(5,":/ex04c.vert",":/ex04c.frag");
   Shader(6,":/ex04ci.vert",":/ex04c.frag");
   Shader(7,":/ex04d.vert",":/ex04d.frag");
   Shader(8,":/ex04e.vert",":/ex04e.frag");

   // Cube
   Cube* cube = new Cube();
   cube->texture(":/crate.png");
   objects.push_back(cube);

   // Teapot
   Teapot* pot = new Teapot(8);
   pot->scale(0.5);
   pot->texture(":/pi.png");
   objects.push_back(pot);

   // Cruiser
   WaveOBJ* cruiser=0;
   try
   {
      cruiser = new WaveOBJ("cruiser.obj",":/");
   }
   catch (QString err)
   {
      Fatal("Error loading object\n"+err);
   }
   if (cruiser)
   {
      cruiser->color(1,1,0);
      objects.push_back(cruiser);
   }

   sphere = new Sphere(divs);
   sphere->color(1.0,1.0,1.0);
   objects.push_back(sphere);

   Lamp *lamp = new Lamp();
   lamp->texture(":/9.bmp");

   objects.push_back(lamp);

   //  Set initial object
   obj = objects[0];

   //  Start 100 fps timer connected to updateGL
   move = false;
   timer.setInterval(0);
   connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
   timer.start();
   time.start();

   framecount = 0;
   frameTimer.setInterval(1000);
   connect(&frameTimer,SIGNAL(timeout()),this,SLOT(frameTimerTicked()));
   frameTimer.start();

   frameDuration.start();
}

void Hw3opengl::frameTimerTicked()
{
    QString s = "FPS: ";

    double time = 1.0 / ((lastframe * .000000001) / framecount);

    s = s + QString::number(time);

    emit fps(s);
    framecount = 0;
    lastframe = 0;
}

//
//  Set projection when window is resized
//
void Hw3opengl::resizeGL(int width, int height)
{
   //  Window aspect ration
   asp = height ? width / (float)height : 1;
   //  Viewport is whole screen
   glViewport(0,0,width,height);
   //  Set projection
   Projection();

   QString p = QString::number(width) + "x" + QString::number(height) + " : " + QString::number(width * height) + " pixels";
   emit pixels(p);
}

//
//  Draw the window
//
void Hw3opengl::paintGL()
{

   //  Wall time (seconds)
   float t = 0.001*time.elapsed();
   if (move) zh = fmod(90*t,360);

   frameDuration.restart();

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
   float Position[] = {(float)(3*Cos(zh)),z0,(float)(3*Sin(zh)),1.0};

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
   if (mode) shader[mode].bind();

   //  Draw scene
   glPushMatrix();
   if (obj) obj->display();
   glPopMatrix();

   //  Release shader
   if (mode) shader[mode].release();
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);

   lastframe += frameDuration.nsecsElapsed();

   //  Emit angles to display
   emit angles(QString::number(th)+","+QString::number(ph));
   //  Emit light angle
   emit light((int)zh);

   framecount ++;
}

//
//  Throw a fatal error and die
//
void Hw3opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Hw3opengl",message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void Hw3opengl::Projection()
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
void Hw3opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Hw3opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Hw3opengl::mouseMoveEvent(QMouseEvent* e)
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
void Hw3opengl::wheelEvent(QWheelEvent* e)
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
void Hw3opengl::Shader(int k,QString vert,QString frag)
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
