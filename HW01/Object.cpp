//
//  Generic Object class
//
#include "Object.h"

//
//  Constructor
//
Object::Object(float x,float y,float z)
{
   x0 = x;
   y0 = y;
   z0 = z;
   s = 1;
}

//
//  Set translation
//
void Object::translate(float x,float y,float z)
{
   x0 = x;
   y0 = y;
   z0 = z;
}

void Object::scale(float S)
{
    s = S;
}

//
//  Get current X position
//
float Object::getX()
{
   return x0;
}

//
//  Get current Y position
//
float Object::getY()
{
   return y0;
}

//
//  Get current Z position
//
float Object::getZ()
{
   return z0;
}
