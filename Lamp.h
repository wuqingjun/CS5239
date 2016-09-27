#ifndef LAMP_H
#define LAMP_H
#include "Object.h"

//Color colors3[7] = {Color(1.0, 1.0, 1.0, 1.0), Color(0.8, 0.8, 0.8, 1.0), Color(0.0, 0.4, 0.3, 1.0), Color(0.8, 0.8, 0.8, 1.0), Color(0, 0.8, 1.0, 1.0), Color(1.0, 0.7, 0.9, 1.0), Color(0.7, 0.8, 0.9, 1.0)};
//Color colors1[7] = {Color(1.0, 1.0, 1.0, 1.0), Color(0.0, 0.8, 0.8, 1.0), Color(0.0, 0.1, 0.3, 1.0), Color(0.8, 0.3, 0.8, 1.0), Color(0.3, 0.8, 1.0, 1.0), Color(0.9, 0.7, 0.9, 1.0), Color(0.7, 0.3, 0.9, 1.0)};
//Color colors2[7] = {Color(1.0, 1.0, 1.0, 1.0), Color(0.5, 0.7, 0.8, 1.0), Color(0.0, 0.1, 0.3, 1.0), Color(0.8, 0.9, 0.1, 1.0), Color(0, 0.8, 1.0, 1.0), Color(0.8, 0.7, 0.9, 1.0), Color(0.9, 0.9, 0.9, 1.0)};


class Lamp: public Object
{
private:
    const float PI = 3.14159265;
private:
    float R,G,B;        // Color
    float th,dx,dy,dz;  //  Rotation (angle and axis)
public:
    Lamp();
    void Draw(float x, float y, float z, float s, bool hasCover);
    void scale(float S);  //  Set scale
    void display();
    void square(double x, double y, double z, double l, double w);
    void cover(double x, double y, double z, double r, double angle, double h);
    void sticks(double x, double y, double z, double r);
    void sphere(double x, double y, double z, double r, int startangle, int endangle);
    void cylinder(double x, double y, double z, double r, double h);
    void pie(double x, double y, double z, double r, int ynormal);
    double Sin(double th);
    double Cos(double th);
    double Tan(double th);
};

#endif // LAMP_H
