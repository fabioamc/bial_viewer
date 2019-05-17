#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "guiimage.h"
#include "tool.h"
#include <QGraphicsPixmapItem>
#include <QObject>

class Tool;

/**
 * @brief The Controller class is one of the most important classes of the User Interface,
 * and is responsible to managing the images.
 */
class Controller : public QObject {
  Q_OBJECT
  /**
   * @brief m_image holds opened image.
   */
  GuiImage* m_image;
  /**
   * @brief m_pixmapItems and m_labelItems.
   *        These objects hold two pixmaps, one for the current image and one for the current label.
   */
  QGraphicsPixmapItem* m_pixmapItem;
  QGraphicsPixmapItem* m_labelItem;
  /**
   * @brief scale
   */
  double scale;
  /**
   * @brief tools is a vector containing the image tools.
   */
  Bial::Vector< Tool* > tools;
  size_t m_currentToolPos;

public:
  enum { MaxRecentFiles = 10 };
  /**
   *
   * @brief Controller's constructor
   * @param views is the number of the views of the imageViewer.
   * @param parent is the parent object.
   *
   */
  explicit Controller( int views, QObject *parent = 0 );
  /**
   *
   * @brief Controller's destructor
   *
   */
  ~Controller( );
  /**
   *
   * @brief currentImage
   * @return A pointer to the current guiimage.
   *
   */
  GuiImage* currentGuiImage( );
  /**
   * @brief hasImage
   * @return true if a GuiImage was loaded.
   */
  bool hasGuiImage( );
  /**
   *
   * @brief isEmpty
   * @return true if there is no image
   *
   */
  bool isEmpty( );
  /**
   * @brief getPixmapItem returns the PixmapItem of the view.
   * @param view is the number of the view;
   * @return
   */
  QGraphicsPixmapItem* getPixmapItem( size_t axis );
  /**
   * @brief getLabelItem returns the LabelItem of the view.
   * @param view is the number of the view;
   * @return
   */
  QGraphicsPixmapItem* getLabelItem( size_t axis );
  
  /**
   * @brief currentTool returns the current Tool.
   * @return
   */
  void setTool( int toolType );
  Tool* currentTool( );
  size_t currentToolPos( ) const;
  void setCurrentToolPos( const size_t &currentToolPos );
  void AppendTool( Tool *tool );
  /**
   * @brief loadImage: Opens image. Exception robust function.
   * @param fname is the file name of the Image to be opened.
   */
  GuiImage *loadImage( QString fname );
  /**
   * @brief addLabel: Called to add label to current image.
   * @param fname: name of the file.
   * @return true if the label was correctly added.
   */
  bool addLabel(QString fname);
  /**
   * @brief setImage: Sets an image to m_image.
   * @param img: the Image to be set.
   */
  void setImage( GuiImage *img );
  /**
   * @brief setRecentLabel: Update settings with last opened label file name.
   * @param fname: Name of the label file
   */
  void setRecentLabel( QString &fname );
  /**
   * @brief clear Clears the image vector, and resets thumbnails.
   */
  void clearGuiImage( );
  /**
   * @brief setAxis changes current axis of ImageViewer.
   */
  void setAxis( size_t axis );

signals:
  /**
   * @brief setSliceRange: Initializing Navigation Tool, Widgets, and Overlay position.
   */
  void initilizeImageViewer( );
  /**
   * @brief This signal is emmited every time the current image changes.
   */
  void currentImageChanged( );
  /**
   * @brief This signal is emmited every time the current image is updated.
   */
  void pixmapUpdated( );
  /**
   * @brief This signal is emmited avery time the m_image is updated.
   */
  void updateMainWindowUIElements( );
  /**
   * @brief recentImagesUpdated: called after inserting new item to recent images list
   */
  void recentImagesUpdated( );
  /**
   * @brief recentLabelsUpdated: called after inserting new item to recent labels list
   */
  void recentLabelsUpdated( );
public slots:
  /**
   *
   * @brief update the pixmaps containts and refreshes the view.
   *
   */
  void updatePixMap();
  /**
   *
   * @brief Just sets new pixmaps, but do not update its containts in the view.
   *
   */
  void setPixMap();
  /**
   * @brief setCurrentSlice is called by the imageViewer when the slider or
   *  the spinbox have theis values updated.
   * @param view
   * @param slice
   */
  void setCurrentSlice( size_t view, size_t slice );
  /**
   * @brief rotate90 rotates a view in 90 degrees.
   * @param view View number
   */
  void rotate90( );
  /**
   * @brief flipH mirrors the current view on X axis.
   * @param view View number
   */
  void flipH( );
  /**
   * @brief flipV mirrors the current view on Y axis.
   * @param view View number
   */
  void flipV( );
  
private:
  /**
   * @brief setRecentFile: Update settings with last opened image file name.
   * @param fname: Name of the image file
   */
  void setRecentImage(QString &fname );
};

#endif /** CONTROLLER_H */
