#include "Griddable.h"
using namespace vr;

const Vector Griddable::computeResolution(int n) const
{
  return this->computeResolution(n, n, n);
}
