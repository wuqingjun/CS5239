//
//  OpenGL Ex13opengl Widget
//

#ifndef EX13OPENGL_H
#define EX13OPENGL_H

#include <QtOpenGL>
#include <QString>
#include <QVector>
#include <QImage>

class Opengl : public QGLWidget
{
Q_OBJECT
private:
   bool    init;         //  Initialized
   float   x0,y0;        //  Image center
   float   zoom;         //  Zoom
   int     w,h;          //  Image size
   int     frac;         //  Image fraction
   int     mode;         //  Display mode
   bool    mouse;        //  Mouse pressed
   QPoint  pos;          //  Mouse position
   QGLShaderProgram shader; //  Shaders
public:
   Opengl(QWidget* parent=0);                  //  Constructor
   QSize sizeHint() const {return QSize(400,400);} //  Default size of widget
public slots:
    void setShader(int sel);               //  Slot to set shader
    void setFrac(int pct);                 //  Slot to set fraction
    void reset();                          //  Reset view
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
   void Shader(QGLShaderProgram& shader,QString vert,QString frag);  //  Create shader
   void LoadImage(int unit,const QString file);  //  Load image to texture unit
};

#endif
