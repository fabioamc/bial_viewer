#include "controller.h"
#include "navigationtool.h"
#include <QDebug>
#include <QFile>
#include <qsettings.h>

Controller::Controller( int views, QObject *parent ) :
  QObject( parent ), m_image( nullptr ), m_pixmapItem( new QGraphicsPixmapItem( ) ),
  m_labelItem( new QGraphicsPixmapItem( ) ), m_currentToolPos( 999 ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
}

Controller::~Controller( ) {
  BGN_CMT( "Begin.", 0 );
  if( m_currentToolPos != 999 ) {
    //qDeleteAll( tools );
    tools.clear( );
  }
  delete( m_image );
  delete( m_pixmapItem );
  delete( m_labelItem );
  END_CMT( "End.", 0 );
}

GuiImage* Controller::currentGuiImage( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_image );
}

bool Controller::hasGuiImage( ) {
  BGN_CMT( "Begin. Has image? " << ( m_image != nullptr ), 0 );
  END_CMT( "End.", 0 );
  return( m_image != nullptr );
}

bool Controller::addLabel( QString fname ) {
  BGN_CMT( "Begin. Loading file " << fname.toStdString( ), 0 );
  bool res = m_image->addLabel( fname );
  END_CMT( "End.", 0 );
  return( res );
}

GuiImage *Controller::loadImage( QString fname ) {
  BGN_CMT( "Begin. Loading file " << fname.toStdString( ), 0 );
  GuiImage *img = nullptr;
  try {
    COMMENT ( "Creating Gui Image.", 0 );
    img = new GuiImage( fname, this );
    if( img != nullptr ) {
      COMMENT( "Setting recent file.", 0 );
      setRecentImage( fname );
    }
    END_CMT( "End.", 0 );
    return( img );
  }
  catch( ... ) {
    BIAL_WARNING( "Could not load image. It does not exist or could not allocate in memory." );
    END_CMT( "End.", 0 );
    return( nullptr );
  }
}

void Controller::setImage( GuiImage *img ) {
  BGN_CMT( "Begin.", 0 );
  m_image = img;
  m_image->setHasLabel( false );
  COMMENT( "Initializing Navigation Tool, Slice Range and Overlay position.", 0 );
  emit initilizeImageViewer( );
  COMMENT( "Loaded. Updating image and label pixmap.", 0 );
  setPixMap( );
  connect( m_image, &GuiImage::pixmapUpdated, this, &Controller::updatePixMap );
  COMMENT( "Updating the elements displayed in the Main Window, both controllers and view.", 0 );
  emit updateMainWindowUIElements( );
  COMMENT( "emitting currentImageChanged.", 0 );
  emit currentImageChanged( );
  END_CMT( "End.", 0 );
}

bool Controller::isEmpty( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_image == nullptr );
}

void Controller::clearGuiImage( ) {
  BGN_CMT( "Begin. Deleting image.", 0 );
  delete( m_image );
  COMMENT( "Resetting GuiImage pointer.", 0 );
  m_image = nullptr;
  COMMENT( "Resetting current tool position.", 0 );
  m_currentToolPos = 999;
  END_CMT( "End.", 0 );
}

void Controller::setAxis( size_t axis ) {
  tools[ m_currentToolPos ]->setAxisNumber( axis );
}

void Controller::updatePixMap( ) {
  BGN_CMT( "Begin", 0 );
  setPixMap( );
  COMMENT( "emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End", 0 );
}

