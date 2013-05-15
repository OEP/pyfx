%module vrend

%{
#include<vector>
#include "WispChain.h"
%}

%template(StdVectorWispParameters) std::vector<vr::WispParameters>;

%include "WispChain.h"
