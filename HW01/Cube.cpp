//
//  Cube class
//
#include "Cube.h"

//
//  Constructor
//
Cube::Cube(float x,float y,float z,float dx,float dy,float dz,float th) :
   Object(x,y,z)
{
   Sx = dx;
   Sy = dy;
   Sz = dz;
   Th = th;
}

//
//  Set rotation
//
void Cube::rotate(float th)
{
   Th = th;
}

//
//  Set scaling
//
void Cube::scale(float dx,float dy,float dz)
{
   Sx = dx;
   Sy = dy;
   Sz = dz;
}

void Cube::scale(float S)
{
    Sx = S;
    Sy = S;
    Sz = S;
}

//
//  Display the cube
//
void Cube::display()
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x0,y0,z0);
   glRotated(Th,0,1,0);
   glScaled(Sx,Sy,Sz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glNormal3f( 0, 0, 1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glNormal3f( 0, 0,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glNormal3f(+1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glNormal3f( 0,+1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glNormal3f( 0,-1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}
