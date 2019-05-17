#include "FileImage.hpp"
#include "NiftiHeader.hpp"
#include "gdcm.h"
#include "guiimage.h"
#include <QDebug>
#include <QPixmap>
#include <QRgb>

GuiImage::GuiImage( QString fname, QObject *parent ) : QObject( parent ),
  m_image( GDCM::OpenGImage( fname.toStdString( ) ) ), m_label( ), labelDisplayType( LabelDisplayType::border ),
  labelDisplayColor( LabelDisplayColor::gray ), m_translucence_factor( 127 ), m_fileName( fname ), m_contrast( 0 ),
  m_brightness( 0 ) {
  BGN_CMT( "Begin.", 0 );
  transform.resize( 4 );
  Bial::Vector< size_t > dim( m_image.Dim( ) );
  bounding.insert( bounding.begin( ), 3,
                   Bial::BBox( Bial::Point3D( 0, 0, 0 ), Bial::Point3D( dim( 0 ), dim( 1 ), 1 ) ) );
  m_currentSlice.insert( m_currentSlice.begin( ), 3, 0 );
  COMMENT( "Getting maximum intensity.", 2 );
  m_img_max = m_image.Maximum( );
  COMMENT( "Adjusting slice to appear on canvas.", 2 );
  if( Bial::NiftiHeader::IsNiftiFile( fname.toStdString( ) ) ) {
    COMMENT( "Generating Axial affine transform.", 2 );
    transform[ 0 ].Rotate( 90.0, Bial::FastTransform::X ).Rotate( 90.0, Bial::FastTransform::Y );
    transform[ 0 ].Scale( 1, -1, -1 );
    updateBoundings( 0 );
    COMMENT( "Generating Coronal affine transform.", 2 );
    transform[ 1 ].Rotate( 180.0, Bial::FastTransform::Z ).Rotate( 90.0, Bial::FastTransform::Y );
    transform[ 1 ].Scale( -1, 1, 1 );
    updateBoundings( 1 );
    COMMENT( "Generating Sagittal affine transform.", 2 );
    transform[ 2 ].Rotate( 180.0, Bial::FastTransform::Z );
    updateBoundings( 2 );
  }
  else {
    COMMENT( "Generating Axial affine transform.", 2 );
    transform[ 0 ].Rotate( 90.0, Bial::FastTransform::X ).Rotate( 90.0, Bial::FastTransform::Y );
    updateBoundings( 0 );
    COMMENT( "Generating Coronal affine transform.", 2 );
    transform[ 1 ].Rotate( 90.0, Bial::FastTransform::Y );
    updateBoundings( 1 );
    COMMENT( "Generating Sagittal affine transform.", 2 );
    transform[ 2 ].Rotate( 180.0, Bial::FastTransform::Z );
    updateBoundings( 2 );
  }
  cachedImagePixmaps.resize( 3 );
  cachedLabelPixmaps.resize( 3 );
  needImageUpdate.insert( needImageUpdate.begin( ), 3, true );
  needLabelUpdate.insert( needLabelUpdate.begin( ), 3, true );
  for( size_t axis = 0; axis < m_currentSlice.size( ); ++axis ) {
    setCurrentSlice( axis, depth( axis ) / 2 );
  }
  END_CMT( "End.", 0 );
}

GuiImage::~GuiImage( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End. Calling Tool Destructor as this is its parent.", 0 );
}

QString GuiImage::fileName( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_fileName );
}

