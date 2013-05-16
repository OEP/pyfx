#ifndef MATRIX_H_
#define MATRIX_H_

#include "Vector.h"

namespace vr
{
  class Vector;
  class Matrix
  {
    private:
      double mm[3][3];
    public:
      static const Matrix
        EYE,
        PAULI[3];
      
      Matrix(const double v = 0.0);
      Matrix(
        const double v00, const double v01, const double v02,
        const double v10, const double v11, const double v12,
        const double v20, const double v21, const double v22
      );

      static const Matrix rotation(const Vector &axis, const double theta);

      // Queries
      const Vector row(size_t) const;
      const Vector column(size_t) const;

      // Linalg named operators
      const Vector dot(const Vector &v) const;
      const Matrix dot(const Matrix &m) const;
      const Matrix transpose() const;
      const Matrix adjoint() const;
      const Matrix inverse() const;
      const double determinant() const;

      // Scaling operations
      const Matrix scale(const float) const;

      // Operator overloads
      const Matrix operator*(const float) const;
      const Vector operator*(const Vector &v) const;
      const Matrix operator*(const Matrix &v) const;
      const Matrix operator/(const float) const;
      const Matrix operator+(const Matrix&) const;
      const Matrix operator-(const Matrix&) const;
      const double operator()(size_t i, size_t j) const;
      const bool operator==(const Matrix&) const;

  };
  
  const Matrix operator*(const float, const Matrix &m);
}

#endif
