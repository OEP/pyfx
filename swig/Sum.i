%module native
%{
#include "Sum.h"
%}

%include "Sum.h"

%template(ScalarSum) vr::Sum<float,vr::Vector>;
%template(VectorSum) vr::Sum<vr::Vector, vr::Matrix>;
%template(MatrixSum) vr::Sum<vr::Matrix, int>;
%template(ColorSum)  vr::Sum<vr::Color, int>;