QPixmap GuiImage::getImageSlice( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  size_t slice = currentSlice( axis );
  COMMENT( "GET SLICE: image = " << m_fileName.toStdString( ) << ", axis = " << axis << ", slice = " << slice, 0 );
  if( needImageUpdate[ axis ] ) {
    COMMENT( "Needed update.", 0 );
    const size_t w_size = width( axis );
    const size_t h_size = heigth( axis );
    QImage res( w_size, h_size, QImage::Format_ARGB32 );
    double factor = 255.0 / ( double ) m_img_max;
    double contrastLevel = std::pow( ( 100.0 + m_contrast ) / 100.0, 2 );
    const Bial::FastTransform &transf = transform[ axis ];
    COMMENT( "Transform.", 0 );
#pragma omp parallel for default(none) shared(transf, res) firstprivate(slice, factor, contrastLevel) // removed m_image from shared.
    for( size_t h = 0; h < h_size; ++h ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( h );
      for( size_t w = 0; w < w_size; ++w ) {
        int pixel = 0;
        int xx, yy, zz;
        transf( w, h, slice, &xx, &yy, &zz );
        pixel = m_image( xx, yy, zz ); /* Pegar imagem e depois usar aqui dentro. */
        pixel += m_brightness;
        pixel = ( ( ( ( pixel / 255.0 ) - 0.5 ) * contrastLevel ) + 0.5 ) * 255.0 * factor;
        pixel = qMax( qMin( pixel, 255 ), 0 );
        scanLine[ w ] = qRgb( pixel, pixel, pixel );
      }
    }
    COMMENT( "Updating pixmap.", 0 );
    cachedImagePixmaps[ axis ] = QPixmap::fromImage( res );
    needImageUpdate[ axis ] = false;
  }
  END_CMT( "End.", 0 );
  return( cachedImagePixmaps[ axis ] );
}

size_t GuiImage::width( size_t axis = 0 ) {
  BGN_CMT( "Begin.", 0 );
  size_t res = std::abs( std::round( bounding.at( axis ).pMax.x ) );
  END_CMT( "End.", 0 );
  return( res );
}

size_t GuiImage::heigth( size_t axis = 0 ) {
  BGN_CMT( "Begin.", 0 );
  size_t res = std::abs( std::round( bounding.at( axis ).pMax.y ) );
  END_CMT( "End.", 0 );
  return( res );
}

size_t GuiImage::depth( size_t axis = 0 ) {
  BGN_CMT( "Begin.", 0 );
  size_t res = std::abs( std::round( bounding.at( axis ).pMax.z ) ); // Error here. There is no bounding...
  END_CMT( "End.", 0 );
  return( res );
}

void GuiImage::setCurrentSlice( size_t axis, size_t slice ) {
  BGN_CMT( "Begin. slice: " << slice << ", axis: " << axis, 0 );
  size_t sz = m_currentSlice.size( );
  if( axis < sz ) {
    if( ( m_currentSlice[ axis ] != slice ) && ( slice < depth( axis ) ) ) {
      COMMENT( "Updating current slice: " << m_currentSlice[ axis ] << " with " << slice, 0 );
      m_currentSlice[ axis ] = slice;
      needImageUpdate[ axis ] = true;
      needLabelUpdate[ axis ] = true;
      COMMENT( "Emitting pixmapUpdated.", 0 );
      emit pixmapUpdated( );
    }
  }
  else {
    END_CMT( "End.", 0 );
    throw std::out_of_range( BIAL_ERROR( "Axis out of range." ) );
  }
  END_CMT( "End.", 0 );
}

Bial::Point3D GuiImage::getPosition( QPointF pos, size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  Bial::Point3D point( pos.x( ), pos.y( ), static_cast< double >( m_currentSlice[ axis ] ) );
  transform[ axis ]( point, &point );
  END_CMT( "End.", 0 );
  return( point );
}

Bial::FastTransform GuiImage::getTransform( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( transform.at( axis ) );
}

Bial::Image< int > &GuiImage::getImage( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_image );
}

Bial::Image< int > &GuiImage::getLabel( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_label );
}

Bial::Vector< size_t > GuiImage::getDim( ) {
  BGN_CMT( "Begin.", 0 );
  Bial::Vector< size_t > res( m_image.Dim( ) );
  END_CMT( "End.", 0 );
  return( res );
}

