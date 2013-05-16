/*
#include <ostream>
#include <sstream>
#include <cmath>
#include <cfloat>
*/
#include <sstream>

#include "Vector.h"
#include "Matrix.h"
#include "macros.h"

using namespace vr;

const Vector
vector::rotate(const Vector &a, const Vector &axis, const double theta)
{
  return
    a * cos(theta) +
    axis * a.dot(axis) * (1 - cos(theta)) +
    a.cross(axis) * sin(theta);
}

const Vector vector::replace(const Vector &in, const int i, const double v) 
{
  switch(i)
  {
    case 0: return Vector(v, in.y(), in.z());
    case 1: return Vector(in.x(), v, in.z());
    case 2: return Vector(in.x(), in.y(), v);
  }
  return in;
}

const Vector vector::componentProduct(const Vector &a, const Vector &b)
{
  return Vector(
    a.x() * b.x(),
    a.y() * b.y(),
    a.z() * b.z()
  );
}

const std::string vector::toString(const Vector &v)
{
  std::stringstream ss;
  ss << v.x() << " " << v.y() << " " << v.z();
  return ss.str();
}

const Vector vector::lerpRotate(const Vector &a, const Vector &b, const double q)
{
  const Vector bxa = b.cross(a);
  const Vector au = a.unit();
  const double m1 = a.length();
  const double m2 = b.length();

  if (bxa.length() == 0)
  {
    return au * LERP(m1, m2, q);
  }

  const Vector axis = bxa.unit();
  const double theta = acos(a.dot(b) / (m1 * m2));
  return rotate(au, axis, q * theta) * LERP(m1, m2, q);
}


/*
const Vector vector::componentMin(const Vector &a, const Vector &b) const
{
  return Vector(
    std::min(a.x(), b.x()),
    std::min(a.y(), b.y()),
    std::min(a.z(), b.z()));
}

const Vector vector::componentMax(const Vector &a, const Vector &b) const
{
  return Vector(
    std::max(a.x(), b.x()),
    std::max(a.y(), b.y()),
    std::max(a.z(), b.z()));
}
*/

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
  vector::UX = Vector(1,0,0),
  vector::UY = Vector(0,1,0),
  vector::UZ = Vector(0,0,1);

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

const Vector Vector::unit() const
{
  const double mag = length();
  return Vector(X() / mag, Y() / mag, Z() / mag);
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
  return std::sqrt(X() * X() + Y() * Y() + Z() * Z());
}

const Vector Vector::componentMin(const Vector &other) const
{
  return Vector(
    std::min(X(), other.x()),
    std::min(Y(), other.y()),
    std::min(Z(), other.z()));
}

const Vector Vector::componentMax(const Vector &other) const
{
  return Vector(
    std::max(X(), other.x()),
    std::max(Y(), other.y()),
    std::max(Z(), other.z()));
}

const Vector Vector::componentProduct(const Vector &other) const
{
  return Vector(
    X() * other.x(),
    Y() * other.y(),
    Z() * other.z());
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
    X() + other.x(),
    Y() + other.y(),
    Z() + other.z());
}

const Vector Vector::operator- (const Vector &other) const
{
  return Vector(
    X() - other.x(),
    Y() - other.y(),
    Z() - other.z());
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
  return Vector(X() / v.x(), Y() / v.y(), Z() / v.z());
}

const Vector Vector::operator* (const double amt) const
{
  return Vector(X() * amt, Y() * amt, Z() * amt);
}

const double Vector::operator* (const Vector& other) const
{
  return X() * other.x() + Y() * other.y() + Z() * other.z();
}

const Vector Vector::operator^ (const Vector& other) const
{
  return Vector(
    Y() * other.z() - Z() * other.y(),
    -( X() * other.z() - Z() * other.x()),
    X() * other.y() - Y() * other.x());
}

const bool Vector::operator== (const Vector& v) const
{
  return X() == v.x() && Y() == v.y() && Z() == v.z();
}

const bool Vector::operator!= (const Vector& v) const
{
  return X() != v.x() || Y() != v.y() || Z() != v.z();
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
