%module native
%{
#include "DenseGrid.h"
%}

%include "DenseGrid.h"

%template(ScalarDenseGrid) vr::DenseGrid<float, vr::Vector>;
%template(VectorDenseGrid) vr::DenseGrid<vr::Vector, vr::Matrix>;
%template(ColorDenseGrid) vr::DenseGrid<vr::Color, int>;
%template(MatrixDenseGrid) vr::DenseGrid<vr::Matrix, int>;
