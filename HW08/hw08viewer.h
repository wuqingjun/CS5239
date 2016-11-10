#ifndef EX17V_H
#define EX17V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw08opengl.h"

class Ex17viewer : public QWidget
{
Q_OBJECT
private:
   QSlider*     Lpos;
   QSlider*     Zpos;
   QPushButton* light;
   Ex17opengl*  ogl;
private slots:
   void lmove();        //  Light movement
public:
    Ex17viewer();
};

#endif
