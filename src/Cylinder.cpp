#include <cmath>

#include "Cylinder.h"

using namespace vr;

const float Cylinder::eval(const Vector &p) const
{
  const Vector xperp = p - (p.dot(m_N) * m_N);
  const float f = m_R - xperp.length();
  const float dist = fabs(p.dot(m_N));

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
    p1  = end1 + Vector::UX * m_R,
    p2  = end1 + Vector::UY * m_R,
    p3  = end1 + Vector::UZ * m_R,
    p4  = end1 - Vector::UX * m_R,
    p5  = end1 - Vector::UY * m_R,
    p6  = end1 - Vector::UZ * m_R,
    p7  = end2 + Vector::UX * m_R,
    p8  = end2 + Vector::UY * m_R,
    p9  = end2 + Vector::UZ * m_R,
    p10 = end2 - Vector::UX * m_R,
    p11 = end2 - Vector::UY * m_R,
    p12 = end2 - Vector::UZ * m_R,
    pmin =
      end1.minComponent(p1)
        .minComponent(p2)
        .minComponent(p3)
        .minComponent(p4)
        .minComponent(p5)
        .minComponent(p6)
        .minComponent(p7)
        .minComponent(p8)
        .minComponent(p9)
        .minComponent(p10)
        .minComponent(p11)
        .minComponent(p12),
    pmax =
      end1.maxComponent(p1)
        .maxComponent(p2)
        .maxComponent(p3)
        .maxComponent(p4)
        .maxComponent(p5)
        .maxComponent(p6)
        .maxComponent(p7)
        .maxComponent(p8)
        .maxComponent(p9)
        .maxComponent(p10)
        .maxComponent(p11)
        .maxComponent(p12);
  return Box(pmin, pmax);
}
