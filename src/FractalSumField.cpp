#include "FractalSumField.h"

using namespace vr;

const float FractalSumField::eval(const Vector &p) const
{
  return m_FS->eval(p);
}

const Vector FractalSumField::grad(const Vector &p) const
{
  throw std::runtime_error("FractalSumField::grad(): Not implemented.");
}

const Box FractalSumField::getBBox() const
{
  throw std::runtime_error("FractalSumField::getBBox(): Not implemented.");
}
