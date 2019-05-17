#ifndef TOOL_H
#define TOOL_H

#include "guiimage.h"
#include "imageviewer.h"

#include <QPointF>
#include <QString>

class Tool : public QObject {
  Q_OBJECT
protected:
  GuiImage *guiImage;
  ImageViewer *viewer;
  bool m_visible;

public:
  Tool( GuiImage *guiImage, ImageViewer *viewer );
  virtual int type( ) = 0;
  virtual void mouseReleased( QPointF pt, Qt::MouseButtons buttons ) = 0;
  virtual void mouseClicked( QPointF pt, Qt::MouseButtons buttons ) = 0;
  virtual void mouseDragged( QPointF pt, Qt::MouseButtons buttons ) = 0;
  virtual void mouseMoved( QPointF pt ) = 0;
  virtual void sliceChanged( size_t slice ) = 0;
  virtual void enter( );
  virtual void leave( );
  virtual QPixmap getLabel( );
  GuiImage* getGuiImage( ) const;
  void setAxisNumber( size_t axis );
  size_t axisNumber( ) const;
};

#endif /* TOOL_H */
