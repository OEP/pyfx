%module vrend
%{
#include "Scale.h"
%}

%include "Scale.h"

namespace vr
{
  %template(ScalarScale) Scale<float, Vector>;
  %template(VectorScale) Scale<Vector, Matrix>;
  %template(MatrixScale) Scale<Matrix, int>;
  %template(ColorScale) Scale<Color, int>;
}