size_t GuiImage::getSize( ) {
  BGN_CMT( "Begin.", 0 );
  size_t res = m_image.size( );
  END_CMT( "End.", 0 );
  return( res );
}

void GuiImage::rotate90( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  Bial::FastTransform transf;
  transf.Rotate( -90.0, Bial::FastTransform::Z );
  transform[ axis ] = transf * transform[ axis ].Inverse( );
  updateBoundings( axis );
  needImageUpdate[ axis ] = true;
  needLabelUpdate[ axis ] = true;
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
}

void GuiImage::flipH( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  Bial::FastTransform transf;
  transf.Scale( -1, 1, 1 );
  transform[ axis ] = transf * transform[ axis ].Inverse( );
  updateBoundings( axis );
  needImageUpdate[ axis ] = true;
  needLabelUpdate[ axis ] = true;
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
}

void GuiImage::flipV( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  Bial::FastTransform transf;
  transf.Scale( 1, -1, 1 );
  transform[ axis ] = transf * transform[ axis ].Inverse( );
  updateBoundings( axis );
  needImageUpdate[ axis ] = true;
  needLabelUpdate[ axis ] = true;
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
}

int GuiImage::max( ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_img_max );
}

size_t GuiImage::currentSlice( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_currentSlice[ axis ] );
}

int GuiImage::getPixel( int x, int y, int z ) {
  BGN_CMT( "Begin.", 0 );
  int color = 0;
  if( m_image.ValidCoordinate( x, y, z ) ) {
    color = m_image( x, y, z );
  }
  END_CMT( "End.", 0 );
  return( color );
}

int GuiImage::getContrast( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_contrast );
}

void GuiImage::setContrast( int contrast ) {
  BGN_CMT( "Begin.", 0 );
  m_contrast = contrast;
  for( size_t axis = 0; axis < needImageUpdate.size( ); ++axis )
    needImageUpdate[ axis ] = true;
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
}

int GuiImage::getBrightness( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_brightness );
}

void GuiImage::setBrightness( int brightness ) {
  BGN_CMT( "Begin.", 0 );
  m_brightness = brightness;
  for( size_t axis = 0; axis < needImageUpdate.size( ); ++axis )
    needImageUpdate[ axis ] = true;
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
}

LabelDisplayType GuiImage::getLabelDisplayType( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( labelDisplayType );
}

LabelDisplayColor GuiImage::getLabelDisplayColor( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( labelDisplayColor );
}

void GuiImage::setLabelDisplayType( const LabelDisplayType &value ) {
  BGN_CMT( "Begin.", 0 );
  if( labelDisplayType == value ) {
    END_CMT( "End.", 0 );
    return;
  }
  labelDisplayType = value;
  for( size_t axis = 0; axis < 3; ++axis ) {
    needLabelUpdate[ axis ] = true;
  }
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
}

void GuiImage::setLabelDisplayColor( const LabelDisplayColor &value ) {
  BGN_CMT( "Begin.", 0 );
  if( labelDisplayColor == value ) {
    END_CMT( "End.", 0 );
    return;
  }
  labelDisplayColor = value;
  for( size_t axis = 0; axis < 3; ++axis ) {
    needLabelUpdate[ axis ] = true;
  }
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
}

bool GuiImage::validLabel(const Bial::Image< int > &label, int max ) {
  BGN_CMT( "Begin.", 0 );
  bool valid = true;
  if( max == 0 ) {
    valid = false;
    BIAL_WARNING( "Label image should have valid pixels (pixel > 0)." );
  }
  else {
    if( label.Dims( ) == 3 ) { // guiImage->getDims( )
      for( size_t dim = 0; dim < label.Dims( ); ++dim ) {
        valid &= ( label.Dim( ).at( dim ) == m_image.Dim( ).at( dim ) );
      }
    }
    else {
      BIAL_WARNING( "Label image should have same dimensions. " << label.Dims( ) << " != " << 3 ); // guiImage->getDims( )
      valid = false;
    }
  }
  END_CMT( "End.", 0 );
  return( valid );
}

