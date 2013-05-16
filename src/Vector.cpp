#include <ostream>
#include <sstream>
#include <cmath>
#include <cfloat>

#include "Vector.h"
#include "Matrix.h"
#include "macros.h"

using namespace std;
using namespace vr;

const int
  Vector::COMPONENT_X = 0,
  Vector::COMPONENT_Y = 1,
  Vector::COMPONENT_Z = 2;

const Vector
  Vector::MAX = Vector(DBL_MAX, DBL_MAX, DBL_MAX),
  Vector::MIN = Vector(-DBL_MAX, -DBL_MAX, -DBL_MAX),
  Vector::ZEROS = Vector(0,0,0),
  Vector::ONES  = Vector(1,1,1),
  Vector::AXIS_X = Vector(1,0,0),
  Vector::AXIS_Y = Vector(0,1,0),
  Vector::AXIS_Z = Vector(0,0,1);

Vector::Vector(const double x)
{
  m_XYZ[0] = m_XYZ[1] = m_XYZ[2] = x;
}

Vector::Vector(const Vector &other)
{
  m_XYZ[0] = other.m_XYZ[0];
  m_XYZ[1] = other.m_XYZ[1];
  m_XYZ[2] = other.m_XYZ[2];
}

Vector::Vector(const double x, const double y, const double z)
{
  m_XYZ[0] = x;
  m_XYZ[1] = y;
  m_XYZ[2] = z;
}

const double Vector::x() const
{
  return m_XYZ[0];
}

const double Vector::y() const
{
  return m_XYZ[1];
}

const double Vector::z() const
{
  return m_XYZ[2];
}

const Vector Vector::rotate(const Vector &axis, const double theta) const
{
  return
    *this * cos(theta) +
    axis * (*this * axis) * (1 - cos(theta)) +
    (*this ^ axis) * sin(theta);
}

const Vector Vector::lerpRotate(const Vector &b, const double q) const
{
  const Vector bxa = b ^ (*this);
  const Vector au = unit();
  const double m1 = length();
  const double m2 = b.length();

  if (bxa.length() == 0)
  {
    return au * LERP(m1, m2, q);
  }

  const Vector axis = bxa.unit();
  const double theta = acos(((*this) * b) / (m1 * m2));
  return au.rotate(axis, q * theta) * LERP(m1, m2, q);
}

const Vector Vector::unit() const
{
  const double mag = length();
  return Vector(x() / mag, y() / mag, z() / mag);
}

void Vector::normalize()
{
  const double mag = length();
  m_XYZ[0] /= mag;
  m_XYZ[1] /= mag;
  m_XYZ[2] /= mag;
}

const double Vector::length() const
{
  return std::sqrt(x() * x() + y() * y() + z() * z());
}

const Vector Vector::replace(const int i, const double v) const
{
  switch(i)
  {
    case 0: return Vector(v, y(), z());
    case 1: return Vector(x(), v, z());
    case 2: return Vector(x(), y(), v);
  }
  return *this;
}

const Vector Vector::minComponent(const Vector &other) const
{
  return Vector(
    std::min(x(), other.x()),
    std::min(y(), other.y()),
    std::min(z(), other.z()));
}

const Vector Vector::maxComponent(const Vector &other) const
{
  return Vector(
    std::max(x(), other.x()),
    std::max(y(), other.y()),
    std::max(z(), other.z()));
}

const Matrix Vector::outerProduct(const Vector &other) const
{
  return Matrix(
    (*this)[0] * other[0], (*this)[0] * other[1], (*this)[0] * other[2],
    (*this)[1] * other[0], (*this)[1] * other[1], (*this)[1] * other[2],
    (*this)[2] * other[0], (*this)[2] * other[1], (*this)[2] * other[2]
  );
}

const Vector Vector::operator+ (const Vector &other) const
{
  return Vector(
    x() + other.x(),
    y() + other.y(),
    z() + other.z());
}

const Vector Vector::operator- (const Vector &other) const
{
  return Vector(
    x() - other.x(),
    y() - other.y(),
    z() - other.z());
}

const Vector Vector::operator-() const
{
  return Vector(
    -m_XYZ[0],
    -m_XYZ[1],
    -m_XYZ[2]);
}

const Vector Vector::operator/ (const double amt) const
{
  return this->scale(1.0 / amt);
}

const Vector Vector::operator/ (const Vector &v) const
{
  return Vector(x() / v.x(), y() / v.y(), z() / v.z());
}

const Vector Vector::scale (const float amt) const
{
  return Vector(x() * amt, y() * amt, z() * amt);
}

const Vector Vector::scale (const Vector &amt) const
{
  return Vector(x() * amt.x(), y() * amt.y(), z() * amt.z());
}

const Vector Vector::operator* (const double amt) const
{
  return this->scale(amt);
}

const double Vector::dot (const Vector& other) const
{
  return x() * other.x() + y() * other.y() + z() * other.z();
}

const double Vector::operator* (const Vector& other) const
{
  return this->dot(other);
}

const Vector Vector::cross (const Vector& other) const
{
  return Vector(
    y() * other.z() - z() * other.y(),
    -( x() * other.z() - z() * other.x()),
    x() * other.y() - y() * other.x());
}

const Vector Vector::operator^ (const Vector& other) const
{
  return this->cross(other);
}

const bool Vector::equals(const Vector &v) const
{
  return x() == v.x() && y() == v.y() && z() == v.z();
}

const bool Vector::operator== (const Vector& v) const
{
  return this->equals(v);
}

const bool Vector::operator!= (const Vector& v) const
{
  return !this->equals(v);
}

const bool Vector::operator>= (const Vector& v) const
{
  return length() >= v.length();
}

const bool Vector::operator<= (const Vector& v) const
{
  return length() <= v.length();
}

const bool Vector::operator< (const Vector& v) const
{
  return length() < v.length();
}

const bool Vector::operator> (const Vector& v) const
{
  return length() > v.length();
}

const double Vector::operator[](const int i) const
{
  return m_XYZ[i];
}

ostream& vr::operator<< (ostream& out, const vr::Vector &v)
{
  out << "<" <<
    v.x() << ", " <<
    v.y() << ", " <<
    v.z() << ">";
  return out;
}

const Vector vr::operator*(const double t, const Vector &v)
{
  return Vector(t * v.x(), t * v.y(), t * v.z());
}

const std::string Vector::__str__() const
{
  return toString();
}

const std::string Vector::__repr__() const
{
  std::stringstream ss;
  ss << "(" << x() << ", " << y() << ", " << z() << ")";
  return ss.str();
}

const std::string Vector::toString() const
{
  std::stringstream ss;
  ss << x() << " " << y() << " " << z();
  return ss.str();
}
