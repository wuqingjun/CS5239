#include "Lamp.h"
#include <math.h>

//
// draw a square
//

Lamp::Lamp()
{
    //s = 1;
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
    int dir = 1.0;
    double d = 1.0;
    float rep = 1.0;
    glPushMatrix();
    glTranslated(x, y, z);
    glColor3f(1.0,1.0, 1.0);
    glBegin(GL_TRIANGLE_STRIP);
    double R = r + h * Tan(angle);
    for(int i = 0; i <=360; i += d)
    {
        glNormal3f(dir * Cos(angle) * Cos(i), dir * Sin(angle), dir * Cos(angle) * Sin(i));
        glTexCoord2f(rep * i / 360, 0);
        glVertex3f(R * Cos(i), -h, R * Sin(i));
        glNormal3f(dir * Cos(angle) * Cos(i), dir * Sin(angle), dir * Cos(angle) * Sin(i));
        glTexCoord2f(rep * i / 360, rep);
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
    float rep = 1.0;
    glPushMatrix();

    glTranslated(x, y, z);
    glScaled(r, r, r);
    glColor3f(R, G, B);
    EnableTex();

     for(int j = startangle; j <= endangle; j += d)
     {
         glBegin(GL_TRIANGLE_STRIP);
         for(int i = 0; i <=360; i += d)
         {
            glNormal3f(Sin(j) * Sin(i), Cos(j), Sin(j) * Cos(i));
            glTexCoord2f(rep * (float) i / 360, rep * (j - startangle) /(float) (endangle - startangle));
            glVertex3f(Sin(j) * Sin(i), Cos(j), Sin(j) * Cos(i));
            glTexCoord2f(rep * (float) i / 360, rep * (j + d - startangle) /(float) (endangle - startangle));
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
    float rep = 1.0;
    glPushMatrix();

    glTranslated(x, y, z);
    glScaled(r, 1, r);
    glColor3f(R, G, B);
    glBegin(GL_QUAD_STRIP);

    for(int i = 0;i <= 360; i += d)
    {
        glNormal3f(Cos(i), 0, Sin(i));
        glTexCoord2f(rep * (float) i / 360, 0);
        glVertex3f(Cos(i), 0, Sin(i));
        glNormal3f(Cos(i), 0, Sin(i));
        glTexCoord2f(rep * (float) i / 360, rep);
        glVertex3f(Cos(i), h, Sin(i));
     }


    glEnd();
    glPopMatrix();
}

//
// draw circular surface.
//
void Lamp::pie(double x, double y, double z, double r, int ynormal)
{
    double d = 1.0;
    glPushMatrix();

    glTranslated(x, y, z);
    glScaled(r, 1, r);
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_POLYGON);

    for(int i = 0;i <= 360; i += d)
    {
        glNormal3f(0, ynormal, 0);
        glTexCoord2f(Cos(i) * 0.5 + 0.5, Sin(i) * 0.5 + 0.5);
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
    EnableTex();
    pie(0, 0, 0, 0.32, -1);
    sphere(0, 0.39, 0, 0.50, 60, 140);
    pie(0, 0.64, 0, 0.44, 1);
    cylinder(0, 0.64, 0, 0.20, 0.10);
    cylinder(0, 0.74, 0, 0.20, 0.10);
    pie(0, 0.84, 0, 0.245, -1);
    cylinder(0, 0.84, 0, 0.245, 0.10);
    pie(0, 0.94, 0, 0.245, 1);
    cylinder(0, 0.94, 0, 0.03, 0.60);
    pie(0, 0.54, 0, 0.15, -1);
    cylinder(0, 1.54, 0, .15, .10);
    pie(0, 1.64, 0, .15, 1);
    sphere(0, 1.84, 0, .20, 0, 180);
    if(hasCover)
    {
        sticks(0, 2.07, 0, .40);
        cover(0, 2.07, 0, .40, 38, 1.00);
    }

    DisableTex();
    glPopMatrix();
}


void Lamp::display()
{
    Draw(x0, y0, z0, 1, true);
}
