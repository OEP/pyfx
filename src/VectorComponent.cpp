#include "VectorComponent.h"

using namespace vr;

const float VectorComponent::eval(const Vector &p) const
{
  return m_V->eval(p)[m_Component];
}

const Vector VectorComponent::grad(const Vector &p) const
{
  throw std::runtime_error("VectorComponent::grad(): Not implemented.");
}

const Box VectorComponent::getBBox() const
{
  return m_V->getBBox();
}
