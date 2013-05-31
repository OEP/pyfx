%module native

%newobject vr::Image::fromFile;

%{
#include "Image.h"
%}

namespace std
{
  %template(PixelArray) std::vector< std::vector<float> >;
}

%typedef std::vector<float> vr::Image::Pixel;

%include "Image.h"

