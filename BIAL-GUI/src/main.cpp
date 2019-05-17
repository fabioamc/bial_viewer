#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>

int main( int argc, char *argv[ ] ) {
  BGN_CMT( "Begin.", 0 );
  COMMENT( "Creating application.", 0 );
  QApplication app( argc, argv );
  app.setOrganizationName( "UNIFESP" );
  app.setApplicationName( "BIAL-view" );
  QCommandLineParser parser;
  parser.setApplicationDescription( "Simple Contour 3D" );
  parser.addHelpOption( );
  parser.addVersionOption( );
  parser.addPositionalArgument( "image", QCoreApplication::translate( "main", "Source image to be opened." ) );
  QCommandLineOption label( "l", QCoreApplication::translate( "main", "Input label." ),
                            QCoreApplication::translate( "main", "label" ) );
  parser.addOption( label );
  parser.process( app );
  MainWindow window;
  window.commandLineOpen( parser, label );
  window.showMaximized( );
  int res = app.exec( );
  END_CMT( "End.", 0 );
  return( res );
}
