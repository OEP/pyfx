#include "Ray.h"
#include "Vector.h"

using namespace vr;

Ray::Ray(const Ray &other)
  : m_Origin(other.m_Origin), m_Direction(other.m_Direction)
{
  update();
}

Ray::Ray(const Vector &p, const Vector &direction)
  : m_Origin(p), m_Direction(direction.unit())
{
  update();
}

void Ray::update()
{
  m_InvD = Vector(1 / m_Direction[0], 1 / m_Direction[1], 1 / m_Direction[2]);
  m_Sign[0] = m_InvD[0] < 0;
  m_Sign[1] = m_InvD[1] < 0;
  m_Sign[2] = m_InvD[2] < 0;
}

const Vector& Ray::origin() const
{
  return m_Origin;
}

const Vector& Ray::direction() const
{
  return m_Direction;
}

const Vector Ray::trace(const double t) const
{
  return m_Origin + (t * m_Direction);
}

const Vector& Ray::inverseDirection() const
{
  return m_InvD;
}

int Ray::sign(const int i) const
{
  return m_Sign[i];
}

ostream& vr::operator<<(ostream& out, const Ray &r)
{
  out << "[" << r.origin() << ", " << r.direction() << "]";
  return out;
}
