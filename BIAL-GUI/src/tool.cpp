#include "navigationtool.h"
#include "tool.h"

GuiImage* Tool::getGuiImage( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( guiImage );
}

void Tool::setAxisNumber( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  viewer->setAxisNumber( axis );
  END_CMT( "End.", 0 );
}

size_t Tool::axisNumber() const {
  BGN_CMT( "Begin.", 0 );
  size_t res = viewer->axisNumber( );
  END_CMT( "End.", 0 );
  return( res );
}

Tool::Tool( GuiImage *guiImage, ImageViewer *viewer ) : QObject( guiImage ), guiImage( guiImage ), viewer( viewer ),
  m_visible( false ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
}

void Tool::leave( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
}

void Tool::enter( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
}

QPixmap Tool::getLabel( ) {
  BGN_CMT( "Begin.", 0 );
  //Q_UNUSED( axis );
  END_CMT( "End.", 0 );
  return( QPixmap( ) );
}

