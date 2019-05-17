#ifndef NAVIGATIONTOOL_H
#define NAVIGATIONTOOL_H

#include "tool.h"

class NavigationTool : public Tool {

public:
  NavigationTool( GuiImage *guiImage, ImageViewer *viewer );
  ~NavigationTool( );
  enum { Type = 0 };

  /* Tool interface */
public:
  int type( );

  void mouseReleased( QPointF pt, Qt::MouseButtons buttons );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons );
  void mouseMoved( QPointF pt );
  void sliceChanged( size_t slice );
  void updateOverlay( QPointF pt );

private:
  /**
   *
   * @brief changeOthersSlices is called when the mouse is clicked over
   *  an 3d image view (axial, sagittal and coronal).
   * @param posF is the mouse pointer position in the scene.
   * @param view is the number of the view's axis.
   *
   */
  void changeOtherSlices( QPointF posF, size_t axis );

};

#endif /* NAVIGATIONTOOL_H */
