#include "MergeColor.h"

using namespace vr;

const int
  MergeColor::MAX = 0,
  MergeColor::MIN = 1,
  MergeColor::PLUS = 2,
  MergeColor::MINUS = 3;

const Color MergeColor::eval(const Vector &p) const
{
  const Color
    a = m_A->eval(p),
    b = m_B->eval(p);

  switch(m_Mode)
  {
    case MergeColor::MAX: return a.componentMax(b);
    case MergeColor::MIN: return a.componentMin(b);
    case MergeColor::PLUS: return a + b;
    case MergeColor::MINUS: return a - b;
    default: throw std::runtime_error("MergeColor: invalid mode: " + m_Mode);
  }
}

const int MergeColor::grad(const Vector &p) const
{
  throw std::runtime_error("MergeColor::grad(): Not implemented.");
}

const Box MergeColor::getBBox() const
{
  const Box
    a = m_A->getBBox(),
    b = m_B->getBBox();
  switch(m_Mode)
  {
    case MergeColor::MIN: return a.intersect(b);
    default: return a.expand(b);
  }
}
