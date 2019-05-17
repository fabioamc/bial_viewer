#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

namespace Ui {
  class ImageWidget;
}

class GraphicsScene;
class GraphicsView;

class ImageWidget : public QWidget {
  Q_OBJECT

public:
  explicit ImageWidget( QWidget *parent = 0 );
  ~ImageWidget( );
  void hideControls( );
  void showControls( );
  void setViewBgColor( const QColor &color );
  GraphicsScene* scene( ) const;
  GraphicsView* graphicsView( );
  void setSliceRange( int start, int end );
  void setSlice( int slice );
  void show( );

signals:
  void sliceChanged( size_t slice );
  void rotate( );
  void fliph( );
  void flipv( );
  void dropImage( const QString &filePath );

private slots:
  void on_spinBox_valueChanged( int position );
  void on_rotateButton_clicked( );
  void on_horizontalSlider_valueChanged( int position );
  void on_flip_h_button_clicked( );
  void on_flip_v_button_clicked( );

private:
  Ui::ImageWidget *ui;
  GraphicsScene *m_graphics_scene;
};

#endif /* IMAGEWIDGET_H */
