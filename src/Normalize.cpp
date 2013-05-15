#include "Normalize.h"

using namespace vr;

const Vector Normalize::eval(const Vector &p) const
{
  const Vector f = m_Field->eval(p);
  const float mag = f.length();

  if(mag == 0.0f)
  {
    return f;
  }

  return f.unit();
}

const Matrix Normalize::grad(const Vector &p) const
{
  throw std::runtime_error("Normalize::grad(): Not implemented.");
}

const Box Normalize::getBBox() const
{
  throw std::runtime_error("Normalize::getBBox(): Not implemented.");
}
