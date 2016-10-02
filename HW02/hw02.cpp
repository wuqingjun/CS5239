//
//  Hw 02:  Procedural Textures
//  Diana Southard
//  CSCI 4239/5239 Spring 2016
//

#include <QApplication>
#include "hw02viewer.h"

//
//  Main function
//
int main(int argc, char *argv[])
{
   //  Create the application
   QApplication app(argc,argv);
   //  Create and show view widget
   Hw02viewer view;
   view.show();
   //  Main loop for application
   return app.exec();
}
