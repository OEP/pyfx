%module native
%{
#include "VolumeGrid.h"
%}

%include "VolumeGrid.h"

namespace vr
{
  %typedef VolumeGrid<float, Vector>  ScalarGridBase;
  %typedef VolumeGrid<Vector, Matrix> VectorGridBase;
  %typedef VolumeGrid<Matrix, int>    MatrixGridBase;
  %typedef VolumeGrid<Color, int>     ColorGridBase;

  %template(ScalarGridBase) VolumeGrid<float, Vector>;
  %template(VectorGridBase) VolumeGrid<Vector, Matrix>;
  %template(ColorGridBase)  VolumeGrid<Color, int>;
  %template(MatrixGridBase) VolumeGrid<Matrix, int>;
}
