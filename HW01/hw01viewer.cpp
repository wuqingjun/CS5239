//
//  Hw01viewer Widget
//

#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "hw01viewer.h"
#include "hw01opengl.h"

//
//  Constructor
//
Hw01viewer::Hw01viewer()
{
    // Control slide positions of objects
    int maximumPosition = 10;  
    int defaultPosition = 0;
    int slideStep = 1;

   //  Set window title
   setWindowTitle(tr("Hw01: NDC to RGB Shader [D. Southard]"));

   //  Create new OpenGL widget
   Hw01opengl* hw01opengl = new Hw01opengl;

   //  Select shader
   QComboBox* shader = new QComboBox();
   shader->addItem("None");
   shader->addItem("NDC to RGB Shader");

   //  Select projection
   QComboBox* projection = new QComboBox();
   projection->addItem("Orthogonal");
   projection->addItem("Perspective");

   //  Select object
   QComboBox* object = new QComboBox();
   object->addItem("Cube");
   object->addItem("Sphere");
   object->addItem("Teapot");
   object->addItem("T-Rex");
   object->addItem("Lamp");
   
   // User control of X position
   QSlider* xSlider = new QSlider();
   xSlider->setMinimum(-maximumPosition);
   xSlider->setMaximum(maximumPosition);
   xSlider->setSliderPosition(defaultPosition);
   xSlider->setSingleStep(slideStep);
   xSlider->setOrientation(Qt::Horizontal);
   
   // User control of Y position
   QSlider* ySlider = new QSlider();
   ySlider->setMinimum(-maximumPosition);
   ySlider->setMaximum(maximumPosition);
   ySlider->setSliderPosition(defaultPosition);
   ySlider->setSingleStep(slideStep);
   ySlider->setOrientation(Qt::Horizontal);
   
   // User control of Z position
   QSlider* zSlider = new QSlider();
   zSlider->setMinimum(-maximumPosition);
   zSlider->setMaximum(maximumPosition);
   zSlider->setSliderPosition(defaultPosition);
   zSlider->setSingleStep(slideStep);
   zSlider->setOrientation(Qt::Horizontal);

   // User control of Scaling
   QSlider* sSlider = new QSlider();
   sSlider->setMinimum(-maximumPosition);
   sSlider->setMaximum(maximumPosition);
   sSlider->setSliderPosition(defaultPosition);
   sSlider->setSingleStep(slideStep);
   sSlider->setOrientation(Qt::Horizontal);
   
   //  Lighting
   QCheckBox* lighting = new QCheckBox("");

   //  View angle
   QLabel* angles = new QLabel();

   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(hw01opengl,0,0,9,1);
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
   layout->addWidget(new QLabel("Scale"), 6, 1);
   layout->addWidget(sSlider, 6, 2);
   layout->addWidget(new QLabel("Angles"), 7, 1);
   layout->addWidget(angles,7,2);
   layout->addWidget(quit,8,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(5,100);
   setLayout(layout);

   //  Connect valueChanged() signals to hw01opengl
   connect(shader,SIGNAL(currentIndexChanged(int))     , hw01opengl,SLOT(setShader(int)));
   connect(object,SIGNAL(currentIndexChanged(int))     , hw01opengl,SLOT(setObject(int)));
   connect(projection,SIGNAL(currentIndexChanged(int)) , hw01opengl,SLOT(setPerspective(int)));
   connect(lighting,SIGNAL(stateChanged(int))          , hw01opengl,SLOT(setLighting(int)));
   connect(xSlider, SIGNAL(valueChanged(int))         ,  hw01opengl,SLOT(setXPosition(int)));
   connect(ySlider, SIGNAL(valueChanged(int))         ,  hw01opengl,SLOT(setXPosition(int)));
   connect(zSlider, SIGNAL(valueChanged(int))         ,  hw01opengl,SLOT(setZPosition(int)));
   connect(sSlider, SIGNAL(valueChanged(int))         ,  hw01opengl,SLOT(setScale(int)));
   //  Connect angles() signal to label
   connect(hw01opengl,SIGNAL(angles(QString)) , angles,SLOT(setText(QString)));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}
