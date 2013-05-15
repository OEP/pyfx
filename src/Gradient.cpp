#include "Gradient.h"

using namespace vr;

const Vector Gradient::eval(const Vector &p) const
{
  return m_V->grad(p);
}

const Matrix Gradient::grad(const Vector &p) const
{
  throw std::runtime_error("Gradient::grad(): Not implemented.");
}

const Box Gradient::getBBox() const
{
  // TODO: Not really true -- maybe Box(-INF, INF)?
  return m_V->getBBox();
}