bool GuiImage::addLabel( QString filename ) {
  BGN_CMT( "Begin.", 0 );
  Bial::Image< int > label( Bial::Read< int >( filename.toStdString( ) ) );
  int max = label.Maximum( );
  if( !validLabel( label, max ) ) {
    END_CMT( "End.", 0 );
    return( false );
  }
  m_label = std::move( label );
  m_lbl_max = std::move( max );
  labelColors( );
  setHasLabel( true );
  for( int axis = 0; axis < 3; ++axis ) {
    needLabelUpdate[ axis ] = true;
  }
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
  return( true );
}

void GuiImage::labelColors( ) {
  BGN_CMT( "Begin.", 0 );
  Bial::Vector< bool > defined( m_lbl_max + 1, false );
  size_t size = m_label.size( );
  int min = m_label.Minimum( );
  COMMENT( "size: " << size << ", min: " << min << ", max: " << m_lbl_max, 0 );
  m_label_color = Bial::Vector< QRgb >( m_lbl_max + 1, qRgb( 0, 0, 0 ) );
  for( size_t pxl = 0; pxl < size; ++pxl ) {
    size_t idx = m_label[ pxl ];
    COMMENT( "idx: " << idx, 3 );
    if( !defined[ idx ] ) {
      defined[ idx ] = true;
      COMMENT( "Computing mapcolor. idx: " << idx, 3 );
      m_label_color[ idx ] = GuiImage::mapcolor( idx, min, m_lbl_max, 0 );
    }
  }
  END_CMT( "End.", 0 );
}

QRgb GuiImage::getLabelColor( size_t index ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_label_color[ index ] );
}

void GuiImage::setLabelColor( size_t index, QRgb color ) {
  BGN_CMT( "Begin.", 0 );
  if( m_label_color[ index ] != color ) {
    m_label_color[ index ] = color;
    for( int axis = 0; axis < 3; ++axis ) {
      needLabelUpdate[ axis ] = true;
    }
  }
  END_CMT( "End.", 0 );
}

size_t GuiImage::labels( ) {
  return( m_lbl_max + 1 );
}

void GuiImage::removeLabel( ) {
  BGN_CMT( "Begin.", 0 );
  setHasLabel( false );
  m_label = Bial::Image< int >( );
  m_lbl_max = 0;
  for( int axis = 0; axis < 3; ++axis ) {
    cachedLabelPixmaps[ axis ] = QPixmap( );
    needLabelUpdate[ axis ] = true;
  }
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
}

