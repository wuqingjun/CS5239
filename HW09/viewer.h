#ifndef EX19V_H
#define EX19V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "opengl.h"

class Viewer : public QWidget
{
Q_OBJECT
private:
   Opengl*  ogl;
public:
    Viewer();
};

#endif
