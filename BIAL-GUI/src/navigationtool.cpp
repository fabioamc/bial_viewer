#include "FileImage.hpp"
#include "controller.h"
#include "navigationtool.h"
#include "gdcm.h"
#include <QDebug>
#include <QPointF>
#include <QRgb>

NavigationTool::NavigationTool( GuiImage *guiImage, ImageViewer *viewer ) : Tool( guiImage, viewer ) { //, m_factor( 64.0 ), m_max( 0 ) {
  BGN_CMT( "Begin.", 0 );
  setObjectName( "NavigationTool" );
  END_CMT( "End.", 0 );
}

NavigationTool::~NavigationTool( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
}

int NavigationTool::type( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( NavigationTool::Type );
}

void NavigationTool::mouseReleased( QPointF pt, Qt::MouseButtons buttons ) {
  BGN_CMT( "Begin. Position: " << pt.x( ) << ", " << pt.y( ), 0 );
  if( buttons & Qt::LeftButton ) {
    changeOtherSlices( pt, viewer->axisNumber( ) );
    updateOverlay( pt );
  }
  END_CMT( "End.", 0 );
}

void NavigationTool::mouseClicked( QPointF pt, Qt::MouseButtons buttons ) {
  BGN_CMT( "Begin. Position: " << pt.x( ) << ", " << pt.y( ), 0 );
  if( buttons & Qt::LeftButton ) {
    changeOtherSlices( pt, viewer->axisNumber( ) );
    updateOverlay( pt );
  }
  END_CMT( "End.", 0 );
}

void NavigationTool::mouseDragged( QPointF pt, Qt::MouseButtons buttons ) {
  BGN_CMT( "Begin. point x: " << pt.x( ) << ", y: " << pt.y( ), 0 );
  if( buttons & Qt::LeftButton ) {
    changeOtherSlices( pt, viewer->axisNumber( ) );
    updateOverlay( pt );
  }
  END_CMT( "End.", 0 );
}

void NavigationTool::mouseMoved( QPointF pt ) {
  BGN_CMT( "Begin.", 0 );
  Q_UNUSED( pt )
  END_CMT( "End.", 0 );
}

void NavigationTool::changeOtherSlices( QPointF posF , size_t axis ) {
  BGN_CMT( "Begin. Current slice: " << axis << ". Position: " << posF.x( ) << ", " << posF.y( ), 0 );
  Bial::FastTransform transform = guiImage->getTransform( axis );
  Bial::Point3D pt = transform( static_cast< double >( posF.x( ) ), static_cast< double >( posF.y( ) ),
                                static_cast< double >( guiImage->currentSlice( axis ) ) );
  for( size_t other = 0; other < 3; ++other ) {
    if( other != axis ) {
      Bial::FastTransform otherTransf = guiImage->getTransform( other ).Inverse( );
      Bial::Point3D otherPt = otherTransf( pt );
      size_t pos = static_cast< size_t >( round( otherPt.z ) );
      if( pos < guiImage->depth( other ) ) {
        COMMENT( "Setting axis " << other << ". Slice: " << pos, 0 );
        guiImage->setCurrentSlice( other, pos );
      }
    }
  }
  END_CMT( "End.", 0 );
}

void NavigationTool::updateOverlay( QPointF pt ) {
  BGN_CMT( "Begin. Coordinates: " << pt.x( ) << ", " << pt.y( ), 0 );
  pt.setX( qMin( qMax( std::round( pt.x( ) ), 0.0 ),
                 static_cast< double >( guiImage->width( viewer->axisNumber( ) ) ) ) );
  pt.setY( qMin( qMax( std::round( pt.y( ) ), 0.0 ),
                 static_cast< double >( guiImage->heigth( viewer->axisNumber( ) ) ) ) );
  viewer->getScene( )->setOverlayPos( pt );
  END_CMT( "End.", 0 );
}

void NavigationTool::sliceChanged( size_t slice ) {
  BGN_CMT( "Begin", 0 );
  Q_UNUSED( slice );
  END_CMT( "End.", 0 );
}
