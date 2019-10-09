#include <QApplication>
#include <QSurfaceFormat>

#include "MainWindow.h"
 
int main( int argc, char** argv )
{
  // needed to ensure appropriate OpenGL context is created for VTK rendering.
  //QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

  // QT Stuff
  QApplication app( argc, argv );
 
  MainWindow w;
  w.show();
 
  return app.exec();
}