QPixmap GuiImage::getLabelSlice( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  if( ( !needLabelUpdate[ axis ] ) || ( !hasLabel( ) ) ) {
    END_CMT( "End.", 0 );
    return( cachedLabelPixmaps[ axis ] );
  }
  size_t slice = currentSlice( axis );
  const size_t cols = width( axis );
  const size_t rows = heigth( axis );
  Bial::FastTransform transf = getTransform( axis );
  QImage res( cols, rows, QImage::Format_ARGB32 );
  if( labelDisplayType == LabelDisplayType::none ) {
    COMMENT( "No label.", 0 );
    res.fill( qRgba( 0, 0, 0, 0 ) );
  }
  else if( labelDisplayType == LabelDisplayType::border ) {
    COMMENT( "Border label. Gray? " << ( labelDisplayColor == LabelDisplayColor::gray ), 0 );
    float factor = m_translucence_factor / m_lbl_max;
    int alpha = qMin( static_cast< int >( m_translucence_factor ), 255 );
#pragma omp parallel for firstprivate( axis, cols, rows, alpha, transf, factor ) shared( res )
    for( size_t row = 0; row < rows; ++row ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( row );
      for( size_t col = 0; col < cols; ++col ) {
        int xx, yy, zz;
        transf( col, row, slice, &xx, &yy, &zz );
        COMMENT( "col: " << col << ", row: " << row << ", slice: " << slice <<
                 ", x: " << xx << ", y: " << yy << ", z: " << zz, 3 );
        int color = m_label( xx, yy, zz );
        bool diff_neighbour = false;
        if( col > 0 ) {
          COMMENT( "Cheking Left neighbor.", 3 );
          transf( col - 1, row, slice, &xx, &yy, &zz );
          if( color != m_label( xx, yy, zz ) )
            diff_neighbour = true;
        }
        if( ( !diff_neighbour ) && ( col < cols - 1 ) ) {
          COMMENT( "Cheking Right neighbor.", 3 );
          transf( col + 1, row, slice, &xx, &yy, &zz );
          if( color != m_label( xx, yy, zz ) )
            diff_neighbour = true;
        }
        if( ( !diff_neighbour ) && ( row > 0 ) ) {
          COMMENT( "Cheking Upper neighbor.", 3 );
          transf( col, row - 1, slice, &xx, &yy, &zz );
          if( color != m_label( xx, yy, zz ) )
            diff_neighbour = true;
        }
        if( ( !diff_neighbour ) && ( row < rows - 1 ) ) {
          COMMENT( "Cheking Lower neighbor.", 3 );
          transf( col, row + 1, slice, &xx, &yy, &zz );
          if( color != m_label( xx, yy, zz ) ) {
            diff_neighbour = true;
          }
        }
        if( diff_neighbour ) {
          COMMENT( "Border pixel.", 3 );
          if( labelDisplayColor == LabelDisplayColor::gray )
            scanLine[ col ] = qRgba( 255, 255, 255, color * factor );
          else {
            QRgb rgb( m_label_color[ color ] );
            scanLine[ col ] = qRgba( qRed( rgb ), qGreen( rgb ), qBlue( rgb ), alpha );
          }
        }
        else {
          COMMENT( "Internal pixel.", 3 );
          scanLine[ col ] = qRgba( 0, 0, 0, 0 );
        }
      }
    }
  }
  else if( labelDisplayType == LabelDisplayType::solid ) {
    COMMENT( "Solid label. Gray? " << ( labelDisplayColor == LabelDisplayColor::gray ), 0 );
    float factor = 255.0 / m_lbl_max;
    int alpha = qMin( static_cast< int >( m_translucence_factor ), 255 );
#pragma omp parallel for firstprivate( axis, cols, rows, alpha, transf, factor ) shared( res )
    for( size_t row = 0; row < rows; ++row ) {
      QRgb *scanLine = ( QRgb* ) res.scanLine( row );
      for( size_t col = 0; col < cols; ++col ) {
        int xx, yy, zz;
        transf( col, row, slice, &xx, &yy, &zz );
        if( labelDisplayColor == LabelDisplayColor::gray ) {
          int color = m_label( xx, yy, zz ) * factor;
          if( color != 0 )
            scanLine[ col ] = qRgba( color, color, color, alpha );
          else
            scanLine[ col ] = qRgba( 0, 0, 0, 0 );
        }
        else {
          int color = m_label( xx, yy, zz );
          if( color != 0 ) {
            QRgb rgb( m_label_color[ color ] );
            scanLine[ col ] = qRgba( qRed( rgb ), qGreen( rgb ), qBlue( rgb ), alpha );
          }
          else
            scanLine[ col ] = qRgba( 0, 0, 0, 0 );

        }
      }
    }
  }
  needLabelUpdate[ axis ] = false;
  cachedLabelPixmaps[ axis ] = QPixmap::fromImage( res );
  END_CMT( "End.", 0 );
  return( cachedLabelPixmaps[ axis ] );
}

bool GuiImage::hasLabel( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_hasLabel );
  //return( true );
}

void GuiImage::setHasLabel( bool sHasLabel ) {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  m_hasLabel = sHasLabel;
}

