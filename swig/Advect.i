%module vrend
%{
#include "Advect.h"
%}

%include "Advect.h"

namespace vr
{
  %template(ScalarAdvect) Advect<float, Vector>;
  %template(VectorAdvect) Advect<Vector, Matrix>;
  %template(MatrixAdvect) Advect<Matrix, int>;
  %template(ColorAdvect) Advect<Color, int>;
}
