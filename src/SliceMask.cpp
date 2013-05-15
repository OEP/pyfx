#include "SliceMask.h"

using namespace vr;

const float SliceMask::eval(int i, const Vector &p) const
{
  int result = 1;

  for(size_t j = 0; result && j < m_Volumes->size(); j++)
  {
    result = ((*m_Volumes)[j]->eval(p) > 0) == (i & 1);
    i = i >> 1;
  }

  return result;
}

const Vector SliceMask::grad(int i, const Vector &p) const
{
  throw std::runtime_error("SliceMask::grad(): Not implemented.");
}

const Box SliceMask::getBBox(int i) const
{
  throw std::runtime_error("SliceMask::getBBox(): Not implemented.");
}

const size_t SliceMask::slices() const
{
  return 1 << m_Volumes->size();
}
