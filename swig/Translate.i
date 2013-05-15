%module vrend
%{
#include "Translate.h"
%}

%include "Translate.h"

namespace vr
{
  %template(ScalarTranslate) Translate<float, Vector>;
  %template(VectorTranslate) Translate<Vector, Matrix>;
  %template(MatrixTranslate) Translate<Matrix, int>;
  %template(ColorTranslate) Translate<Color, int>;
}
