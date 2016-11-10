//
//  OpenGL widget
//
#include "hw08opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include "Cube.h"
#include "WaveOBJ.h"
#include "Teapot.h"
#include <math.h>
#include "lamp.h"
#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))

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
// SimplexTexture
//   Create and load a 1D texture for a simplex traversal order lookup table.
//   This is used for simplex noise only, and only for 3D and 4D noise where there are more than 2 simplices.
//   (3D simplex noise has 6 cases to sort out, 4D simplex noise has 24 cases.)
//
static void SimplexTexture(int unit)
{
   unsigned int id;
   // This is a look-up table to speed up the decision on which simplex we are in inside a cube or hypercube "cell"
   // for 3D and 4D simplex noise.  It is used to avoid complicated nested conditionals in the GLSL code.
   // The table is indexed in GLSL with the results of six pair-wise comparisons beween the components of the 
   // P=(x,y,z,w) coordinates within a hypercube cell.
   //   c1 = x>=y ? 32 : 0;
   //   c2 = x>=z ? 16 : 0;
   //   c3 = y>=z ? 8 : 0;
   //   c4 = x>=w ? 4 : 0;
   //   c5 = y>=w ? 2 : 0;
   //   c6 = z>=w ? 1 : 0;
   //   offsets = simplex[c1+c2+c3+c4+c5+c6];
   //   o1 = step(160,offsets);
   //   o2 = step(96,offsets);
   //   o3 = step(32,offsets);
   //   (For the 3D case, c4, c5, c6 and o3 are not needed.)
   unsigned char simplex4[][4] = {{0,64,128,192},{0,64,192,128},{0,0,0,0},{0,128,192,64},{0,0,0,0},{0,0,0,0},{0,0,0,0},{64,128,192,0},{0,128,64,192},{0,0,0,0},{0,192,64,128},{0,192,128,64},{0,0,0,0},{0,0,0,0},{0,0,0,0},{64,192,128,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{64,128,0,192},{0,0,0,0},{64,192,0,128},{0,0,0,0},{0,0,0,0},{0,0,0,0},{128,192,0,64},{128,192,64,0},{64,0,128,192},{64,0,192,128},{0,0,0,0},{0,0,0,0},{0,0,0,0},{128,0,192,64},{0,0,0,0},{128,64,192,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{128,0,64,192},{0,0,0,0},{0,0,0,0},{0,0,0,0},{192,0,64,128},{192,0,128,64},{0,0,0,0},{192,64,128,0},{128,64,0,192},{0,0,0,0},{0,0,0,0},{0,0,0,0},{192,64,0,128},{0,0,0,0},{192,128,0,64},{192,128,64,0}};

   //  Select texture unit
   glActiveTexture(unit);

   //  Generate 1D texture id and make current
   glGenTextures(1,&id);
   glBindTexture(GL_TEXTURE_1D,id);

   //  Set texture
   glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,64,0,GL_RGBA,GL_UNSIGNED_BYTE,simplex4);
   glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

   // Switch active texture unit back to 0 again
   glActiveTexture(GL_TEXTURE0);
}

//
// PermTexture(GLuint *texID)
//    Create and load a 2D texture for a combined index permutation and gradient lookup table.
//    This texture is used for 2D and 3D noise, both classic and simplex.
//
static void PermTexture(int unit)
{
   unsigned int id;
   int i,j,k=0;
   unsigned char pixels[4*256*256];
   // These are Ken Perlin's proposed gradients for 3D noise. I kept them for
   // better consistency with the reference implementation, but there is really
   // no need to pad this to 16 gradients for this particular implementation.
   // If only the "proper" first 12 gradients are used, they can be extracted
   // from the grad4[][] array: grad3[i][j] == grad4[i*2][j], 0<=i<=11, j=0,1,2
   // (12 cube edges + 4 more to make 16)
   int perm[256]= {151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};
   int grad3[16][3] = {{0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},{1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},{1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0},{1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}};

   //  Select texture unit
   glActiveTexture(unit);

   //  Generate 2D texture id and make current
   glGenTextures(1,&id);
   glBindTexture(GL_TEXTURE_2D,id);

   //  Set pixels
   for (i=0;i<256;i++)
      for(j=0;j<256;j++)
      {
         char value = perm[(j+perm[i]) & 0xFF];
         pixels[k++] = grad3[value & 0x0F][0] * 64 + 64; // Gradient x
         pixels[k++] = grad3[value & 0x0F][1] * 64 + 64; // Gradient y
         pixels[k++] = grad3[value & 0x0F][2] * 64 + 64; // Gradient z
         pixels[k++] = value;                            // Permuted index
      }

   //  Set texture
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,256,256,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

   // Switch active texture unit back to 0 again
   glActiveTexture(GL_TEXTURE0);
}

//
// GradTexture:
//   Create and load a 2D texture for a 4D gradient lookup table. 
//   This is used for 4D noise only.
//
static void GradTexture(int unit)
{
   unsigned int id;
   int i,j,k=0;
   unsigned char pixels[4*256*256];
   // These are Stefan Gustavson's gradients for 4D noise.
   // They are the coordinates of the midpoints of each of the 32 edges of a tesseract,
   // just like the 3D noise gradients are the midpoints of the 12 edges of a cube.
   int perm[256]= {151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};
   int grad4[32][4]= {{0,1,1,1},{0,1,1,-1},{0,1,-1,1},{0,1,-1,-1},{0,-1,1,1},{0,-1,1,-1},{0,-1,-1,1},{0,-1,-1,-1},{1,0,1,1},{1,0,1,-1},{1,0,-1,1},{1,0,-1,-1},{-1,0,1,1},{-1,0,1,-1},{-1,0,-1,1},{-1,0,-1,-1},{1,1,0,1},{1,1,0,-1},{1,-1,0,1},{1,-1,0,-1},{-1,1,0,1},{-1,1,0,-1},{-1,-1,0,1},{-1,-1,0,-1},{1,1,1,0},{1,1,-1,0},{1,-1,1,0},{1,-1,-1,0},{-1,1,1,0},{-1,1,-1,0},{-1,-1,1,0},{-1,-1,-1,0}};

   //  Select texture unit
   glActiveTexture(unit);

   //  Generate 2D texture id and make current
   glGenTextures(1,&id);
   glBindTexture(GL_TEXTURE_2D,id);

   //  Set pixels
   for (i=0;i<256;i++)
      for (j=0;j<256;j++)
      {
         char value = perm[(j+perm[i]) & 0xFF];
         pixels[k++] = grad4[value & 0x1F][0] * 64 + 64; // Gradient x
         pixels[k++] = grad4[value & 0x1F][1] * 64 + 64; // Gradient y
         pixels[k++] = grad4[value & 0x1F][2] * 64 + 64; // Gradient z
         pixels[k++] = grad4[value & 0x1F][3] * 64 + 64; // Gradient z
      }

   //  Set texture
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,256,256,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

   // Switch active texture unit back to 0 again
   glActiveTexture(GL_TEXTURE0);
}


//
// GradTexture:
//   Create and load a 2D texture for a 4D gradient lookup table.
//   This is used for 4D noise only.
//
static void bambooTexture(int unit)
{
    unsigned int id;
    glActiveTexture(unit);
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D, id);
    QImage img(":/9.bmp");
    QImage rgba = QGLWidget::convertToGLFormat(img);
    glTexImage2D(GL_TEXTURE_2D,0,4,rgba.width(),rgba.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   // Switch active texture unit back to 0 again
   glActiveTexture(GL_TEXTURE0);
}

//
//  Constructor
//
Ex17opengl::Ex17opengl(QWidget* parent)
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
}

