//
//  HW07, Wade Wu

#include <QApplication>
#include "viewer.h"

//
//  Main function
//
int main(int argc, char *argv[])
{
   //  Create the application
   QApplication app(argc,argv);
   //  Create and show view widget
   Viewer view;
   view.show();
   //  Main loop for application
   return app.exec();
}
