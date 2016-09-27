//
//  Hw02viewer Widget
//

#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include "hw02viewer.h"

//
//  Constructor
//
Hw02viewer::Hw02viewer()
{
    // Control slide positions of objects
    int maximumPosition = 50;  
    int defaultPosition = 0;
    int slideStep = 1;
    
   //  Set window title
   setWindowTitle(tr("Hw02:  Procedural Textures Qingjun Wu"));

   //  Create new Triangle widget
   ogl = new Hw02opengl;

   //  Select shader
   QComboBox* shader = new QComboBox();
   shader->addItem("None");
   shader->addItem("Chess Board");
   //shader->addItem("Psychedelic");

   //  Select projection
   QComboBox* projection = new QComboBox();
   projection->addItem("Orthogonal");
   projection->addItem("Perspective");

   //  Select object
   QComboBox* object = new QComboBox();
   object->addItem("Cube");
   object->addItem("Teapot");
   object->addItem("Lamp");

   //  Center position
// User control of X position
   xSlider = new QSlider();
   xSlider->setMinimum(-maximumPosition);
   xSlider->setMaximum(maximumPosition);
   xSlider->setSliderPosition(defaultPosition);
   xSlider->setSingleStep(slideStep);
   xSlider->setOrientation(Qt::Horizontal);
   
   // User control of Y position
   ySlider = new QSlider();
   ySlider->setMinimum(-maximumPosition);
   ySlider->setMaximum(maximumPosition);
   ySlider->setSliderPosition(defaultPosition);
   ySlider->setSingleStep(slideStep);
   ySlider->setOrientation(Qt::Horizontal);
   
   // User control of Z position
   zSlider = new QSlider();
   zSlider->setMinimum(-maximumPosition);
   zSlider->setMaximum(maximumPosition);
   zSlider->setSliderPosition(defaultPosition);
   zSlider->setSingleStep(slideStep);
   zSlider->setOrientation(Qt::Horizontal);

   //  View angle and zoom
   QLabel* angles = new QLabel();
   QLabel* position = new QLabel();
   QLabel* zoom  = new QLabel();

   //  Pause/resume button
   light = new QPushButton("Pause");

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,12,1);
   layout->addWidget(new QLabel("Shader"),0,1);
   layout->addWidget(shader,0,2);
   layout->addWidget(new QLabel("Projection"),1,1);
   layout->addWidget(projection,1,2);
   layout->addWidget(new QLabel("Object"),2,1);
   layout->addWidget(object,2,2);
   layout->addWidget(new QLabel("X Position"),3,1);
   layout->addWidget(xSlider,3,2);
   layout->addWidget(new QLabel("Y Position"),4,1);
   layout->addWidget(ySlider,4,2);
   layout->addWidget(new QLabel("Z Position"),5,1);
   layout->addWidget(zSlider,5,2);
   layout->addWidget(new QLabel("Angles"),6,1);
   layout->addWidget(angles,6,2);
   layout->addWidget(new QLabel("Position"),7,1);
   layout->addWidget(position,7,2);
   layout->addWidget(new QLabel("Zoom"),8,1);
   layout->addWidget(zoom,8,2);
   layout->addWidget(new QLabel("Light"),9,1);
   layout->addWidget(light,9,2);
   layout->addWidget(rst,11,1);
   layout->addWidget(quit,11,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(9,100);
   setLayout(layout);

   //  Connect valueChanged() signals to ogl
   connect(shader,SIGNAL(currentIndexChanged(int))     , ogl,SLOT(setShader(int)));
   connect(object,SIGNAL(currentIndexChanged(int))     , ogl,SLOT(setObject(int)));
   connect(projection,SIGNAL(currentIndexChanged(int)) , ogl,SLOT(setPerspective(int)));
   connect(xSlider,SIGNAL(valueChanged(int)) , ogl,SLOT(setX(int)));
   connect(ySlider,SIGNAL(valueChanged(int)) , ogl,SLOT(setY(int)));
   connect(zSlider,SIGNAL(valueChanged(int)) , ogl,SLOT(setZ(int)));
   //  Connect angles() and zoom() signal to labels
   //  Connect angles() and zoom() signal to labels
   connect(ogl,SIGNAL(angles(QString)) , angles,SLOT(setText(QString)));
   connect(ogl,SIGNAL(position(QString)) , position,SLOT(setText(QString)));
   connect(ogl,SIGNAL(zoom(QString))   , zoom,SLOT(setText(QString)));
   //  Connect reset() and lmove() signals
   connect(rst  ,SIGNAL(pressed()),this,SLOT(reset()));
   connect(light,SIGNAL(pressed()),this,SLOT(lmove()));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}

//
//  Reset view
//
void Hw02viewer::reset()
{
   xSlider->setValue(0);
   ySlider->setValue(0);
   zSlider->setValue(0);
   ogl->reset();
}

//
//  Light pause/move
//
void Hw02viewer::lmove()
{
   bool pause = (light->text()=="Pause");
   if (pause)
      light->setText("Animate");
   else
      light->setText("Pause");
   ogl->setLightMove(!pause);
}
