//
//  Generic Object class
//
#include "Object.h"

//
//  Constructor
//
Object::Object()
{
   //  Location
   x0 = y0 = z0 = 0;
   //  Rotation
   th = ny = nz = 0;
   nx = 1;
   //  Color
   r0 = g0 = b0 = 1;
   //  Texture
   tex = 0;
}

//
//  Object position
//
void Object::translate(float x,float y,float z)
{
   x0 = x;
   y0 = y;
   z0 = z;
}

//
//  Object rotation
//
void Object::rotate(float t,float dx,float dy,float dz)
{
   th = t;
   nx = dx;
   ny = dy;
   nz = dz;
}

//
//  Object rotation
//
void Object::scale(float dx,float dy,float dz)
{
   sx = dx;
   sy = dy;
   sz = dz;
}


//
//  Object color
//
void Object::color(float r,float g,float b)
{
   r0 = r;
   g0 = g;
   b0 = b;
}

//
//  Apply default color
//
void Object::setTransform(float dx,float dy,float dz)
{
   glTranslated(x0,y0,z0);
   glRotated(th,nx,ny,nz);
   glScaled(dx,dy,dz);
}

void Object::setTransform()
{
   glTranslated(x0,y0,z0);
   glRotated(th,nx,ny,nz);
   glScaled(sx,sy,sz);
}

//
//  Apply default color
//
void Object::setColor()
{
   setColor(Color(r0,g0,b0));
}

//
//  Apply ambient and diffuse color
//  Specular is set to white
//  Emission is set to black
//
void Object::setColor(Color c)
{
   setColor(c,c,Color(1,1,1),Color(0,0,0),16);
}

//
//  Apply colors
//
void Object::setColor(Color a,Color d,Color s,Color e,float Ns)
{
   glColor4fv(d.fv());
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT ,a.fv());
   glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE ,d.fv());
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,s.fv());
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,e.fv());
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&Ns);
}

//
//  Set texture
//
void Object::texture(QString file)
{
   glGenTextures(1,&tex);
   glBindTexture(GL_TEXTURE_2D,tex);
   QImage img(file);
   QImage rgba = QGLWidget::convertToGLFormat(img);
   glTexImage2D(GL_TEXTURE_2D,0,4,rgba.width(),rgba.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.bits());
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

//
//  Enable texture
//
void Object::EnableTex()
{
   glBindTexture(GL_TEXTURE_2D,tex);
   if (tex) glEnable(GL_TEXTURE_2D);
}

//
//  Disable texture
//
void Object::DisableTex()
{
   if (tex) glDisable(GL_TEXTURE_2D);
}
