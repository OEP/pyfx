#ifndef VECTOR_H_
#define VECTOR_H_

#include <ostream>

#include "Matrix.h"

namespace vr
{
  class Matrix;
  class Vector
  {
    private:
      double m_XYZ[3];

    protected:
    public:
      static const int
        COMPONENT_X, COMPONENT_Y, COMPONENT_Z;

      static const Vector
        MAX, MIN,
        UX, UY, UZ;

      Vector(const double x = 0.0);
      Vector(const Vector&);
      Vector(const double x, const double y, const double z);

      const double length() const;
      const Vector unit() const;
      void normalize();

      // Component operators
      const Vector minComponent(const Vector&) const;
      const Vector maxComponent(const Vector&) const;
      const Vector replace(const int i, const double x) const;

      // Component accessors
      const double x() const;
      const double y() const;
      const double z() const;

      // Rotation methods
      const Vector rotate(const Vector&, const double) const;
      const Vector lerpRotate(const Vector&, const double) const;

      const bool equals(const Vector&) const;
      
      // Scaling methods
      const Vector scale(const Vector&) const;
      const Vector scale(const float) const;

      // Named linalg methods
      const double dot(const Vector &v) const;
      const Vector dot(const Matrix &m) const;
      const Vector cross(const Vector &v) const;
      const Matrix outerProduct(const Vector&) const;

      const Vector operator- ()                const;
      const Vector operator- (const Vector& v) const;
      const Vector operator/ (const double)    const;
      const Vector operator/ (const Vector& v) const;
      const Vector operator+ (const Vector& v) const;
      const double operator* (const Vector& v) const;
      const Vector operator* (const double)    const;
      const Vector operator^ (const Vector& v) const;

      const double operator[](const int) const;

      const bool operator== (const Vector& v) const;
      const bool operator!= (const Vector& v) const;
      const bool operator>= (const Vector& v) const;
      const bool operator<= (const Vector& v) const;
      const bool operator>  (const Vector& v) const;
      const bool operator<  (const Vector& v) const;

      const std::string toString() const;
      const std::string __str__() const;
      const std::string __repr__() const;

      friend std::ostream& operator<< (std::ostream& output, const Vector& v);
      friend const Vector operator* (const double, const Vector& v);
  };
  
  std::ostream& operator<< (std::ostream& output, const Vector& v);
  const Vector operator*(const double, const Vector& v);
}


#endif
