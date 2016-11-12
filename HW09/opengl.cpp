//
//  OpenGL widget
//
#include "opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include <QGLFunctions>
#include "Noise.h"
#include <math.h>

//  Set up array indexes for program
#define VELOCITY_ARRAY 4
#define START_ARRAY    5
const char* Name[] = {"","","","","Vel","Start",NULL};

/*
 *  Random numbers with range and offset
 */
static float frand(float rng,float off)
{
   return rand()*rng/RAND_MAX+off;
}

/*
 *  Initialize particles
 */
void Opengl::InitSmoke(void)
{
   //  Array Pointers
   float* vert  = Vert;
   float* color = Color;
   float* vel   = Vel;
   float* start = Start;
   //  Loop over NxN patch
   n = mode ? 22 : N;
   for (int i=0;i<n;i++)
      for (int j=0;j<n;j++)
      {
          //  Location x,y,z
          *vert++ = (i+0.5)/n;-0.75;
          *vert++ = 0;
          *vert++ = (j+0.5)/n-0.75;
          //  Color r,g,b (0.5-1.0)
          *color++ = frand(0.5,0.5);
          *color++ = frand(0.5,0.5);
          *color++ = frand(0.5,0.5);
          //  Velocity
          *vel++ = frand( 1.0,3.0);
          *vel++ = frand(10.0,0.0);
          *vel++ = frand( 1.0,3.0);
          //  Launch time
          *start++ = frand(2.0,0.0);
      }
}

void Opengl::InitRain(void)
{
   //  Array Pointers
   float* vert  = Vert;
   float* color = Color;
   float* vel   = Vel;
   float* start = Start;
   //  Loop over NxN patch
   n = mode ? 15 : N;
   for (int i=0;i<n;i++)
      for (int j=0;j<n;j++)
      {
         //  Location x,y,z
         *vert++ = i - 5.0;
         *vert++ = 5;
         *vert++ = (j+0.5)/n-0.75;
         //  Color r,g,b (0.5-1.0)
         *color++ = frand(0.5,0.5);
         *color++ = frand(0.5,0.5);
         *color++ = frand(0.5,0.5);
         //  Velocity
         *vel++ = 0.0; frand( 1.0,3.0);
         *vel++ =  frand(10.0,0.0);
         *vel++ = 0.0; frand( 1.0,3.0);
         //  Launch time
         *start++ = frand(2.0,0.0);
      }
}

/*
 *  Draw particles
 */
void Opengl::DrawPart(void)
{
   QGLFunctions glf(QGLContext::currentContext());
   //  Set particle size
   glPointSize(mode?50:2);
   //  Point vertex location to local array Vert
   glVertexPointer(3,GL_FLOAT,0,Vert);
   //  Point color array to local array Color
   glColorPointer(3,GL_FLOAT,0,Color);
   //  Point attribute arrays to local arrays
   glf.glVertexAttribPointer(VELOCITY_ARRAY,3,GL_FLOAT,GL_FALSE,0,Vel);
   glf.glVertexAttribPointer(START_ARRAY,1,GL_FLOAT,GL_FALSE,0,Start);
   //  Enable arrays used by DrawArrays
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
   glf.glEnableVertexAttribArray(VELOCITY_ARRAY);
   glf.glEnableVertexAttribArray(START_ARRAY);
   //  Set transparent large particles
  // if (mode)
   {
      glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
      glEnable(GL_POINT_SPRITE);
      glTexEnvi(GL_POINT_SPRITE,GL_COORD_REPLACE,GL_TRUE);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE);
      glDepthMask(0);
   }
   //  Draw arrays
   glDrawArrays(GL_POINTS,0,n*n);
   //  Reset
   //if (mode)
   {
      glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
      glDisable(GL_POINT_SPRITE);
      glDisable(GL_BLEND);
      glDepthMask(1);
   }
   //  Disable arrays
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);
   glf.glDisableVertexAttribArray(VELOCITY_ARRAY);
   glf.glDisableVertexAttribArray(START_ARRAY);
}

//
//  Constructor
//
Opengl::Opengl(QWidget* parent)
    : QGLWidget(parent)
{
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
   N = 100;
   Vert  = new float[3*N*N];
   Color = new float[3*N*N];
   Vel   = new float[3*N*N];
   Start = new float[N*N];
   InitRain();
}

//
//  Reset view
//
void Opengl::reset()
{
   th = ph = 0;
   dim = 3;
   Projection();
   //  Request redisplay
   updateGL();
}

//
//  Set shader
//
void Opengl::setShader(int sel)
{
   mode = sel;

   if(mode == 1){
    InitSmoke();
   }
   else {
       InitRain();
   }
   //  Request redisplay
   updateGL();
}

//
//  Set projection
//
void Opengl::setPerspective(int on)
{
   fov = on ? 55 : 0;
   Projection();
   //  Request redisplay
   updateGL();
}

//
//  Initialize
//
void Opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Load shaders
   Shader(shader[0],":/ex19a.vert","");
   Shader(shader[1],":/ex19b.vert",":/ex19b.frag");

   //  Load random texture
   CreateNoise3D(GL_TEXTURE1);
   //  Load smoke particle
   QPixmap smoke(":/particle.png");
   bindTexture(smoke,GL_TEXTURE_2D);

   //  Start 100 fps timer connected to updateGL
   move = true;
   timer.setInterval(1);
   connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
   timer.start();
   time.start();
}

//
//  Set projection when window is resized
//
void Opengl::resizeGL(int width, int height)
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
void Opengl::paintGL()
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

   //  Apply shader
   shader[mode].bind();
   shader[mode].setUniformValue("time",t);
   shader[mode].setUniformValue("img",0);
   shader[mode].setUniformValue("Noise3D",1);

   //  Draw scene
   DrawPart();

   //  Release shader
   shader[mode].release();
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);

   //  Draw axes
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(1,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,1,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,1);
   glEnd();
   //  Label axes
   renderText(1,0,0,"X");
   renderText(0,1,0,"Y");
   renderText(0,0,1,"Z");
   
   //  Emit angles to display
   emit angles(QString::number(th)+","+QString::number(ph));
}

//
//  Throw a fatal error and die
//
void Opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Opengl",message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void Opengl::Projection()
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
void Opengl::wheelEvent(QWheelEvent* e)
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
void Opengl::Shader(QGLShaderProgram& shader,QString vert,QString frag)
{
   QGLFunctions glf(QGLContext::currentContext());
   //  Vertex shader
   if (vert.length() && !shader.addShaderFromSourceFile(QGLShader::Vertex,vert))
      Fatal("Error compiling "+vert+"\n"+shader.log());
   //  Fragment shader
   if (frag.length() && !shader.addShaderFromSourceFile(QGLShader::Fragment,frag))
      Fatal("Error compiling "+frag+"\n"+shader.log());
   //  Bind Attribute Locations
   for (int k=0;Name[k];k++)
      if (Name[k][0])
         glf.glBindAttribLocation(shader.programId(),k,Name[k]);
   //  Link
   if (!shader.link())
      Fatal("Error linking shader\n"+shader.log());
}