void GuiImage::setTranslucenceFactor( float value ) {
  BGN_CMT( "Begin.", 0 );
  if( m_translucence_factor == value ) {
    END_CMT( "End.", 0 );
    return;
  }
  m_translucence_factor = value;
  for( int axis = 0; axis < 3; ++axis ) {
    needLabelUpdate[ axis ] = true;
  }
  COMMENT( "Emitting pixmapUpdated.", 0 );
  emit pixmapUpdated( );
  END_CMT( "End.", 0 );
}

float GuiImage::getTranslucenceFactor( ) const {
  BGN_CMT( "Begin.", 0 );
  END_CMT( "End.", 0 );
  return( m_translucence_factor );
}

QRgb GuiImage::mapcolor( double v, double vmin, double vmax, int alpha ) {
  BGN_CMT( "Begin.", 3 );
  double r, g, b;
  r = g = b = 1.0;
  if( v < vmin ) {
    v = vmin;
  }
  if( v > vmax ) {
    v = vmax;
  }
  double dv = vmax - vmin;
  if( v < ( vmin + 0.25 * dv ) ) {
    r = 0.0;
    g = 4.0 * ( v - vmin ) / dv;
  }
  else if( v < ( vmin + 0.5 * dv ) ) {
    r = 0.0;
    b = 1.0 + 4.0 * ( vmin + 0.25 * dv - v ) / dv;
  }
  else if( v < ( vmin + 0.75 * dv ) ) {
    r = 4.0 * ( v - vmin - 0.5 * dv ) / dv;
    b = 0.0;
  }
  else {
    g = 1.0 + 4.0 * ( vmin + 0.75 * dv - v ) / dv;
    b = 0.0;
  }
  COMMENT( "r: " << r << ", g: " << g << ", b: " << b, 3 );
  int ir, ig, ib;
  ir = qMin( ( int ) ( 255 * r ), 255 );
  ig = qMin( ( int ) ( 255 * g ), 255 );
  ib = qMin( ( int ) ( 255 * b ), 255 );
  END_CMT( "End.", 3 );
  return( qRgba( ir, ig, ib, alpha ) );
}

QPointF GuiImage::getCoordinates( QPointF &posF, size_t curr_axis, size_t new_axis ) {
  size_t curr_slice = m_currentSlice[ curr_axis ];
  BGN_CMT( "Begin. Input point: " << posF.x( ) << ", " << posF.y( ) << ", "
           << curr_slice << ". Axis: " << curr_axis, 0 );
  Bial::Point3D pt = transform[ curr_axis ]( static_cast< double >( posF.x( ) ),
                                             static_cast< double >( posF.y( ) ),
                                             static_cast< double >( curr_slice ) );
  COMMENT( "Overlay current position: " << pt << " to axis " << new_axis, 0 );
  Bial::FastTransform invTransf = transform[ new_axis ].Inverse( );
  Bial::Point3D invPt = invTransf( pt );
  COMMENT( "Current position in axis " << new_axis << ": " << invPt, 0 );
  QPointF res( invPt[ 0 ], invPt[ 1 ] );
  END_CMT( "End.", 0 );
  return( res );
}

void GuiImage::updateBoundings( size_t axis ) {
  BGN_CMT( "Begin.", 0 );
  Bial::Point3D start;
  Bial::Vector< size_t > size( getDim( ) );
  Bial::Point3D end( size[ 0 ] - 1, size[ 1 ] - 1, std::max( size[ 2 ] - 1, ( size_t ) 1 ) );
  transform[ axis ]( start, &start );
  transform[ axis ]( end, &end );
  bounding[ axis ] = Bial::BBox( start, end );
  transform[ axis ] = transform[ axis ].Inverse( );
  transform[ axis ].Translate( bounding[ axis ].pMin.x, bounding[ axis ].pMin.y, bounding[ axis ].pMin.z );
  bounding[ axis ] = bounding[ axis ].Normalized( );
  END_CMT( "End.", 0 );
}
