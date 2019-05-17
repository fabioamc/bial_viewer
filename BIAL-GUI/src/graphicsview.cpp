#include "Common.hpp"
#include "graphicsview.h"

#include <QApplication>
#include <QDebug>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QScrollBar>

GraphicsView::GraphicsView( QWidget *parent ) : QGraphicsView( parent ), fit( false ) {
  BGN_CMT( "Begin.", 0 );
  _pan = false;
  _space = false;
  _panStartX = 0;
  _panStartY = 0;
  END_CMT( "End.", 0 );
}

void GraphicsView::mousePressEvent( QMouseEvent *e ) {
  BGN_CMT( "Begin.", 0 );
  if( e->button( ) == Qt::MiddleButton ) {
    _pan = true;
    _panStartX = e->x( );
    _panStartY = e->y( );
    QApplication::setOverrideCursor( Qt::ClosedHandCursor );
    e->accept( );
    END_CMT( "End.", 0 );
    return;
  }
  QGraphicsView::mousePressEvent( e );
  END_CMT( "End.", 0 );
}

void GraphicsView::mouseReleaseEvent( QMouseEvent *e ) {
  BGN_CMT( "Begin.", 0 );
  if( e->button( ) == Qt::MiddleButton ) {
    _pan = false;
    QApplication::restoreOverrideCursor( );
    e->accept( );
    END_CMT( "End.", 0 );
    return;
  }
  QGraphicsView::mouseReleaseEvent( e );
  END_CMT( "End.", 0 );
}

void GraphicsView::mouseMoveEvent( QMouseEvent *e ) {
  BGN_CMT( "Begin.", 0 );
  if( _pan || _space ) {
    horizontalScrollBar( )->setValue( horizontalScrollBar( )->value( ) - ( e->x( ) - _panStartX ) );
    verticalScrollBar( )->setValue( verticalScrollBar( )->value( ) - ( e->y( ) - _panStartY ) );
    _panStartX = e->x( );
    _panStartY = e->y( );
    e->accept( );
    END_CMT( "End.", 0 );
    return;
  }
  _panStartX = e->x( );
  _panStartY = e->y( );
  QGraphicsView::mouseMoveEvent( e );
  END_CMT( "End.", 0 );
}

void GraphicsView::keyPressEvent( QKeyEvent *e ) {
  BGN_CMT( "Begin.", 0 );
  if( e->key( ) == Qt::Key_Space ) {
    _space = true;
    QApplication::setOverrideCursor( Qt::ClosedHandCursor );
    e->accept( );
  }
  QGraphicsView::keyPressEvent( e );
  END_CMT( "End.", 0 );
}

void GraphicsView::keyReleaseEvent( QKeyEvent *e ) {
  BGN_CMT( "Begin.", 0 );
  if( e->key( ) == Qt::Key_Space ) {
    _space = false;
    QApplication::restoreOverrideCursor( );
    e->accept( );
  }
  QGraphicsView::keyReleaseEvent( e );
  END_CMT( "End.", 0 );
}

void GraphicsView::dragEnterEvent( QDragEnterEvent *event ) {
  BGN_CMT( "Begin.", 0 );
  event->acceptProposedAction( );
  END_CMT( "End.", 0 );
}

void GraphicsView::dragMoveEvent( QDragMoveEvent *event ) {
  BGN_CMT( "Begin.", 0 );
  event->acceptProposedAction( );
  END_CMT( "End.", 0 );
}

void GraphicsView::dragLeaveEvent( QDragLeaveEvent *event ) {
  BGN_CMT( "Begin.", 0 );
  event->accept( );
  END_CMT( "End.", 0 );
}

void GraphicsView::dropEvent( QDropEvent *event ) {
  BGN_CMT( "Begin.", 0 );
  const QMimeData *mimeData = event->mimeData( );
  //qDebug( ) << "mime: " << mimeData->text( );
  if( mimeData->hasText( ) ) {
    QString file = mimeData->text( ).remove( "file://" ).remove( "\r\n" );
    //qDebug( ) << "text: " << file;
    QFileInfo info( file );
    //qDebug( ) << "file: " << info.isFile( );
    //qDebug( ) << "folder: " << info.isDir( );
    //qDebug( ) << "suffix: " << info.completeSuffix( );
    if( objectName( ) == "graphicsViewOutput" ) {
      COMMENT( "Emitting saveFile.", 0 );
      emit saveFile( event->mimeData( )->text( ) );
      END_CMT( "End.", 0 );
      return;
    }
    if( info.isFile( ) ) {
      emit dropImage( file );
    }
    else if( info.isDir( ) ) {
      //emit dropFolder( file );
    }
  }
  END_CMT( "End.", 0 );
}

void GraphicsView::resizeEvent( QResizeEvent *event ) {
  BGN_CMT( "Begin.", 0 );
  if( fit ) {
    fitInView( scene( )->itemsBoundingRect( ), Qt::KeepAspectRatio );
  }
  QGraphicsView::resizeEvent( event );
  END_CMT( "End.", 0 );
}

bool GraphicsView::getFit( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( fit );
}

void GraphicsView::setFit( bool value ) {
  BGN_CMT( "Begin.", 0 );
  fit = value;
  END_CMT( "End.", 0 );
}
