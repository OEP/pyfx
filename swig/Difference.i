%module native
%{
#include "Difference.h"
%}

%include "Difference.h"

%template(ScalarDifference) vr::Difference<float,vr::Vector>;
%template(VectorDifference) vr::Difference<vr::Vector,vr::Matrix>;
%template(MatrixDifference) vr::Difference<vr::Matrix,int>;
%template(ColorDifference)  vr::Difference<vr::Color,int>;
