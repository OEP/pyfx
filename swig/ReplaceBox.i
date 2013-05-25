%module native
%{
#include "ReplaceBox.h"
%}

%include "ReplaceBox.h"

%template(ScalarReplaceBox) vr::ReplaceBox<float, vr::Vector>;
%template(VectorReplaceBox) vr::ReplaceBox<vr::Vector, vr::Matrix>;
%template(MatrixReplaceBox) vr::ReplaceBox<vr::Matrix, int>;
%template(ColorReplaceBox) vr::ReplaceBox<vr::Color, int>;
