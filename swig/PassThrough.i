%module native
%{
#include "PassThrough.h"
%}

%include "PassThrough.h"

namespace vr
{
  %template(ScalarPassThrough) PassThrough<float, Vector>;
  %template(VectorPassThrough) PassThrough<Vector, Matrix>;
  %template(MatrixPassThrough) PassThrough<Matrix, int>;
  %template(ColorPassThrough) PassThrough<Color, int>;
}