void Controller::setPixMap( ) {
  BGN_CMT( "Begin.", 0 );
  Tool *tool = tools[ m_currentToolPos ];
  COMMENT( "Getting current axis.", 0 );
  size_t axis = tool->axisNumber( );
  COMMENT( "Getting image pixmap and setting it to display.", 0 );
  const QPixmap &imgpix = m_image->getImageSlice( axis );
  m_pixmapItem->setPixmap( imgpix );
  if( m_image->hasLabel( ) ) {
    COMMENT( "Getting label pixmap and setting it to display.", 0 );
    const QPixmap &lblpix = m_image->getLabelSlice( axis );
    m_labelItem->setPixmap( lblpix );
  }
  else {
    COMMENT( "Does not have label.", 0 );
    m_labelItem->setPixmap( QPixmap( ) );
  }
  END_CMT( "End", 0 );
}

void Controller::setCurrentSlice( size_t view, size_t slice ) {
  BGN_CMT( "Begin.", 0 );
  m_image->setCurrentSlice( view, slice );
  END_CMT( "End.", 0 );
}

void Controller::rotate90( ) {
  BGN_CMT( "Begin.", 0 );
  m_image->rotate90( tools[ m_currentToolPos ]->axisNumber( ) );
  END_CMT( "End.", 0 );
}

void Controller::flipH( ) {
  BGN_CMT( "Begin.", 0 );
  m_image->flipH( tools[ m_currentToolPos ]->axisNumber( ) );
  END_CMT( "End.", 0 );
}

void Controller::flipV( ) {
  BGN_CMT( "Begin.", 0 );
  m_image->flipV( tools[ m_currentToolPos ]->axisNumber( ) );
  END_CMT( "End.", 0 );
}

void Controller::setRecentImage( QString &fname ) {
  BGN_CMT( "Begin.", 0 );
  COMMENT( "Setting recent file to : \"" << fname.toStdString( ) << "\"", 1 );
  QSettings settings;
  QStringList files = settings.value( "recentImageList" ).toStringList( );
  files.removeAll( fname );
  files.prepend( fname );
  while( files.size( ) > MaxRecentFiles ) {
    files.removeLast( );
  }
  settings.setValue( "recentImageList", files );
  COMMENT( "Emitting recentImageUpdated.", 0 );
  emit recentImagesUpdated( );
  END_CMT( "End.", 0 );
}

void Controller::setRecentLabel( QString &fname ) {
  BGN_CMT( "Begin.", 0 );
  COMMENT( "Setting recent file to : \"" << fname.toStdString( ) << "\"", 1 );
  QSettings settings;
  QStringList files = settings.value( "recentLabelList" ).toStringList( );
  files.removeAll( fname );
  files.prepend( fname );
  while( files.size( ) > MaxRecentFiles ) {
    files.removeLast( );
  }
  settings.setValue( "recentLabelList", files );
  COMMENT( "Emitting recentLabelUpdated.", 0 );
  emit recentLabelsUpdated( );
  END_CMT( "End.", 0 );
}

QGraphicsPixmapItem* Controller::getPixmapItem( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_pixmapItem );
}

QGraphicsPixmapItem* Controller::getLabelItem( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_labelItem );
}

Tool* Controller::currentTool( ) {
  BGN_CMT( "Begin.", 0 );
  if( m_currentToolPos == 999 ) {
    END_CMT( "End. Position 999. nullptr.", 0 );
    return( nullptr );
  }
  Tool *res = tools.at( m_currentToolPos );
  END_CMT( "End. Position " << m_currentToolPos, 0 );
  return( res );
}

size_t Controller::currentToolPos( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_currentToolPos );
}

void Controller::setCurrentToolPos( const size_t &currentToolPos ) {
  BGN_CMT( "Begin.", 0 );
  if( currentToolPos < static_cast< size_t >( tools.size( ) ) ) {
    currentTool( )->leave( );
    m_currentToolPos = currentToolPos;
    currentTool( )->enter( );
  }
  END_CMT( "End.", 0 );
}

void Controller::AppendTool( Tool *tool ) {
  BGN_CMT( "Begin.", 0 );
  tools.push_back( tool );
  m_currentToolPos = tools.size( ) - 1;
  END_CMT( "End. Appended to position " << m_currentToolPos, 0 );
}
