#include <QDebug>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGridLayout>
#include <iostream>

#include "Common.hpp"
#include "controller.h"
#include "navigationtool.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "imageviewer.h"
#include "imagewidget.h"

ImageViewer::ImageViewer( QWidget *parent ) :
    QWidget( parent ), img_widget( new ImageWidget( this ) ), controller( nullptr ), dragging( false ),
    m_axisNumber( 0 ) {
  BGN_CMT( "Begin.", 0 );
  img_widget->hideControls( );
  getScene( )->installEventFilter( this );
  connect( img_widget, &ImageWidget::dropImage, this, &ImageViewer::dropImage );
  QPalette p( palette( ) );
  p.setColor( QPalette::Background, Qt::black );
  setAutoFillBackground( true );
  setPalette( p );
  END_CMT( "End.", 0 );
}

ImageViewer::~ImageViewer( ) {
  BGN_CMT( "Begin.", 0 );
  delete img_widget;
  END_CMT( "End.", 0 );
}

void ImageViewer::setViewBgColor( const QColor &color ) {
  BGN_CMT( "Begin.", 0 );
  img_widget->setViewBgColor( color );
  END_CMT( "End.", 0 );
}

void ImageViewer::linkToController( Controller *value ) {
  BGN_CMT( "Begin.", 0 );
  controller = value;
  connect( controller, &Controller::currentImageChanged, this, &ImageViewer::changeImage );
  connect( controller, &Controller::pixmapUpdated, this, &ImageViewer::changePixmap );
  connect( controller, &Controller::initilizeImageViewer, this, &ImageViewer::Initialize );
  connect( img_widget, &ImageWidget::sliceChanged, this, &ImageViewer::sliceChanged );
  connect( img_widget, &ImageWidget::rotate, controller, &Controller::rotate90 );
  connect( img_widget, &ImageWidget::fliph, controller, &Controller::flipH );
  connect( img_widget, &ImageWidget::flipv, controller, &Controller::flipV );
  getScene( )->addItem( controller->getPixmapItem( m_axisNumber ) );
  getScene( )->addItem( controller->getLabelItem( m_axisNumber ) );
  END_CMT( "End.", 0 );
}

void ImageViewer::Initialize( ) {
  BGN_CMT( "Begin.", 0 );
  GuiImage *gi = controller->currentGuiImage( );
  COMMENT( "Creating Navigation Tool.", 0 );
  controller->AppendTool( new NavigationTool( gi, this ) );
  COMMENT( "Setting slice range", 0 );
  img_widget->setSliceRange( 0, gi->depth( m_axisNumber ) - 1 );
  COMMENT( "Initializing overlay position.", 0 );
  QPointF scnPos( gi->width( m_axisNumber ) / 2, gi->heigth( m_axisNumber ) / 2 );
  img_widget->scene( )->setOverlayPos( scnPos );
  END_CMT( "End.", 0 );
}

size_t ImageViewer::axisNumber( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_axisNumber );
}

void ImageViewer::setAxisNumber( size_t axisNumber ) {
  BGN_CMT( "Begin.", 0 );
  if( m_axisNumber != axisNumber ) {
    setOverlayPosition( m_axisNumber, axisNumber );
    m_axisNumber = axisNumber;
    //COMMENT( "Emitting changePixmap.", 0 );
    //emit changePixmap( );
  }
  END_CMT( "End.", 0 );
}

QPointF ImageViewer::overlayPosition( ) {
  return( img_widget->scene( )->overlayPos( ) );
}

void ImageViewer::setOverlayPosition( size_t curr_axis, size_t new_axis ) {
  BGN_CMT( "Begin.", 0 );
  GraphicsScene *gs = img_widget->scene( );
  GuiImage *gi = controller->currentGuiImage( );
  QPointF pt = gs->overlayPos( );
  COMMENT( "Updating overlay position.", 0 );
  gs->setOverlayPos( gi->getCoordinates( pt, curr_axis, new_axis ) );
  END_CMT( "End.", 0 );
}

