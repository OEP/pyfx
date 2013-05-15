#include <ostream>
#include <sstream>
#include <cmath>
#include <cfloat>

#include "Vector2D.h"
#include "macros.h"

using namespace std;
using namespace vr;

const int
  Vector2D::COMPONENT_X = 0,
  Vector2D::COMPONENT_Y = 1;

const Vector2D
  Vector2D::MAX = Vector2D(DBL_MAX, DBL_MAX),
  Vector2D::MIN = Vector2D(-DBL_MAX, -DBL_MAX),
  Vector2D::ZEROS = Vector2D(0,0),
  Vector2D::ONES  = Vector2D(1,1),
  Vector2D::AXIS_X = Vector2D(1,0),
  Vector2D::AXIS_Y = Vector2D(0,1);

Vector2D::Vector2D()
{
  m_XY[0] = m_XY[1] = 0.0;
}

Vector2D::Vector2D(const Vector2D &other)
{
  m_XY[0] = other.m_XY[0];
  m_XY[1] = other.m_XY[1];
}

Vector2D::Vector2D(const double x, const double y)
{
  m_XY[0] = x;
  m_XY[1] = y;
}

const double Vector2D::x() const
{
  return m_XY[0];
}

const double Vector2D::y() const
{
  return m_XY[1];
}


const Vector2D Vector2D::unit() const
{
  const double mag = length();
  return (*this) / mag;
}

void Vector2D::normalize()
{
  const double mag = length();
  m_XY[0] /= mag;
  m_XY[1] /= mag;
}

const double Vector2D::length() const
{
  return std::sqrt(x() * x() + y() * y());
}

const Vector2D Vector2D::replace(const int i, const double v) const
{
  switch(i)
  {
    case 0: return Vector2D(v, y());
    case 1: return Vector2D(x(), v);
  }
  return *this;
}

const Vector2D Vector2D::componentMin(const Vector2D &other) const
{
  return Vector2D(
    std::min(x(), other.x()),
    std::min(y(), other.y()));
}

const Vector2D Vector2D::componentMax(const Vector2D &other) const
{
  return Vector2D(
    std::max(x(), other.x()),
    std::max(y(), other.y()));
}

const Vector2D Vector2D::componentProduct(const Vector2D &other) const
{
  return Vector2D(
    x() * other.x(),
    y() * other.y());
}

const bool Vector2D::equals(const Vector2D &other) const
{
  return *this == other;
}

const Vector2D Vector2D::operator+ (const Vector2D &other) const
{
  return Vector2D(
    x() + other.x(),
    y() + other.y());
}

const Vector2D Vector2D::operator- (const Vector2D &other) const
{
  return Vector2D(
    x() - other.x(),
    y() - other.y());
}

const Vector2D Vector2D::operator-() const
{
  return Vector2D(
    -m_XY[0],
    -m_XY[1]);
}

const Vector2D Vector2D::operator/ (const double amt) const
{
  return Vector2D(x() / amt, y() / amt);
}

const Vector2D Vector2D::operator/ (const Vector2D &v) const
{
  return Vector2D(x() / v.x(), y() / v.y());
}

const Vector2D Vector2D::operator* (const double amt) const
{
  return Vector2D(x() * amt, y() * amt);
}

const double Vector2D::operator* (const Vector2D& other) const
{
  return x() * other.x() + y() * other.y();
}

const bool Vector2D::operator== (const Vector2D& v) const
{
  return x() == v.x() && y() == v.y();
}

const bool Vector2D::operator!= (const Vector2D& v) const
{
  return x() != v.x() || y() != v.y();
}

const bool Vector2D::operator>= (const Vector2D& v) const
{
  return length() >= v.length();
}

const bool Vector2D::operator<= (const Vector2D& v) const
{
  return length() <= v.length();
}

const bool Vector2D::operator< (const Vector2D& v) const
{
  return length() < v.length();
}

const bool Vector2D::operator> (const Vector2D& v) const
{
  return length() > v.length();
}

const double Vector2D::operator[](const int i) const
{
  return m_XY[i];
}

ostream& vr::operator<< (ostream& out, const vr::Vector2D &v)
{
  out << "<" <<
    v.x() << ", " <<
    v.y() << ">";
  return out;
}

const Vector2D vr::operator*(const double t, const Vector2D &v)
{
  return Vector2D(t * v.x(), t * v.y());
}

const std::string Vector2D::__str__() const
{
  return toString();
}

const std::string Vector2D::__repr__() const
{
  std::stringstream ss;
  ss << "(" << x() << ", " << y() << ")";
  return ss.str();
}

const std::string Vector2D::toString() const
{
  std::stringstream ss;
  ss << x() << " " << y();
  return ss.str();
}
 
const float vr::line_distance(
  const Vector2D &p,
  const Vector2D &v,
  const Vector2D &w,
  const int bounded_flags)
{
  const Vector2D vmw = v - w;
  const float l2 = vmw * vmw;

  if(l2 == 0.0)
  {
    return (p - v).length();
  }

  const float t = ((p - v) * (w - v)) / l2;

  if(((bounded_flags & 0x1) != 0) && t < 0.0)
  {
    return (p-v).length();
  }
  else if(((bounded_flags & 0x2) != 0) && t > 1.0)
  {
    return (p-w).length();
  }

  const Vector2D projection = v + t * (w - v);
  return (p - projection).length();
}
