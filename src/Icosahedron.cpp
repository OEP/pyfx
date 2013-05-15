#include <cmath>

#include "Icosahedron.h"

using namespace vr;

const float Icosahedron::eval(const Vector &p) const
{
  const double
    x = p[0],
    y = p[1],
    z = p[2],
    mag = p.magnitude(),
    T = 1.61803399;

  if(mag > 1.8 * M_PI)
  {
    return -1.8 * M_PI;
  }

  return
    cos(x + T*y) + cos(x - T*y)
    + cos(y + T*z) + cos(y - T*z)
    + cos(z - T*x) + cos(z + T*x) - 2.0;
}

const Vector Icosahedron::grad(const Vector &p) const
{
  const double
    x = p[0],
    y = p[1],
    z = p[2],
    mag = p.magnitude(),
    T = 1.61803399;

  if(mag > 1.8 * M_PI) return Vector(0,0,0);

  return Vector(
    T * (sin(z + T*x) - sin(z - T*x)) - sin(x + T*y) - sin(x - T*y),
    T * (sin(x + T*y) - sin(x - T*y)) - sin(y + T*z) - sin(y - T*z),
    T * (sin(y + T*z) - sin(y - T*z)) - sin(z + T*x) - sin(z - T*x)
  );
}

const Box Icosahedron::getBBox() const
{
  return Box(1.8 * M_PI);
}
