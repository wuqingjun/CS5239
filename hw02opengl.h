//
//  OpenGL Hw02opengl Widget
//

#ifndef EX03OPENGL_H
#define EX03OPENGL_H

#include <QtOpenGL>
#include <QString>
#include <QVector>
#include "Object.h"

class Hw02opengl : public QGLWidget
{
Q_OBJECT
private:
   bool    init;      //  Initialized
   bool    move;      //  Light animation
   float   zh;        //  Light position
   float   x0,y0;     //  Object center
   float   z0;        //  Zoom
   int     mode;      //  Display mode
   int     th,ph;     //  Display angles
   bool    mouse;     //  Mouse pressed
   QPoint  pos;       //  Mouse position
   int     fov;       //  Field of view
   double  dim;       //  Display size
   double  asp;       //  Sceen aspect ratio
   Object* obj;       //  Object
   QGLShaderProgram shader[4]; //  Shaders
   QVector<Object*> objects;   //  Objects
   QTimer           timer;     //  Timer for animations
   QElapsedTimer    time;      //  Track elapsed time
public:
   Hw02opengl(QWidget* parent=0);                  //  Constructor
   QSize sizeHint() const {return QSize(400,400);} //  Default size of widget
public slots:
    void setShader(int sel);               //  Slot to set shader
    void setPerspective(int on);           //  Slot to set projection type
    void setObject(int type);              //  Slot to set displayed object
    void setX(int X);                   //  Slot to set X position
    void setY(int Y);                   //  Slot to set Y position
    void setZ(int Z);                   //  Slot to set Z position
    void setLightMove(bool on);            //  Slot to set light movement
    void reset();                          //  Reset view
signals:
    void angles(QString text);             //  Signal for view angles
    void position(QString text);               //  Signal for zoom
    void zoom(QString text);               //  Signal for zoom
protected:
    void initializeGL();                   //  Initialize widget
    void resizeGL(int width, int height);  //  Resize widget
    void paintGL();                        //  Draw widget
    void mousePressEvent(QMouseEvent*);    //  Mouse pressed
    void mouseReleaseEvent(QMouseEvent*);  //  Mouse released
    void mouseMoveEvent(QMouseEvent*);     //  Mouse moved
    void wheelEvent(QWheelEvent*);         //  Mouse wheel
private:
   void Fatal(QString message);            //  Error handler
   void Projection();                      //  Update projection
   void Shader(int k,QString vert,QString frag);  //  Create shader
};

#endif
