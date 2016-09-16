#include "Lamp.h"
#include <math.h>

//
// draw a square
//

Lamp::Lamp()
{
    s = 1;
    th = 0;
    dx = dy = dz = 1;
    x0 = y0 = z0 = 0;
    R = G = B = 1;
}

double Lamp::Sin(double th)
{
    return sin(th * PI / 180);
}

double Lamp::Cos(double th)
{
    return cos(th * PI / 180);
}

double Lamp::Tan(double th)
{
    return tan(th * PI / 180);
}


void Lamp::square(double x, double y, double z, double l, double w)
{

    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(l, 1, w);
        glColor3f(1.0,1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(1.0, 0, 1.0);
    glVertex3f(1.0, 0, -1.0);
    glVertex3f(-1.0, 0, -1.0);
    glVertex3f(-1.0, 0, 1.0);
    glEnd();
    glPopMatrix();
}


//
// draw the cover
//
void Lamp::cover(double x, double y, double z, double r, double angle, double h)
{
    double d = 1.0;
    glPushMatrix();
    glTranslated(x, y, z);
    glColor3f(1.0,1.0, 1.0);
    glBegin(GL_TRIANGLE_STRIP);
    double R = r + h * Tan(angle);
    for(int i = 0; i <=360; i += d)
    {
        glVertex3f(R * Cos(i), -h, R * Sin(i));
        glVertex3f(r * Cos(i), 0, r * Sin(i));
    }

    glEnd();
    glPopMatrix();
}

//
// draw 3 sticks at the top of the cover
//
void Lamp::sticks(double x, double y, double z, double r)
{
    glPushMatrix();

    glTranslated(x, y, z);
    glScaled(r, 1, r);

    glColor3f(1.0,1.0,0.0);
    glBegin(GL_LINES);

    for(int i = 0; i < 360; i += 120)
    {
        glVertex3f(0, 0, 0);
        glVertex3f(Sin(i), 0, Cos(i));
    }

    glEnd();
    glPopMatrix();
}

//
// a function used to draw spheres: bubble and the bottom of the lamp.
//
void Lamp::sphere(double x,
    double y,
    double z,
    double r,
    int startangle,
    int endangle)
{
    double d = 1.0;
    glPushMatrix();

    glTranslated(x, y, z);
    glScaled(r, r, r);
    glColor3f(R, G, B);

    for(int j = startangle; j <= endangle; j += d)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0; i <=360; i += d)
        {
            glVertex3f(Sin(j) * Sin(i), Cos(j), Sin(j) * Cos(i));
            glVertex3f(Sin(j + d) * Sin(i), Cos(j +  d), Sin(j + d) * Cos(i));
        }
        glEnd();
    }

    glPopMatrix();
}

//
// draw the cylinder. for example the stick holding the bubble.
//
void Lamp::cylinder(double x, double y, double z, double r, double h)
{
    double d = 1.0;
    glPushMatrix();

    glTranslated(x, y, z);
    glScaled(r, 1, r);
    glColor3f(R, G, B);
    glBegin(GL_QUAD_STRIP);

   for(int i = 0;i <= 360; i += d)
   {
        glVertex3f(Cos(i), 0, Sin(i));
        glVertex3f(Cos(i), h, Sin(i));
    }

    glEnd();
    glPopMatrix();
}

//
// draw circular surface.
//
void Lamp::pie(double x, double y, double z, double r)
{
    double d = 1.0;
    glPushMatrix();

    glTranslated(x, y, z);
    glScaled(r, 1, r);
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_POLYGON);

    for(int i = 0;i <= 360; i += d)
    {
        glVertex3f(Cos(i), 0, Sin(i));
    }

    glEnd();
    glPopMatrix();
}


//
// Draw the entire lamp.
//

void Lamp::Draw(float x, float y, float z, float s,  bool hasCover)
{
    glPushMatrix();
    glScaled(s, s, s);
    glTranslated(x, y, z);
    pie(0, 0, 0, 0.32);
    sphere(0, 0.39, 0, 0.50, 60, 140);
    pie(0, 0.64, 0, 0.44);
    cylinder(0, 0.64, 0, 0.20, 0.10);
    cylinder(0, 0.74, 0, 0.20, 0.10);
    pie(0, 0.84, 0, 0.245);
    cylinder(0, 0.84, 0, 0.245, 0.10);
    pie(0, 0.94, 0, 0.245);
    cylinder(0, 0.94, 0, 0.03, 0.60);
    pie(0, 0.54, 0, 0.15);
    cylinder(0, 1.54, 0, .15, .10);
    pie(0, 1.64, 0, .15);
    sphere(0, 1.84, 0, .20, 0, 180);
    if(hasCover)
    {
        sticks(0, 2.07, 0, .40);
        cover(0, 2.07, 0, .40, 38, 1.00);
    }

    glPopMatrix();
}


void Lamp::scale(float S)
{
   s = S;
}

void Lamp::display()
{
    //Color colors1[7] = {Color(1.0, 1.0, 1.0, 1.0), Color(0.0, 0.8, 0.8, 1.0), Color(0.0, 0.1, 0.3, 1.0), Color(0.8, 0.3, 0.8, 1.0), Color(0.3, 0.8, 1.0, 1.0), Color(0.9, 0.7, 0.9, 1.0), Color(0.7, 0.3, 0.9, 1.0)};
    Draw(x0, y0, z0, s, true);
}
