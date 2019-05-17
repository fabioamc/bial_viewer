#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "graphicsscene.h"
#include <QTime>
#include <QWidget>
#include <array>

class ImageWidget;
class QGraphicsScene;
class Controller;

class ImageViewer : public QWidget {
  Q_OBJECT
  ImageWidget *img_widget;
  Controller *controller;
  bool dragging;
  QTime timer;
  size_t m_axisNumber;

public:
  explicit ImageViewer( QWidget *parent = 0 );
  ~ImageViewer( );

  /**
   *
   * @brief returns the GraphicScenes of one of the views, that is, a ImageWidget which controls one of the canvas. In this version, there is just one active canvas. GraphicScene controls the cross marker.
   *
   */
  GraphicsScene* getScene( );
  void setViewBgColor( const QColor &color );
  bool eventFilter( QObject *obj, QEvent *evt );
  void linkToController( Controller *value );
  size_t axisNumber( );
  void setAxisNumber( size_t viewNumber );
  /**
   * @brief overlayPosition: returns the overlay position.
   */
  QPointF overlayPosition( );
  void setOverlayPosition( size_t curr_axis, size_t new_axis );
  /**
   * @brief exportPixmap: Exports current pixmap to file.
   * @param fileName: Name of the file
   * @return true if exportation succeeded.
   */
  bool exportPixmap(QString fileName);
signals:
  void mouseReleased( QPointF pt, Qt::MouseButtons buttons );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons );
  void mouseMoved( QPointF pt );
  void dropImage( const QString &filePath );

private slots:
  void Initialize( );
  void changeImage( );
  void changePixmap( );
  void sliceChanged( size_t slice );
  void setGridLayout( );
protected:
  void resizeEvent( QResizeEvent* );
};

#endif /* IMAGEVIEWER_H */
