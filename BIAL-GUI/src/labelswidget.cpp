#include "labelswidget.h"
#include "ui_labelswidget.h"
#include <QPalette>
#include <string>
#include <QColorDialog>

LabelsWidget::LabelsWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::LabelsWidget ), need_update( true ) {
  BGN_CMT( "Begin.", 0 );
  ui->setupUi( this );
  END_CMT( "End.", 0 );
}

LabelsWidget::~LabelsWidget( ) {
  BGN_CMT( "Begin.", 0 );
  delete ui;
  END_CMT( "End.", 0 );
}

void LabelsWidget::setGuiImage( GuiImage *value ) {
  BGN_CMT( "Begin.", 0 );
  if( ( value ) && ( value->hasLabel( ) ) ) {
    COMMENT( "Has label.", 0 );
    image = value;
    setEnabled( true );
    switch( image->getLabelDisplayType( ) ) {
    case LabelDisplayType::none:
      ui->no_label->setChecked( true );
      break;
    case LabelDisplayType::solid:
      ui->solid_label->setChecked( true );
      break;
    default: // LabelType::translucent:
      ui->border_label->setChecked( true );
      break;
    }
    switch ( image->getLabelDisplayColor( ) ) {
    case LabelDisplayColor::gray:
      ui->gray_label->setChecked( true );
      break;
    default: // LabelColor::color
      ui->color_label->setChecked( true );
      break;
    }
    int tf = static_cast< int >( image->getTranslucenceFactor( ) );
    ui->translucentSlider->setValue( tf );
    setComboBox( );
  }
  else {
    COMMENT( "Does not have labels.", 0 );
    setEnabled( false );
  }
  END_CMT( "End.", 0 );
}

void LabelsWidget::needUpdate( ) {
  need_update = true;
}

void LabelsWidget::setComboBox( ) {
  if( need_update ) {
    need_update = false;
    ui->label_select_comboBox->clear( );
    size_t size = image->labels( );
    for( size_t index = 0; index < size; ++index )
      ui->label_select_comboBox->addItem( QString( "Label " ) + QString::number( index ) );
    ui->label_select_comboBox->setCurrentIndex( 0 );
    QPalette pal = ui->color_select_buttom->palette( );
    pal.setColor( QPalette::Button, QColor( image->getLabelColor( 0 ) ) );
    ui->color_select_buttom->setPalette( pal );
    ui->color_select_buttom->update( );
  }
}

void LabelsWidget::on_translucentSlider_valueChanged( int value ) {
  BGN_CMT( "Begin. Value: " << value, 0 );
  image->setTranslucenceFactor( value );
  END_CMT( "End.", 0 );
}

void LabelsWidget::on_clearLabelButton_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  image->removeLabel( );
  END_CMT( "End.", 0 );
}

void LabelsWidget::on_no_label_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  image->setLabelDisplayType( LabelDisplayType::none );
  END_CMT( "End.", 0 );
}

void LabelsWidget::on_solid_label_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  image->setLabelDisplayType( LabelDisplayType::solid );
  END_CMT( "End.", 0 );
}

void LabelsWidget::on_border_label_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  image->setLabelDisplayType( LabelDisplayType::border );
  END_CMT( "End.", 0 );
}

void LabelsWidget::on_gray_label_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  image->setLabelDisplayColor( LabelDisplayColor::gray );
  END_CMT( "End.", 0 );
}

void LabelsWidget::on_color_label_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  image->setLabelDisplayColor( LabelDisplayColor::color );
  END_CMT( "End.", 0 );
}

void LabelsWidget::on_label_select_comboBox_activated( int index ) {
  BGN_CMT( "Begin.", 0 );
  QPalette pal = ui->color_select_buttom->palette( );
  pal.setColor( QPalette::Button, QColor( image->getLabelColor( static_cast< size_t >( index ) ) ) );
  ui->color_select_buttom->setPalette( pal );
  ui->color_select_buttom->update( );
  COMMENT( "Emitting on_color_label_clicked.", 0 );
  emit on_color_label_clicked( );
  END_CMT( "End.", 0 );
}

void LabelsWidget::on_color_select_buttom_clicked( ) {
  BGN_CMT( "Begin.", 0 );
  COMMENT( "Getting new color with dialog.", 0 );
  size_t index = static_cast< size_t >( ui->label_select_comboBox->currentIndex( ) );
  QColor current_color( image->getLabelColor( index ) );
  QColorDialog dialog( current_color );
  QColor new_color( dialog.getColor( ) );
  if( new_color.isValid( ) ) {
    COMMENT( "Setting new color to label.", 0 );
    image->setLabelColor( index, new_color.rgb( ) );
    QPalette pal = ui->color_select_buttom->palette( );
    pal.setColor( QPalette::Button, QColor( new_color ) );
    ui->color_select_buttom->setPalette( pal );
    ui->color_select_buttom->update( );
    COMMENT( "Emitting on_color_label_clicked.", 0 );
    emit on_color_label_clicked( );
    COMMENT( "Emitting updatePixMap( )", 0 );
    emit updatePixMap( );
  }
  END_CMT( "End.", 0 );
}

