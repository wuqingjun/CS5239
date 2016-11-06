//
//  Viewer Widget
//

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "viewer.h"

//
//  Constructor
//
Viewer::Viewer()
{
   //  Set window title
   setWindowTitle(tr("Ex13:  Image Processing"));

   //  Create new OpenGL widget
   Opengl* ogl = new Opengl;

   //  Select shader
   QComboBox* shader = new QComboBox();
   shader->addItem("Original Image");
   shader->addItem("Transition");

   // Fraction
   flab = new QLabel("Fraction");
   frac = new QSlider(Qt::Horizontal);
   frac->setRange(0,100);
   flab->hide();
   frac->hide();

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,4,1);
   layout->addWidget(new QLabel("Shader"),0,1);
   layout->addWidget(shader,0,2);
   layout->addWidget(flab,1,1);
   layout->addWidget(frac,1,2);
   layout->addWidget(rst,3,1);
   layout->addWidget(quit,3,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(2,100);
   setLayout(layout);

   //  Connect valueChanged() signals to ogl
   connect(shader,SIGNAL(currentIndexChanged(int)) , ogl,SLOT(setShader(int)));
   connect(frac,SIGNAL(valueChanged(int))          , ogl,SLOT(setFrac(int)));
   connect(shader,SIGNAL(currentIndexChanged(int)) , this,SLOT(setShader(int)));
   //  Connect reset() and lmove() signals
   connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}

//
//  Shader adjust
//
void Viewer::setShader(int mode)
{
   if (mode==1)
   {
      flab->show();
      frac->show();
   }
   else
   {
      flab->hide();
      frac->hide();
   }
}
