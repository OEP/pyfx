#include "Identity.h"

using namespace vr;

const Vector Identity::eval(const Vector &p) const
{
  return p;
}

const Matrix Identity::grad(const Vector &p) const
{
  return Matrix();
}

const Box Identity::getBBox() const
{
  return Box::INFINITE;
}
