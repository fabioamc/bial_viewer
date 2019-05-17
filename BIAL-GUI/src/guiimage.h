#ifndef GUIIMAGE_H
#define GUIIMAGE_H
#include "Geometrics.hpp"
#include "Image.hpp"
#include <QObject>
#include <QPixmap>

enum class LabelDisplayType : char {
  none,
  solid,
  border
};

enum class LabelDisplayColor : char {
  gray,
  color
};

/**
 * @brief The GuiImage class is a bridge to the Bial::Image data structure to the QImage data structure. <br>
 * It have many important functions, such as the axes spatial transforms, histogram equalization and others.
 */
class GuiImage : public QObject {
  Q_OBJECT
  /**
   * @brief size size of the image.
   */
  Bial::Image< int > m_image;
  Bial::Image< int > m_label;
  LabelDisplayType labelDisplayType;
  LabelDisplayColor labelDisplayColor;
  float m_translucence_factor; // Translucence factor of the label.
  bool m_hasLabel = false;
  /**
   * @brief m_fileName is the image absolute path.
   */
  QString m_fileName;
  /**
   * @brief cachedPixmaps holds a copy of the last generated pixmap at each axis.
   */
  Bial::Vector< QPixmap > cachedImagePixmaps;
  Bial::Vector< QPixmap > cachedLabelPixmaps;
  /**
   * @brief needUpdate is a flag that determines if a pixmap needs to be updated.
   */
  Bial::Vector< bool > needImageUpdate;
  Bial::Vector< bool > needLabelUpdate;
  /**
   * @brief transform is the affine transform related to each axis.
   */
  Bial::Vector< Bial::FastTransform > transform;
  /**
   * @brief bounding is the bounding box that determines each axis boundings.
   */
  Bial::Vector< Bial::BBox > bounding;
  /**
   * @brief m_currentSlice is the current slice at the z axis of each axis.
   */
  Bial::Vector< size_t > m_currentSlice;
  /**
   * @brief m_img_max is the maximum integer value at the input image.
   */
  int m_img_max;
  /**
   * @brief m_lbl_max is the maximum integer value at the input label.
   */
  int m_lbl_max;
  /**
   * @brief contrast level
   */
  int m_contrast;
  /**
   * @brief brightness level
   */
  int m_brightness;
  /**
   * @brief m_label_intensities: Intensities contained in label image.
   */
  Bial::Vector< QRgb > m_label_color;
private:
  /**
   * @brief updateBoundings is called each time the transformation matrix is updated. <br>
   * This function applys the affone transform to the input image boundings, and fixes
   * negative positions.
   * @param axis
   */
  void updateBoundings( size_t axis );

public:
  /**
   * @brief GuiImage is the GuiImage class constructor.
   * @param fName is the absolute path to the input image.
   * @param parent is the QObject parent.
   */
  explicit GuiImage( QString fName, QObject *parent = 0 );
  /**
   * @brief Destructor
   */
  virtual ~GuiImage( );
  /**
   * @brief fileName is the image absolute path getter.
   * @return
   */
  QString fileName( );
  /**
   * @brief getSlice calculates and returns a QImage with the current slice of the axis. <br>
   * @see setCurrentSlice
   * @param axis
   * @return
   */
  QPixmap getImageSlice( size_t axis );
  /**
   * @brief width is the axis width.
   * @param axis
   * @return
   */
  size_t width( size_t axis );
  /**
   * @brief heigth is the axis heigth.
   * @param axis
   * @return
   */
  size_t heigth( size_t axis );
  /**
   * @brief depth is the number of slices of the axis.
   * @param axis
   * @return
   */
  size_t depth( size_t axis );
  /**
   * @brief currentSlice is the axis's current slice.
   * @param axis
   * @return
   */
  size_t currentSlice( size_t axis );
  /**
   * @brief currentSlice sets the axis's current slice.
   * @param axis
   * @param slice
   * @return
   */
  void setCurrentSlice( size_t axis, size_t slice );
  /**
   * @brief getPosition transforms the scene position to the input image coordinates.
   * @param pos is the graphicsscene position.
   * @param axis is the axis number.
   * @return The input image coordinates.
   */
  Bial::Point3D getPosition( QPointF pos, size_t axis );
  /**
   * @brief getTransform returns the transform matrix of the axes.
   * @param axis
   * @return
   */
  Bial::FastTransform getTransform( size_t axis );
  /**
   * @brief getImage
   * @return a reference to Bial image.
   */
  Bial::Image< int > &getImage( );
  /**
   * @brief getImage
   * @return a reference to Bial label.
   */
  Bial::Image< int > &getLabel( );
  /**
   * @brief getDim
   * @return a vector with bial image dimensions.
   */
  Bial::Vector< size_t > getDim( );
  /**
   * @brief getSize
   * @return the image size.
   */
  size_t getSize( );
  /**
   * @brief rotate90 rotates a axis in 90 degrees.
   * @param axis axis number
   */
  void rotate90( size_t axis );
  /**
   * @brief flipH mirrors the current axis on X axis.
   * @param axis axis number
   */
  void flipH( size_t axis );
  /**
   * @brief flipV mirrors the current axis on Y axis.
   * @param axis axis number
   */
  void flipV( size_t axis );
  /**
   * @brief max is the maximum integer intensity of the input image.
   * @return
   */
  int max( );
  /**
   * @brief getPixel returns the pixel intensity of the image at the given position.
   * @param x
   * @param y
   * @param z
   * @return
   */
  int getPixel( int x, int y, int z = 0 );
  /**
   * @brief getContrast and setContrast: Functions for the contrast applied to compute the pixmaps.
   * @return
   */
  int getContrast( ) const;
  void setContrast( int contrast );
  /**
   * @brief getBrightness and setBrightness: Functions for the brightness applied to compute the pixmaps.
   * @return
   */
  int getBrightness( ) const;
  void setBrightness( int brightness );

