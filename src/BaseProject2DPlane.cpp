#include "BaseProject2DPlane.h"

using namespace vr;

const float BaseProject2DPlane::eval(const Vector &p) const
{
  // Project p onto a point 'x' on the image plane.
  const Vector
    origin_to_p = (p - m_X0),
    x = p - m_View * (origin_to_p * m_View);

  // Put x in image coordinates.
  const Vector2D
    plane_coord =
      Vector2D(m_Right.dot(x - m_X0) + 0.5,
                  m_Up.dot(x - m_X0) + 0.5);
  
  // The rest is up to the child class.
  return evalPlane(plane_coord);
}


const Vector BaseProject2DPlane::grad(const Vector &p) const
{
  throw std::runtime_error("BaseProject2DPlane::grad(): Not implemented.");
}

const Box BaseProject2DPlane::getBBox() const
{
  throw std::runtime_error("BaseProject2DPlane::getBBox(): Not implemented.");
}