//
//  Reset view
//
void Ex17opengl::reset()
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
void Ex17opengl::setLightMove(bool on)
{
   move = on;
   updateGL();
}

//
//  Set shader
//
void Ex17opengl::setShader(int sel)
{
   mode = sel;
   //  Request redisplay
   updateGL();
}

//
//  Set light position
//
void Ex17opengl::setPos(int Zh)
{
   zh = Zh;
   //  Request redisplay
   updateGL();
}

//
//  Set light elevation
//
void Ex17opengl::setElev(int Z)
{
   z0 = 0.02*Z;
   //  Request redisplay
   updateGL();
}

//
//  Set projection
//
void Ex17opengl::setPerspective(int on)
{
   fov = on ? 55 : 0;
   Projection();
   //  Request redisplay
   updateGL();
}

//
//  Set object
//
void Ex17opengl::setObject(int type)
{
   obj = objects[type];
   //  Request redisplay
   updateGL();
}

//
//  Initialize
//
void Ex17opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Load shaders
   Shader(shader,":/ex17.vert",":/ex17.frag");

   // Cube
   Cube* cube = new Cube();
   objects.push_back(cube);

   // Teapot
   Teapot* pot = new Teapot(8);
   pot->scale(0.5);
   objects.push_back(pot);

   // Cruiser
   WaveOBJ* bunny=0;
   try
   {
      bunny = new WaveOBJ("bunny.obj",":/");
   }
   catch (QString err)
   {
      Fatal("Error loading object\n"+err);
   }
   if (bunny)
   {
      bunny->color(1,1,0);
      objects.push_back(bunny);
   }

   Lamp *lamp = new Lamp;
   objects.push_back(lamp);

   //  Set initial object
   obj = objects[0];

   //  Set noise texture
   SimplexTexture(GL_TEXTURE1);
   PermTexture(GL_TEXTURE2);
   GradTexture(GL_TEXTURE3);
   bambooTexture(GL_TEXTURE4);

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
void Ex17opengl::resizeGL(int width, int height)
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
void Ex17opengl::paintGL()
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
   if (mode)
   {
      shader.bind();
      shader.setUniformValue("mode",mode);
      shader.setUniformValue("time",t);
      shader.setUniformValue("SimpTex",1);
      shader.setUniformValue("PermTex",2);
      shader.setUniformValue("GradTex",3);
      shader.setUniformValue("bamBoobTexture", 4);
   }

   //  Draw scene
   glPushMatrix();
   if (obj) obj->display();
   glPopMatrix();

   //  Release shader
   if (mode) shader.release();
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   
   //  Emit angles to display
   emit angles(QString::number(th)+","+QString::number(ph));
   //  Emit light angle
   emit light((int)zh);
}

//
//  Throw a fatal error and die
//
void Ex17opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Ex17opengl",message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void Ex17opengl::Projection()
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
void Ex17opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Ex17opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Ex17opengl::mouseMoveEvent(QMouseEvent* e)
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
void Ex17opengl::wheelEvent(QWheelEvent* e)
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
void Ex17opengl::Shader(QGLShaderProgram& shader,QString vert,QString frag)
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
