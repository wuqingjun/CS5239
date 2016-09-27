//
//  HW 01:  NDC to RGB Shader
//  Diana Southard
//  CSCI 4239/5239 Spring 2016
//

#include <QApplication>
#include "hw01viewer.h"

//
//  Main function
//
int main(int argc, char *argv[])
{
   //  Create the application
   QApplication app(argc,argv);
   //  Create and show view widget
   Hw01viewer view;
   view.show();
   //  Main loop for application
   return app.exec();
}
