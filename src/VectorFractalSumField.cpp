#include "VectorFractalSumField.h"

using namespace vr;

const Vector VectorFractalSumField::eval(const Vector &p) const
{
  return Vector(
    m_FS->eval(p),
    m_FS->eval(p + m_V1),
    m_FS->eval(p + m_V2));
}

const Matrix VectorFractalSumField::grad(const Vector &p) const
{
  // TODO: Finite difference.
  throw std::runtime_error("VectorFractalSumField::grad(): Not implemented.");
}

const Box VectorFractalSumField::getBBox() const
{
  return Box::NIL;
}
