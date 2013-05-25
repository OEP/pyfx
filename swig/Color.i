%module native

%include "exception.i"

%exception __getitem__ {
  try
  {
    $action
  }
  catch (std::out_of_range &e)
  {
    SWIG_exception(SWIG_IndexError, "Index out of bounds");
  }
}

%{
#include <stdexcept>
#include "Color.h"
%}

%include "Color.h"

%template(ColorVector) std::vector<vr::Color>;
