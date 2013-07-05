#ifndef DEEPSHADOWMAP_H_
#define DEEPSHADOWMAP_H_

#include <vector>

#include "Griddable.h"
#include "DenseGrid.h"
#include "UniformPRN.h"
#include "Volume.h"

namespace vr
{
  class DeepShadowMap
  {
    public:
      DeepShadowMap(const ScalarField *_field, Light _light)
        : field(_field), light(_light) {}

      ScalarField *field;
      Light light;
  };
}

void compute_dsm(ScalarGrid *grid,
  const ScalarField *dns,
  const Vector &pl,
  const double ds,
  const int samples=1);

#endif
