%module vrend
%{
#include "VolumeGrid.h"
%}

%include "VolumeGrid.h"

%template(ScalarVolumeGrid) vr::VolumeGrid<float, vr::Vector>;
%template(VectorVolumeGrid) vr::VolumeGrid<vr::Vector, vr::Matrix>;
%template(ColorVolumeGrid) vr::VolumeGrid<vr::Color, int>;
%template(MatrixVolumeGrid) vr::VolumeGrid<vr::Matrix, int>;
