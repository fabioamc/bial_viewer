#include "Common.hpp"
#include "graphicsscene.h"
#include <QPainter>

QPointF GraphicsScene::overlayPos( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_overlayPos );
}

GraphicsScene::GraphicsScene( QObject *parent ) : QGraphicsScene( parent ) {
  BGN_CMT( "Begin.", 0 );
  m_overlay = false;
  m_overlayPen = QPen( Qt::green );
  m_overlayPos = QPointF( -1, -1 );
  END_CMT( "End.", 0 );
}

void GraphicsScene::changeOverlay( ) {
  BGN_CMT( "Begin.", 0 );
  m_overlay = !m_overlay;
  END_CMT( "End.", 0 );
}

void GraphicsScene::setOverlayPen( const QPen &overlayPen ) {
  BGN_CMT( "Begin.", 0 );
  if( m_overlayPen != overlayPen ) {
    m_overlayPen = overlayPen;
  }
  END_CMT( "End.", 0 );
}

void GraphicsScene::setOverlayPos( QPointF pos ) {
  BGN_CMT( "Begin. New position: " << pos.x( ) << ", " << pos.y( ), 0 );
  m_overlayPos = QPointF( std::round( pos.x( ) ), std::round( pos.y( ) ) ); // Is this needed?
  END_CMT( "End.", 0 );
}

bool GraphicsScene::overlay( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_overlay );
}

void GraphicsScene::setOverlay( bool overlay ) {
  BGN_CMT( "Begin.", 0 );
  if( m_overlay != overlay ) {
    m_overlay = overlay;
  }
  END_CMT( "End.", 0 );
}

void GraphicsScene::drawForeground( QPainter *painter, const QRectF &rect ) {
  BGN_CMT( "Begin.", 0 );
  if( m_overlay ) {
    COMMENT( "Coordinates: " << m_overlayPos.x( ) << ", " << m_overlayPos.y( ), 0 );
    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( m_overlayPen );
    painter->drawLine( m_overlayPos.x( ), 0, m_overlayPos.x( ), height( ) ); /* vertical */
    painter->drawLine( 0, m_overlayPos.y( ), width( ), m_overlayPos.y( ) ); /* horizontal */
  }
  QGraphicsScene::drawForeground( painter, rect );
  END_CMT( "End.", 0 );
}
