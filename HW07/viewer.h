#ifndef EX13V_H
#define EX13V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "opengl.h"

class Viewer : public QWidget
{
Q_OBJECT
public:
    Viewer();
    QLabel*  flab;
    QSlider* frac;
private slots:
    void setShader(int mode);
};

#endif
