/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2014/Dec/3
 * @brief Bial GDCM class. Wrapper to the GDCM library
 */
#include "Common.hpp"
#include "Image.hpp"
#include "guiimage.h"
#ifndef BIALGDCM_HPP
#define BIALGDCM_HPP

namespace gdcm {
  class Image;
}

class GDCM {
public:
  static Bial::Image< int > OpenGImage( const std::string &filename );
};

#endif /* BIALGDCM_HPP */
