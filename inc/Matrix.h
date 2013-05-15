#ifndef MATRIX_H_
#define MATRIX_H_

#include "Vector.h"

namespace vr
{
  class Matrix
  {
    public:
      Matrix(float v = 0.0);
      Matrix(
        double,
        double,
        double,
        double,
        double,
        double,
        double,
        double,
        double);

      static const Matrix rotation(const Vector &axis, const double theta);
      
      const Vector operator* (const Vector& v) const;
      const Matrix operator* (const Matrix& v) const;
      const Matrix operator+ (const Matrix& v) const;
      const Matrix operator- (const Matrix& v) const;
      
      const bool operator== (const Matrix& v) const;

      const Matrix inverse() const;
      const Matrix transpose() const;
  };
  
  const Matrix operator* (const float f, const Matrix& v);
}

#endif
