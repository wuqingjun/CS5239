//
//  Sphere class
//
#include "Sphere.h"
#include <math.h>
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

//
//  Constructor
//
Sphere::Sphere(float x,float y,float z,float r,float Rc,float Gc,float Bc):
   Object(x,y,z)
{
   r0 = r;
   R  = Rc;
   G  = Gc;
   B  = Bc;
}

//
//  Set radius
//
void Sphere::radius(float r)
{
   r0 = r;
}

//
//  Draw vertex in polar coordinates with normal
//
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

void Sphere::scale(float S)
{
    r0 = S;
}

//
//  Display the sphere
//
void Sphere::display()
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x0,y0,z0);
   glScaled(r0,r0,r0);
   // Colored sphere
   glColor3f(R,G,B);
   //  Bands of latitude
   const int inc=15;
   for (int ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}
