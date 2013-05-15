%module vrend
%{
#include "SparseGrid.h"
%}

%include "SparseGrid.h"

%template(ScalarSparseGrid) vr::SparseGrid<float, vr::Vector>;
%template(VectorSparseGrid) vr::SparseGrid<vr::Vector, vr::Matrix>;
%template(ColorSparseGrid) vr::SparseGrid<vr::Color, int>;
%template(MatrixSparseGrid) vr::SparseGrid<vr::Matrix, int>;
