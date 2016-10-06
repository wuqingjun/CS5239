//
//  OpenGL Hw3opengl Widget
//

#ifndef EX04OPENGL_H
#define EX04OPENGL_H

#include <QtOpenGL>
#include <QString>
#include <QVector>
#include "Object.h"
#include "Sphere.h"

class Hw3opengl : public QGLWidget
{
Q_OBJECT
private:
    bool    init;       //  Initialized
    bool    move;       //  Light animation
    float   zh;         //  Light position
    float   x0,y0;      //  Object center
    float   z0;         //  Zoom
    int     mode;       //  Display mode
    int     th,ph;      //  Display angles
    bool    mouse;      //  Mouse pressed
    QPoint  pos;        //  Mouse position
    int     fov;        //  Field of view
    double  dim;        //  Display size
    double  asp;        //  Sceen aspect ratio
    int     framecount; //  number of frames so far this second
    int     divs;       //  used for building spheres
    Sphere* sphere;
    Object* obj;        //  Object
    QGLShaderProgram shader[9]; //  Shaders
    QVector<Object*> objects;   //  Objects
    QTimer           timer;     //  Timer for animations
    QTimer           frameTimer;//  Timer for frame count
    QElapsedTimer    time;      //  Track elapsed time
    QElapsedTimer    frameDuration;
    int     lastframe;
public:
    Hw3opengl(QWidget* parent=0);                  //  Constructor
    QSize sizeHint() const {return QSize(400,400);} //  Default size of widget
    void setDivs(int d);
public slots:
    void setShader(int sel);               //  Slot to set shader
    void setPerspective(int on);           //  Slot to set projection type
    void setObject(int type);              //  Slot to set displayed object
    void setPos(int ang);                  //  Slot to light position
    void setElev(int z);                   //  Slot to light elevation (%)
    void setLightMove(bool on);            //  Slot to set light movement
    void reset();                          //  Reset view
    void frameTimerTicked();               //  Frame Timer fired
signals:
    void angles(QString text);             //  Signal for view angles
    void light(int);                       //  Light position
    void fps(QString);
    void pixels(QString);
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
