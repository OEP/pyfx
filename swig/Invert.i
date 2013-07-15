%module native
%{
#include "Invert.h"
%}

%include "Invert.h"

namespace vr
{
  %template(ScalarInvert) Invert<float, Vector>;
  %template(VectorInvert) Invert<Vector, Matrix>;
  %template(MatrixInvert) Invert<Matrix, int>;
  %template(ColorInvert) Invert<Color, int>;
}
