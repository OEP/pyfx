%module vrend
%{
#include "Ramp.h"
%}

%include "Ramp.h"

%template(ScalarRamp) vr::Ramp<float, vr::Vector>;
%template(ColorRamp) vr::Ramp<vr::Color, int>;
%template(VectorRamp) vr::Ramp<vr::Vector, vr::Matrix>;
