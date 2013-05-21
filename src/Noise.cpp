#include "Noise.h"
using namespace vr;

const Box Noise::getBBox() const
{
  return Box::INFINITE;
}

const Vector Noise::grad(const Vector &p) const
{
  throw std::runtime_error("Noise::grad() unimplemented.");
}
