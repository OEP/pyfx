
%module native
%{
#include "Volume.h"
%}

%include "Volume.h"

%module native
%{
#include "Constant.h"
%}

%include "Constant.h"

%template(ScalarConstant) vr::Constant < float, vr::Vector >;
%template(VectorConstant) vr::Constant < vr::Vector, vr::Matrix >;
%template(MatrixConstant) vr::Constant < vr::Matrix, int >;
%template(ColorConstant)  vr::Constant < vr::Color, int >;
