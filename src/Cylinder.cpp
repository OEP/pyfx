#include <cmath>

#include "Cylinder.h"

using namespace vr;

const float Cylinder::eval(const Vector &p) const
{
  const Vector xperp = p - (p*m_N)*m_N;
  const float f = m_R - xperp.magnitude();
  const float dist = fabs(p * m_N);

  if(m_L < 0) return f;
  else if(dist > m_L/2)
  {
    return -1;
  }

  return f;
}

const Vector Cylinder::grad(const Vector &p) const
{
  throw std::runtime_error("Cylinder::grad(): not implemented");
}

const Box Cylinder::getBBox() const
{
  if(m_L < 0) return Box::INFINITE;

  const Vector
    end1 = m_N * (m_L/2),
    end2 = m_N * (-m_L/2),
    p1  = end1 + Vector::AXIS_X * m_R,
    p2  = end1 + Vector::AXIS_Y * m_R,
    p3  = end1 + Vector::AXIS_Z * m_R,
    p4  = end1 - Vector::AXIS_X * m_R,
    p5  = end1 - Vector::AXIS_Y * m_R,
    p6  = end1 - Vector::AXIS_Z * m_R,
    p7  = end2 + Vector::AXIS_X * m_R,
    p8  = end2 + Vector::AXIS_Y * m_R,
    p9  = end2 + Vector::AXIS_Z * m_R,
    p10 = end2 - Vector::AXIS_X * m_R,
    p11 = end2 - Vector::AXIS_Y * m_R,
    p12 = end2 - Vector::AXIS_Z * m_R,
    pmin = end1.componentMin(p1)
      .componentMin(p2)
      .componentMin(p3)
      .componentMin(p4)
      .componentMin(p5)
      .componentMin(p6)
      .componentMin(p7)
      .componentMin(p8)
      .componentMin(p9)
      .componentMin(p10)
      .componentMin(p11)
      .componentMin(p12),
    pmax = end1.componentMax(p1)
      .componentMax(p2)
      .componentMax(p3)
      .componentMax(p4)
      .componentMax(p5)
      .componentMax(p6)
      .componentMax(p7)
      .componentMax(p8)
      .componentMax(p9)
      .componentMax(p10)
      .componentMax(p11)
      .componentMax(p12);
  return Box(pmin, pmax);
}
