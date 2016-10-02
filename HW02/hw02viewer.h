#ifndef EX03V_H
#define EX03V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw02opengl.h"

class Hw02viewer : public QWidget
{
Q_OBJECT
private:
   QSlider*        xSlider;
   QSlider*        ySlider;
   QSlider*        zSlider;
   QPushButton*    light;
   Hw02opengl*     ogl;
private slots:
   void reset();        //  Reset angles
   void lmove();        //  Pause/animate light
//   void izoom(int iz);  //  Zoom level (percent)
public:
    Hw02viewer();
};

#endif
