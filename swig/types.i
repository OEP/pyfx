%module native
%{
#include "Volume.h"
#include "types.h"
%}

%include "types.h"
%include "Volume.h"

namespace vr
{
  typedef ScalarVolumeSet std::vector<vr::ScalarField*>;
}

%template(ScalarVolumeSet) std::vector<vr::ScalarField*>;
// %template(VectorSequence) std::vector<vr::Vector>;
%template(Vector2DSequence) std::vector<vr::Vector2D>;
