%module vrend

%newobject vr::Image::fromFile;

%{
#include "Image.h"
%}

%include "Image.h"

