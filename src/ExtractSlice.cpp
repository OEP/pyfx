#include "ExtractSlice.h"

using namespace vr;

const float ExtractSlice::eval(const Vector &p) const
{
  return m_Mask->eval(m_SliceIndex, p);
}

const Vector ExtractSlice::grad(const Vector &p) const
{
  return m_Mask->grad(m_SliceIndex, p);
}

const Box ExtractSlice::getBBox() const
{
  return m_Mask->getBBox(m_SliceIndex);
}
