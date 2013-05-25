%module native
%{
#include "Volume.h"
%}

%include "Volume.h"

%template(ScalarField) vr::Volume<float, vr::Vector>;
%template(VectorField) vr::Volume<vr::Vector, vr::Matrix>;
%template(MatrixField) vr::Volume<vr::Matrix, int>;
%template(ColorField) vr::Volume<vr::Color, int>;
