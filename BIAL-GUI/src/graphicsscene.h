#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class GraphicsScene : public QGraphicsScene {
  bool m_overlay;
  QPen m_overlayPen;
  QPointF m_overlayPos;

public:
  explicit GraphicsScene( QObject *parent = 0 );
  void changeOverlay( );
  void setOverlay( bool overlay );
  void setOverlayPen( const QPen &overlayPen );
  /**
   * @brief setOverlayPos: updates the position of the overlay. May also refresh GS in canvas.
   * @param pos: New position of overlay
   */
  void setOverlayPos( QPointF pos );
  bool overlay( ) const;
  QPointF overlayPos( ) const;

protected:
  void drawForeground( QPainter *painter, const QRectF &rect );
};

#endif /* GRAPHICSSCENE_H */
