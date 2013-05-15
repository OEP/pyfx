#include "FindSurface.h"

using namespace vr;

const Vector FindSurface::inward(const Vector &p) const
{
  Vector in = m_F->grad(p);

  if(in.length() == 0)
  {
    in = getBBox().center() - p;
  }

  return in.unit();
}

const Vector FindSurface::eval(const Vector &p) const
{
  float step = m_InitialStep;
  Vector pp = p,
    dir = inward(pp);
  int inside = m_F->eval(pp) > 0 ? -1 : 1;
  int test;
  int count = m_MaxIterations;

  while(step > m_Tolerance && count > 0)
  {
    pp = pp + dir * step;
    dir = inward(pp) * inside;
    test = m_F->eval(pp) > 0 ? -1 : 1;

    if(inside != test)
    {
      inside = test;
      step = step / 2;
    }

    count--;
  }

  return pp;
}

const Matrix FindSurface::grad(const Vector &p) const
{
  throw std::runtime_error("FindSurface::grad(): Not implemented.");
}

const Box FindSurface::getBBox() const
{
  return m_F->getBBox();
}
