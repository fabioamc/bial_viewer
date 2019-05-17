#ifndef LABELSWIDGET_H
#define LABELSWIDGET_H

#include <QWidget>
#include "guiimage.h"

namespace Ui {
  class LabelsWidget;
}

class LabelsWidget : public QWidget {
  Q_OBJECT

public:
  explicit LabelsWidget( QWidget *parent = 0 );
  ~LabelsWidget( );
  void setGuiImage( GuiImage *value );
  /**
   * @brief needUpdate: Sets flags to update controls.
   */
  void needUpdate( );

private slots:
  /**
   * @brief on_translucentSlider_valueChanged: Slider to select translucence level.
   * @param value: level of translucence. Default: 127.
   */
  void on_translucentSlider_valueChanged( int value );
  /**
   * @brief on_clearLabelButton_clicked: Clears current label.
   */
  void on_clearLabelButton_clicked();
  /**
   * @brief on_no_label_clicked: Selected no label. Removes from canvas.
   */
  void on_no_label_clicked();
  /**
   * @brief on_solid_label_clicked: Label is solid and covers it.
   */
  void on_solid_label_clicked();
  /**
   * @brief on_border_label_clicked: selected border label.
   */
  void on_border_label_clicked();
  /**
   * @brief on_gray_label_clicked: selected gray level label.
   */
  void on_gray_label_clicked();
  /**
   * @brief on_color_label_clicked: Selected color label.
   */
  void on_color_label_clicked();
  /**
   * @brief on_label_select_comboBox_activated: Selecting a label to change its color.
   * @param index: Label number
   */
  void on_label_select_comboBox_activated( int index );
  /**
   * @brief on_color_select_buttom_clicked: pressed button to select new color of label.
   */
  void on_color_select_buttom_clicked( );

signals:
  void clearDock( );
  void updatePixMap( );

private:
  Ui::LabelsWidget *ui;
  GuiImage *image;
  bool need_update;
  /**
   * @brief setComboBox: Created combobox with the available labels.
   */
  void setComboBox( );
};

#endif /* LABELSWIDGET_H */
