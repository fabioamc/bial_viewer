#include "controller.h"
#include "controlswidget.h"
#include "imageviewer.h"
#include "ui_controlswidget.h"
#include <QTimer>

ControlsWidget::ControlsWidget( Controller *controller, QWidget *parent ) :
  QWidget( parent ), ui( new Ui::ControlsWidget ), controller( controller ) {
  BGN_CMT( "Begin.", 0 );
  ui->setupUi( this );
  ui->horizontalSliderBrightness->setEnabled( true );
  ui->horizontalSliderContrast->setEnabled( true );
  connect( controller, &Controller::currentImageChanged, this, &ControlsWidget::setBrightnessContrast );
  connect( controller, &Controller::pixmapUpdated, this, &ControlsWidget::updateBrightnessContrastSliders );
  END_CMT( "End.", 0 );
}

ControlsWidget::~ControlsWidget( ) {
  BGN_CMT( "Begin.", 0 );
  delete ui;
  END_CMT( "End.", 0 );
}

void ControlsWidget::setBrightnessContrast( ) {
  BGN_CMT( "Begin.", 0 );
  GuiImage *img = controller->currentGuiImage( );
  ui->horizontalSliderBrightness->setValue( img->getBrightness( ) );
  ui->horizontalSliderContrast->setValue( img->getContrast( ) );
  END_CMT( "End.", 0 );
}

void ControlsWidget::updateBrightnessContrastSliders( ) {
  BGN_CMT( "Begin.", 0 );
  GuiImage *img = controller->currentGuiImage( );
  COMMENT( "Setting sliders.", 0 );
  int brightness = img->getBrightness( );
  if( ui->horizontalSliderBrightness->value( ) != brightness )
    ui->horizontalSliderBrightness->setValue( brightness );
  int contrast = img->getContrast( );
  if( ui->horizontalSliderContrast->value( ) != contrast )
    ui->horizontalSliderContrast->setValue( contrast );
  END_CMT( "End.", 0 );
}

void ControlsWidget::updateUIAxisElements( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  if( axis == 0 ) {
    ui->pushButtonAxial->setChecked( true );
    ui->pushButtonCoronal->setChecked( false );
    ui->pushButtonSagittal->setChecked( false );
  }
  else if( axis == 1 ) {
    ui->pushButtonAxial->setChecked( false );
    ui->pushButtonCoronal->setChecked( true );
    ui->pushButtonSagittal->setChecked( false );
  }
  else { //if( axis == 2 ) {
    ui->pushButtonAxial->setChecked( false );
    ui->pushButtonCoronal->setChecked( false );
    ui->pushButtonSagittal->setChecked( true );
  }
  END_CMT( "End.", 0 );
}

void ControlsWidget::on_pushButtonAxial_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  controller->setAxis( 0 );
  COMMENT( "Emitting controller->updatePixMap.", 0 );
  emit( controller->updatePixMap( ) );
  END_CMT( "End.", 0 );
}

void ControlsWidget::on_pushButtonCoronal_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  controller->setAxis( 1 );
  COMMENT( "Emitting controller->updatePixMap.", 0 );
  emit( controller->updatePixMap( ) );
  END_CMT( "End.", 0 );
}

void ControlsWidget::on_pushButtonSagittal_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  controller->setAxis( 2 );
  COMMENT( "Emitting controller->updatePixMap.", 0 );
  emit( controller->updatePixMap( ) );
  END_CMT( "End.", 0 );
}

void ControlsWidget::on_horizontalSliderContrast_valueChanged( int value ) {
  BGN_CMT( "Begin.", 0 );
  controller->currentGuiImage( )->setContrast( value );
  END_CMT( "End.", 0 );
}

void ControlsWidget::on_horizontalSliderBrightness_valueChanged( int value ) {
  BGN_CMT( "Begin.", 0 );
  controller->currentGuiImage( )->setBrightness( value );
  END_CMT( "End.", 0 );
}

void ControlsWidget::on_resetContrast_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  controller->currentGuiImage( )->setContrast( 0 );
  END_CMT( "End.", 0 );
}

void ControlsWidget::on_resetBrightness_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  controller->currentGuiImage( )->setBrightness( 0 );
  END_CMT( "End.", 0 );
}
