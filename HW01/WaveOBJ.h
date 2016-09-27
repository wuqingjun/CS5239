//
//  Wave OBJ class
//  The constructor places the object at the origin
//  All parameters are assigned default values
//
#ifndef WAVEOBJ_H
#define WAVEOBJ_H

#include "Object.h"
#include <QString>

class WaveOBJ: public Object
{
private:
   float s;               // Scale
   float R,G,B;           // Color
   float th,dx,dy,dz;     // Rotation (angle and axis)
   int   list;            // Display list
public:
   WaveOBJ(const char* file);                         //  Constructor
   void color(float R,float G,float B);               //  Set color
   void scale(float s);                               //  Set scale
   void rotate(float th,float dx,float dy,float dz);  //  Set rotation
   void display();                                    //  Render the object
};

#endif
