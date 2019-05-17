#include "Common.hpp"
#include "FileImage.hpp"
#include "NiftiHeader.hpp"
#include "gdcm.h"
#include "guiimage.h"

#include <QFileInfo>
#include <QImageReader>

Bial::Image< int > GDCM::OpenGImage( const std::string &filename ) {
  BGN_CMT( "Begin.", 0 );
  QFileInfo fileInfo( QString::fromStdString( filename ) );
  QString regexpr3D( ".nii|.img|.scn|.hdr|.nii.gz|.img.gz|.scn.gz|.hdr.gz" );
  if( regexpr3D.contains( fileInfo.completeSuffix( ) ) ) {
    Bial::NiftiHeader hdr( filename );
    Bial::NiftiType type( hdr.DataType( ) );
    switch( type ) {
      case Bial::NiftiType::INT8:
      case Bial::NiftiType::INT16:
      case Bial::NiftiType::INT32:
      case Bial::NiftiType::UINT8:
      case Bial::NiftiType::UINT16:
      case Bial::NiftiType::UINT32:
      case Bial::NiftiType::FLOAT32:
      END_CMT( "End.", 0 );
      return( Bial::Read< int >( filename ) );
      default:
      END_CMT( "End.", 0 );
      std::string msg( BIAL_ERROR( "Could not open Nifti image. Unsupported data type." ) );
      throw( std::runtime_error( msg ) );
    }
  }
  else {
    END_CMT( "End.", 0 );
    std::string msg( BIAL_ERROR( "Could not open image. Unsupported type." ) );
    throw( std::runtime_error( msg ) );
  }
}
