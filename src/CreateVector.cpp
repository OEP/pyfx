#include "CreateVector.h"

using namespace vr;

const Vector CreateVector::eval(const Vector &p) const
{
  return Vector(
    (m_X) ? m_X->eval(p) : 0.0f,
    (m_Y) ? m_Y->eval(p) : 0.0f,
    (m_Z) ? m_Z->eval(p) : 0.0f
  );
}

const Matrix CreateVector::grad(const Vector &p) const
{
  return Matrix(
    (m_X) ? m_X->grad(p) : Vector(0),
    (m_Y) ? m_Y->grad(p) : Vector(0),
    (m_Z) ? m_Z->grad(p) : Vector(0)
  );
}

const Box CreateVector::getBBox() const
{
  Box b = Box(0);
  if(m_X) { b = b.expand(m_X->getBBox()); }
  if(m_Y) { b = b.expand(m_Y->getBBox()); }
  if(m_Z) { b = b.expand(m_Z->getBBox()); }
  return b;
}
