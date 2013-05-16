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

      const Matrix rotation(const Vector &axis, const double theta);

      // Scaling operations
      const Matrix scale(const float) const;

      // Operator overloads
      const Matrix operator*(const float) const;
      const Matrix operator/(const float) const;
      const Matrix operator+(const Matrix&) const;
  };
}

#endif
