%module vrend
%{
#include "Rotate.h"
%}

%include "Rotate.h"

namespace vr
{
  %template(ScalarRotate) Rotate<float, Vector>;
  %template(VectorRotate) Rotate<Vector, Matrix>;
  %template(MatrixRotate) Rotate<Matrix, int>;
  %template(ColorRotate) Rotate<Color, int>;
}
