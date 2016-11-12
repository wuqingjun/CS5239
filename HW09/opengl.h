//
//  OpenGL Ex19opengl Widget
//

#ifndef EX19OPENGL_H
#define EX19OPENGL_H

#include <QtOpenGL>
#include <QString>
#include <QVector>

class Opengl : public QGLWidget
{
Q_OBJECT
private:
   int     N;         //  Particle count
   bool    init;      //  Initialized
   bool    move;      //  Light animation
   float   zh;        //  Light position
   float   x0,y0;     //  Object center
   float   z0;        //  Zoom
   int     n;         //  Particle count
   int     mode;      //  Display mode
   int     th,ph;     //  Display angles
   bool    mouse;     //  Mouse pressed
   QPoint  pos;       //  Mouse position
   int     fov;       //  Field of view
   double  dim;       //  Display size
   double  asp;       //  Sceen aspect ratio
   float*  Vert;      //  Vertices
   float*  Color;     //  Colors
   float*  Vel;       //  Velocities
   float*  Start;     //  Start time

   QGLShaderProgram shader[2]; //  Shaders
   QTimer           timer;     //  Timer for animations
   QElapsedTimer    time;      //  Track elapsed time
public:
   Opengl(QWidget* parent=0);                  //  Constructor
   QSize sizeHint() const {return QSize(400,400);} //  Default size of widget
public slots:
    void setShader(int sel);               //  Slot to set shader
    void setPerspective(int on);           //  Slot to set projection type
    void reset();                          //  Reset view
signals:
    void angles(QString text);             //  Signal for view angles
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
   void Shader(QGLShaderProgram& shader,QString vert,QString frag);  //  Create shader
   void InitRain();
   void InitSmoke();
   void DrawPart();
};

#endif
