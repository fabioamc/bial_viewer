#include "graphicsscene.h"
#include "imagewidget.h"
#include "ui_imagewidget.h"
#include <Common.hpp>

ImageWidget::ImageWidget( QWidget *parent ) :
    QWidget( parent ), ui( new Ui::ImageWidget ), m_graphics_scene( new GraphicsScene( this ) ) {
  BGN_CMT( "Begin.", 0 );
  ui->setupUi( this );
  ui->graphicsView->setScene( m_graphics_scene );
  setViewBgColor( Qt::black );
  QPalette p( palette( ) );
  p.setColor( QPalette::Background, Qt::lightGray );
  setAutoFillBackground( true );
  setPalette( p );
  connect( ui->graphicsView, &GraphicsView::dropImage, this, &ImageWidget::dropImage );
  END_CMT( "End.", 0 );
}

ImageWidget::~ImageWidget( ) {
  BGN_CMT( "Begin.", 0 );
  delete ui;
  delete m_graphics_scene;
  END_CMT( "End.", 0 );
}

void ImageWidget::hideControls( ) {
  BGN_CMT( "Begin.", 0 );
  ui->frameControls->hide( );
  END_CMT( "End.", 0 );
}

void ImageWidget::showControls( ) {
  BGN_CMT( "Begin.", 0 );
  ui->frameControls->show( );
  END_CMT( "End.", 0 );
}

void ImageWidget::setViewBgColor( const QColor &color ) {
  BGN_CMT( "Begin.", 0 );
  if( ui->graphicsView->scene( ) ) {
    ui->graphicsView->scene( )->setBackgroundBrush( color );
  }
  END_CMT( "End.", 0 );
}

GraphicsScene* ImageWidget::scene( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_graphics_scene );
}

GraphicsView *ImageWidget::graphicsView( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( ui->graphicsView );
}

void ImageWidget::setSliceRange( int start, int end ) {
  BGN_CMT( "Begin.", 0 );
  if( start > end ) {
    throw std::invalid_argument( BIAL_ERROR( "start should be smaller than end" ) );
  }
  COMMENT( "Spinbox range.", 0 );
  if( ui->spinBox->minimum( ) != start )
    ui->spinBox->setMinimum( start );
  if( ui->spinBox->maximum( ) != end )
    ui->spinBox->setMaximum( end );
  COMMENT( "Slider range.", 0 );
  if( ui->horizontalSlider->minimum( ) != start )
    ui->horizontalSlider->setMinimum( start );
  if( ui->horizontalSlider->maximum( ) != end )
    ui->horizontalSlider->setMaximum( end );
  COMMENT( "start == end. Disabling slider and spin", 0 );
  if( start == end )
    ui->frameControls->hide();
  else
    ui->frameControls->show();
  END_CMT( "End.", 0 );
}

void ImageWidget::setSlice( int slice ) {
  BGN_CMT( "Begin. Setting slice " << slice, 0 );
  if( ui->spinBox->value( ) != slice ) {
    ui->spinBox->setValue( slice );
  }
  if( ui->horizontalSlider->value( ) != slice ) {
    ui->horizontalSlider->setValue( slice );
  }
  END_CMT( "End.", 0 );
}

void ImageWidget::show( ) {
  BGN_CMT( "Begin.", 0 );
  QWidget::show( );
  END_CMT( "End.", 0 );
}

void ImageWidget::on_spinBox_valueChanged( int position ) {
  BGN_CMT( "Begin.", 0 );
  COMMENT( "Emitting sliceChanged.", 0 );
  emit sliceChanged( position );
  END_CMT( "End.", 0 );
}

void ImageWidget::on_horizontalSlider_valueChanged( int position ) {
  BGN_CMT( "Begin.", 0 );
  COMMENT( "Emitting sliceChanged.", 0 );
  emit sliceChanged( position );
  END_CMT( "End.", 0 );
}

void ImageWidget::on_rotateButton_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  QPointF curr_pos( m_graphics_scene->overlayPos( ) );
  QPointF new_pos( m_graphics_scene->height( ) - curr_pos.y( ), curr_pos.x( ) );
  m_graphics_scene->setOverlayPos( new_pos );//, false );
  COMMENT( "Emitting rotate.", 0 );
  emit rotate( );
  //m_graphics_scene->setOverlayPos( new_pos );
  END_CMT( "End.", 0 );
}

void ImageWidget::on_flip_h_button_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  QPointF curr_pos( m_graphics_scene->overlayPos( ) );
  QPointF new_pos( m_graphics_scene->width( ) - curr_pos.x( ), curr_pos.y( ) );
  m_graphics_scene->setOverlayPos( new_pos );
  COMMENT( "Emitting fliph.", 0 );
  emit fliph( );
  END_CMT( "End.", 0 );
}

void ImageWidget::on_flip_v_button_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  QPointF curr_pos( m_graphics_scene->overlayPos( ) );
  QPointF new_pos( curr_pos.x( ), m_graphics_scene->height( ) - curr_pos.y( ) );
  m_graphics_scene->setOverlayPos( new_pos );
  COMMENT( "Emitting flipv.", 0 );
  emit flipv( );
  END_CMT( "End.", 0 );
}
