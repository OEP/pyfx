#include "ColorComponent.h"

using namespace vr;

const float ColorComponent::eval(const Vector &p) const
{
  return m_C->eval(p)[m_Index];
}

const Vector ColorComponent::grad(const Vector &p) const
{
  throw std::runtime_error("ColorComponent::grad(): Not implemented.");
}

const Box ColorComponent::getBBox() const
{
  throw std::runtime_error("ColorComponent::getBBox(): Not implemented.");
}