  /**
   * @brief getLabelDisplayType: returns the display type of the label.
   */
  LabelDisplayType getLabelDisplayType( ) const;
  /**
   * @brief getLabelDisplayColor: returns the display color of the label.
   * @return
   */
  LabelDisplayColor getLabelDisplayColor( ) const;
  /**
   * @brief setLabelDisplayType: Type of the label can be: none, translucent, or solid.
   * @param value: The selected type of the label.
   */
  void setLabelDisplayType( const LabelDisplayType &value );
  /**
   * @brief setLabelDisplayColor: Color of the label can be: gray or color.
   * @param value: The selected color of the label.
   */
  void setLabelDisplayColor(const LabelDisplayColor &value);
  bool addLabel( QString filename );
  void removeLabel( );
  QPixmap getLabelSlice( size_t axis );
  void setHasLabel( bool sHasLabel );
  bool hasLabel( ) const;
  /**
   * @brief getTranslucenceFactor and setTranslucenceFactor: Functions to access the translucence factor
   *        applied to the label. It is its transparency.
   */
  float getTranslucenceFactor( ) const;
  void setTranslucenceFactor( float factor );

  /**
   * @brief mapcolor: generates a color for a given label.
   * @param v: intensity of label
   * @param vmin: minimum label intensity
   * @param vmax: maximum label intensity
   * @param alpha: translucence factor.
   * @return a color.
   */
  QRgb mapcolor( double v, double vmin, double vmax, int alpha );
  /**
   * @brief getOverlay: Gets point coordinates of point from one axis to another.
   * @param pos: Current axis position.
   * @param curr_axis: Current axis.
   * @param new_axis: New axis.
   * @return: coordinates inside the pixmap for the new axis.
   */
  QPointF getCoordinates( QPointF &posF, size_t curr_axis , size_t new_axis );
  /**
   * @brief getlabelColor: returns the color of a intensity label.
   * @param index: index of the desired intensity.
   * @return: The color assigned to a label.
   */
  QRgb getLabelColor( size_t index );
  /**
   * @brief setlabelColor: Updates the color of a label index.
   * @param index: The index to be changed.
   * @param color: The color to be assigned.
   */
  void setLabelColor( size_t index, QRgb color );
  /**
   * @brief labels: returns m_lbl_max + 1. That is the total number of labels.
   * @return Total number of labels.
   */
  size_t labels( );

signals:
  /**
   * @brief imageUpdated is called each time a internal property is updated,
   * after that the image axes are updated.
   */
  void pixmapUpdated( );

private:
  /**
   * @brief validLabel: Checks if the loaded label matches the image dimensions and if it is not zero.
   * @param label: Label image.
   * @param max: label maximum intensity.
   * @return true if label is valid.
   */
  bool validLabel( const Bial::Image< int > &label, int max );

  /**
   * @brief colorVector: Creates color vector for each available label intensity.
   */
  void labelColors( );
};

#endif /* GUIIMAGE_H */
