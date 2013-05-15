/*
#include <ostream>
#include <sstream>
#include <cmath>
#include <cfloat>
*/

#include "Vector.h"
#include "Matrix.h"
#include "macros.h"

using namespace vr;

Vector::Vector(const Vec3Double &v)
{
  init(v.x(), v.y(), v.z());
}

Vector::Vector(const double c)
{
  init(c,c,c);
}

Vector::Vector(const double x, const double y, const double z)
{
  init(x,y,z);
}

const Vector vector::replace(const Vector &in, const int i, const double v) const
{
  switch(i)
  {
    case 0: return Vector(v, in.y(), in.z());
    case 1: return Vector(in.x(), v, in.z());
    case 2: return Vector(in.x(), in.y(), v);
  }
  return *this;
}


/*
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

Vector::Vector()
{
  m_XYZ[0] = m_XYZ[1] = m_XYZ[2] = 0.0;
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

const double Vector::X() const
{
  return m_XYZ[0];
}

const double Vector::Y() const
{
  return m_XYZ[1];
}

const double Vector::Z() const
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
  const double m1 = magnitude();
  const double m2 = b.magnitude();

  if (bxa.magnitude() == 0)
  {
    return au * LERP(m1, m2, q);
  }

  const Vector axis = bxa.unit();
  const double theta = acos(((*this) * b) / (m1 * m2));
  return au.rotate(axis, q * theta) * LERP(m1, m2, q);
}

const Vector Vector::unit() const
{
  const double mag = magnitude();
  return Vector(X() / mag, Y() / mag, Z() / mag);
}

void Vector::normalize()
{
  const double mag = magnitude();
  m_XYZ[0] /= mag;
  m_XYZ[1] /= mag;
  m_XYZ[2] /= mag;
}

const double Vector::magnitude() const
{
  return std::sqrt(X() * X() + Y() * Y() + Z() * Z());
}

const Vector Vector::componentMin(const Vector &other) const
{
  return Vector(
    std::min(X(), other.X()),
    std::min(Y(), other.Y()),
    std::min(Z(), other.Z()));
}

const Vector Vector::componentMax(const Vector &other) const
{
  return Vector(
    std::max(X(), other.X()),
    std::max(Y(), other.Y()),
    std::max(Z(), other.Z()));
}

const Vector Vector::componentProduct(const Vector &other) const
{
  return Vector(
    X() * other.X(),
    Y() * other.Y(),
    Z() * other.Z());
}

const bool Vector::equals(const Vector &other) const
{
  return *this == other;
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
    X() + other.X(),
    Y() + other.Y(),
    Z() + other.Z());
}

const Vector Vector::operator- (const Vector &other) const
{
  return Vector(
    X() - other.X(),
    Y() - other.Y(),
    Z() - other.Z());
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
  return Vector(X() / amt, Y() / amt, Z() / amt);
}

const Vector Vector::operator/ (const Vector &v) const
{
  return Vector(X() / v.X(), Y() / v.Y(), Z() / v.Z());
}

const Vector Vector::operator* (const double amt) const
{
  return Vector(X() * amt, Y() * amt, Z() * amt);
}

const double Vector::operator* (const Vector& other) const
{
  return X() * other.X() + Y() * other.Y() + Z() * other.Z();
}

const Vector Vector::operator^ (const Vector& other) const
{
  return Vector(
    Y() * other.Z() - Z() * other.Y(),
    -( X() * other.Z() - Z() * other.X()),
    X() * other.Y() - Y() * other.X());
}

const bool Vector::operator== (const Vector& v) const
{
  return X() == v.X() && Y() == v.Y() && Z() == v.Z();
}

const bool Vector::operator!= (const Vector& v) const
{
  return X() != v.X() || Y() != v.Y() || Z() != v.Z();
}

const bool Vector::operator>= (const Vector& v) const
{
  return magnitude() >= v.magnitude();
}

const bool Vector::operator<= (const Vector& v) const
{
  return magnitude() <= v.magnitude();
}

const bool Vector::operator< (const Vector& v) const
{
  return magnitude() < v.magnitude();
}

const bool Vector::operator> (const Vector& v) const
{
  return magnitude() > v.magnitude();
}

const double Vector::operator[](const int i) const
{
  return m_XYZ[i];
}

ostream& vr::operator<< (ostream& out, const vr::Vector &v)
{
  out << "<" <<
    v.X() << ", " <<
    v.Y() << ", " <<
    v.Z() << ">";
  return out;
}

const Vector vr::operator*(const double t, const Vector &v)
{
  return Vector(t * v.X(), t * v.Y(), t * v.Z());
}

const std::string Vector::__str__() const
{
  return toString();
}

const std::string Vector::__repr__() const
{
  std::stringstream ss;
  ss << "(" << X() << ", " << Y() << ", " << Z() << ")";
  return ss.str();
}

const std::string Vector::toString() const
{
  std::stringstream ss;
  ss << X() << " " << Y() << " " << Z();
  return ss.str();
}
*/
