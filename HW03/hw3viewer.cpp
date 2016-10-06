//
//  Hw3viewer Widget
//

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "hw3viewer.h"
#include <iostream>

using namespace std;

//
//  Constructor
//
Hw3viewer::Hw3viewer()
{
    //  Set window title
    setWindowTitle(tr("Josh Killinger:  HW3"));

    //  Create new Triangle widget
    ogl = new Hw3opengl;

    fps = new QLabel("FPS: ");

    //  Select shader
    QComboBox* shader = new QComboBox();
    shader->addItem("None");
    shader->addItem("Stored");
    shader->addItem("Vertex Blinn");
    shader->addItem("Vertex Blinn Inline");
    shader->addItem("Vertex Blinn Manual");
    shader->addItem("Vertex Phong");
    shader->addItem("Vertex Phong Inline");
    shader->addItem("Pixel Blinn");
    shader->addItem("Pixel Phong");

    //  Select projection
    QComboBox* projection = new QComboBox();
    projection->addItem("Orthogonal");
    projection->addItem("Perspective");
    projection->setCurrentIndex(1);

    //  Select object
    QComboBox* object = new QComboBox();
    object->addItem("Cube");
    object->addItem("Teapot");
    object->addItem("Cruiser");
    object->addItem("Sphere");
    object->addItem("Lamp");

    //  Center position
    Lpos = new QSlider(Qt::Horizontal);
    Zpos = new QSlider(Qt::Horizontal);
    Lpos->setRange(0,360);
    Zpos->setRange(-100,100);

    //  View angle and zoom
    QLabel* angles = new QLabel();

    //  Pause/resume button
    light = new QPushButton("Animate");

    //  Number of Vertices on Sphere
    QDoubleSpinBox* divs = new QDoubleSpinBox();
    divs->setRange(4,1000);
    divs->setValue(4);
    divs->setDecimals(0);
    divs->setSingleStep(1);
    verts = new QLabel();

    pixels = new QLabel();

    //  Reset
    QPushButton* rst = new QPushButton("Reset");
    //  Quit
    QPushButton* quit = new QPushButton("Quit");

    //  Set layout of child widgets
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(ogl,0,0,10,2);
    layout->addWidget(new QLabel("Shader"),0,2);
    layout->addWidget(shader,0,3);
    layout->addWidget(new QLabel("Projection"),1,2);
    layout->addWidget(projection,1,3);
    layout->addWidget(new QLabel("Object"),2,2);
    layout->addWidget(object,2,3);
    layout->addWidget(new QLabel("Light Position"),3,2);
    layout->addWidget(Lpos,3,3);
    layout->addWidget(new QLabel("Light"),4,2);
    layout->addWidget(light,4,3);
    layout->addWidget(new QLabel("Light Elevation"),5,2);
    layout->addWidget(Zpos,5,3);
    layout->addWidget(new QLabel("Angles"),6,2);
    layout->addWidget(angles,6,3);
    layout->addWidget(new QLabel("Divisions"),7,2);
    layout->addWidget(divs, 7, 3);
    layout->addWidget(new QLabel("Vertices:"),8,2);
    layout->addWidget(verts, 8, 3);
    layout->addWidget(fps,10,0);
    layout->addWidget(pixels,10,1);
    layout->addWidget(rst,10,2);
    layout->addWidget(quit,10,3);
    //  Manage resizing
    layout->setColumnStretch(0,50);
    layout->setColumnMinimumWidth(0,50);
    layout->setColumnStretch(1,50);
    layout->setColumnMinimumWidth(1,50);
    layout->setRowStretch(9,100);
    setLayout(layout);

    //  Connect valueChanged() signals to ogl
    connect(shader,SIGNAL(currentIndexChanged(int))     , ogl,SLOT(setShader(int)));
    connect(object,SIGNAL(currentIndexChanged(int))     , ogl,SLOT(setObject(int)));
    connect(projection,SIGNAL(currentIndexChanged(int)) , ogl,SLOT(setPerspective(int)));
    connect(Lpos,SIGNAL(valueChanged(int)) , ogl,SLOT(setPos(int)));
    connect(Zpos,SIGNAL(valueChanged(int)) , ogl,SLOT(setElev(int)));
    //  Connect angles() and zoom() signal to labels
    connect(ogl,SIGNAL(angles(QString)) , angles,SLOT(setText(QString)));
    connect(ogl,SIGNAL(light(int))      , Lpos,SLOT(setValue(int)));
    connect(ogl,SIGNAL(fps(QString))        , fps,SLOT(setText(QString)));
    connect(ogl,SIGNAL(pixels(QString))     , pixels,SLOT(setText(QString)));
    //  Connect reset() and lmove() signals
    connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
    connect(light,SIGNAL(pressed()),this,SLOT(lmove()));
    //  Connect quit() signal to qApp::quit()
    connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
    connect(divs,SIGNAL(valueChanged(double)), this, SLOT(divsChanged(double)));


    divsChanged(divs->value());
}

void Hw3viewer::divsChanged(double d)
{
    //cerr << "Divs changed" << endl;

    int v = d * 2;
    v = v * v;
    verts->setNum(v);
    ogl->setDivs((int)d);
}

//
//  Light pause/move
//
void Hw3viewer::lmove()
{
   bool pause = (light->text()=="Pause");
   if (pause)
      light->setText("Animate");
   else
      light->setText("Pause");
   ogl->setLightMove(!pause);
}