void ImageViewer::changeImage( ) {
  BGN_CMT( "Begin", 0 );
  if( img_widget->size( ) != this->size( ) ) {
    COMMENT( "Resizing pixmap", 0 );
    img_widget->resize( this->size( ) );
  }
  changePixmap( );
  END_CMT( "End.", 0 );
}

void ImageViewer::changePixmap( ) {
  BGN_CMT( "Begin. Setting overlay position.", 0 );
  COMMENT( "Setting slice to IW.", 0 );
  GuiImage *gi = controller->currentGuiImage( );
  img_widget->setSlice( gi->currentSlice( m_axisNumber ) );
  COMMENT( "Resizing graphics view.", 0 );
  QRectF r = controller->getPixmapItem( m_axisNumber )->boundingRect( );
  getScene( )->setSceneRect( r );
  GraphicsView *gview = img_widget->graphicsView( );
  gview->fitInView( r, Qt::KeepAspectRatio );
  END_CMT( "End.", 0 );
}

void ImageViewer::setGridLayout( ) {
  BGN_CMT( "Begin.", 0 );
  changePixmap( );
  END_CMT( "End.", 0 );
}

void ImageViewer::sliceChanged( size_t slice ) {
  BGN_CMT( "Begin. Setting slice " << slice, 0 );
  controller->setCurrentSlice( m_axisNumber, slice );
  controller->currentTool( )->sliceChanged( slice );
  END_CMT( "End.", 0 );
}

void ImageViewer::resizeEvent( QResizeEvent* ) {
  BGN_CMT( "Begin.", 0 );
  if( controller->currentGuiImage( ) != nullptr ) {
    if( img_widget->size( ) != this->size( ) ) {
      COMMENT( "Resizing pixmap", 0 );
      img_widget->resize( this->size( ) );
    }
    changePixmap( );
  }
  END_CMT( "End.", 0 );
}

bool ImageViewer::eventFilter( QObject *obj, QEvent *evt ) {
  BGN_CMT( "Begin.", 0 );
  QGraphicsSceneMouseEvent *mouseEvt = dynamic_cast< QGraphicsSceneMouseEvent* >( evt );
  if( mouseEvt ) {
    Tool *tool = controller->currentTool( );
    QPointF scnPos = mouseEvt->scenePos( );
    scnPos.setX( std::round( scnPos.x( ) ) );
    scnPos.setY( std::round( scnPos.y( ) ) );
    if( mouseEvt->type( ) == QEvent::GraphicsSceneMouseMove ) {
      if( ( dragging ) && ( timer.elapsed( ) > 10 ) ) {
        timer.restart( );
        tool->mouseDragged( scnPos, mouseEvt->buttons( ) );
        emit mouseDragged( scnPos, mouseEvt->buttons( ) );
      }
      tool->mouseMoved( scnPos );
      emit mouseMoved( scnPos );
    }
    else if( mouseEvt->type( ) == QEvent::GraphicsSceneMousePress ) {
      if( mouseEvt->button( ) == Qt::LeftButton ) {
        dragging = true;
        timer.restart( );
      }
      tool->mouseClicked( scnPos, mouseEvt->buttons( ) );
      emit mouseClicked( scnPos, mouseEvt->buttons( ) );
    }
    else if( mouseEvt->type( ) == QEvent::GraphicsSceneMouseRelease ) {
      if( mouseEvt->button( ) == Qt::LeftButton ) {
        dragging = false;
      }
      tool->mouseReleased( scnPos, mouseEvt->buttons( ) );
      emit mouseReleased( scnPos, mouseEvt->buttons( ) );
    }
  }
  bool res = QWidget::eventFilter( obj, evt );
  END_CMT( "End.", 0 );
  return( res );
}

GraphicsScene* ImageViewer::getScene( ) {
  BGN_CMT( "Begin.", 0 );
  GraphicsScene* res = img_widget->scene( );
  END_CMT( "End.", 0 );
  return( res );
}

bool ImageViewer::exportPixmap( QString fileName ) {
  BGN_CMT( "Begin.", 0 );
  QPixmap pixMax = img_widget->graphicsView( )->grab( );
  bool res = pixMax.save( fileName );
  END_CMT( "End.", 0 );
  return( res );
}
