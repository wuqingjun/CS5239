//
//  Sphere class
//
#include "Sphere.h"
#include <math.h>
#include <iostream>
#define M_PI 3.14159265
#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))

using namespace std;

//
//  Constructor
//
Sphere::Sphere(int n)
{
    scale(1);
    rebuild(n);
}

void Sphere::rebuild(int divs)
{
    if (divs == inc) return;

    //cerr << "building sphere with " << divs << " divs" << endl;
    inc = (divs>0) ? divs : 1;

    //  clear the old list, if applicable
    glDeleteLists(list, 1);
    //  Start new displaylist
    list = glGenLists(1);
    glNewList(list,GL_COMPILE);

    //  Bands of latitude
    float dh = 90.0/inc;
    for (int i = -inc; i <= inc; i++)
    {
        float phi = i * dh;
        glBegin(GL_QUAD_STRIP);
        float dw = 180.0/inc;
        for (int j = 0; j <= (inc * 2); j++)
        {
            float theta = j * dw;
            Vertex(theta,phi);
            Vertex(theta,phi+dh);
        }
        glEnd();
    }
    glEndList();

    //cerr << "done building sphere" << endl;
}

//
//  Set radius
//
void Sphere::scale(float r)
{
    Object::scale(r,r,r);
}

//
//  Draw vertex in polar coordinates with normal
//
void Sphere::Vertex(float th,float ph)
{
    float s = th/360;
    float t = ph/180+0.5;
    float x = Cos(th)*Cos(ph);
    float y = Sin(th)*Cos(ph);
    float z =         Sin(ph);

    float d = sqrt((x*x) + (y*y) + (z*z));
    x /= d;
    y /= d;
    z /= d;

    //  For a sphere at the origin, the position
    //  and normal vectors are the same
    glTexCoord2f(s,t);
    glNormal3f(x,y,z);
    glVertex3f(x,y,z);

    //glVertex3d(Sin(th)*Cos(ph),Cos(th)*Cos(ph),Sin(ph));
}

//
//  Display the sphere
//
void Sphere::display()
{
    //  Save transformation
    glPushMatrix();
    //  Offset, scale, rotate and color
    setTransform();
    setColor();
    //EnableTex();

    glCallList(list);

    //DisableTex();
    //  Undo transofrmations
    glPopMatrix();
}
