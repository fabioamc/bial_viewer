#ifndef CONTROLSWIDGET_H
#define CONTROLSWIDGET_H

#include <QWidget>

namespace Ui {
  class ControlsWidget;
}

class ImageViewer;
class Controller;

class ControlsWidget : public QWidget {
  Q_OBJECT

public:
  explicit ControlsWidget( Controller *controller, QWidget *parent = 0 );
  ~ControlsWidget( );
  /**
   * @brief updateUIAxisElements: updates the buttons refering to axis.
   * @param axis: the axis to be checked.
   */
  void updateUIAxisElements( size_t axis );
private slots:
  void setBrightnessContrast( );
  void updateBrightnessContrastSliders( );
  void on_pushButtonAxial_clicked( );
  void on_pushButtonCoronal_clicked( );
  void on_pushButtonSagittal_clicked( );
  void on_horizontalSliderContrast_valueChanged( int value );
  void on_horizontalSliderBrightness_valueChanged( int value );
  void on_resetContrast_clicked( );
  void on_resetBrightness_clicked( );

private:
  Ui::ControlsWidget *ui;
  Controller *controller;
  QTimer *timer;
};

#endif /* CONTROLSWIDGET_H */
