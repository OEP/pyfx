#include "Matrix.h"

using namespace vr;

const Matrix Matrix::EYE =
  Matrix(1, 0, 0,
         0, 1, 0,
         0, 0, 1);

const Matrix Matrix::PAULI[3] =
{
  Matrix( 0, 0, 0,
          0, 0, 1,
          0,-1, 0 ),
  Matrix( 0, 0,-1,
          0, 0, 0,
          1, 0, 0 ),
  Matrix( 0, 1, 0,
         -1, 0, 0,
          0, 0, 0 )
};

Matrix::Matrix(const double v)
{
  for(size_t i = 0; i < 3; i++)
  {
    for(size_t j = 0; j < 3; j++)
    {
      this->mm[i][j] = v;
    }
  }
}

Matrix::Matrix(
  const double v00, const double v01, const double v02,
  const double v10, const double v11, const double v12,
  const double v20, const double v21, const double v22)
{
  this->mm[0][0] = v00;
  this->mm[0][1] = v01;
  this->mm[0][2] = v02;
  this->mm[1][0] = v10;
  this->mm[1][1] = v11;
  this->mm[1][2] = v12;
  this->mm[2][0] = v20;
  this->mm[2][1] = v21;
  this->mm[2][2] = v22;
}

const Matrix Matrix::rotation(const Vector& axis, const double angle)
{
   const double cosa = cos(angle);
   const double sina = sin(angle);
   const Vector ax = axis / sqrt( axis.dot(axis) ); 
   const Matrix op = ax.outerProduct( ax );
   Matrix result = Matrix::EYE * cosa 
                 + op * ( 1.0 - cosa )
                 + Matrix::PAULI[0] * axis[0] * sina 
                 + Matrix::PAULI[1] * axis[1] * sina 
                 + Matrix::PAULI[2] * axis[2] * sina;
   return result;
}

const Matrix
Matrix::scale(const float f) const
{
  return Matrix(
    this->mm[0][0] * f,
    this->mm[0][1] * f,
    this->mm[0][2] * f,
    this->mm[1][0] * f,
    this->mm[1][1] * f,
    this->mm[1][2] * f,
    this->mm[2][0] * f,
    this->mm[2][1] * f,
    this->mm[2][2] * f
  );
}

const Matrix Matrix::transpose() const
{
  return Matrix(
    this->mm[0][0],
    this->mm[1][0],
    this->mm[2][0],
    this->mm[0][1],
    this->mm[1][1],
    this->mm[2][1],
    this->mm[0][2],
    this->mm[1][2],
    this->mm[2][2]
  );
}

const Matrix Matrix::adjoint() const
{
  // Adapted from OpenVDB code base.
  return Matrix(
    this->mm[1][1] * this->mm[2][2] - this->mm[1][2] * this->mm[2][1],
    this->mm[0][2] * this->mm[2][1] - this->mm[0][1] * this->mm[2][2],
    this->mm[0][1] * this->mm[1][2] - this->mm[0][2] * this->mm[1][1],
    this->mm[1][2] * this->mm[2][0] - this->mm[1][0] * this->mm[2][2],
    this->mm[0][0] * this->mm[2][2] - this->mm[0][2] * this->mm[2][0],
    this->mm[0][2] * this->mm[1][0] - this->mm[0][0] * this->mm[1][2],
    this->mm[1][0] * this->mm[2][1] - this->mm[1][1] * this->mm[2][0],
    this->mm[0][1] * this->mm[2][0] - this->mm[0][0] * this->mm[2][1],
    this->mm[0][0] * this->mm[1][1] - this->mm[0][1] * this->mm[1][0]
  );
}

const double Matrix::determinant() const
{
  return
      this->mm[0][0] * (this->mm[1][1] * this->mm[2][2] - this->mm[1][2] * this->mm[2][1])
    - this->mm[0][1] * (this->mm[1][0] * this->mm[2][2] - this->mm[1][2] * this->mm[2][0])
    + this->mm[0][2] * (this->mm[1][0] * this->mm[2][1] - this->mm[1][1] * this->mm[2][0]);
}

const Matrix Matrix::inverse() const
{
  // Adapted from OpenVDB code base.
  const Matrix adj = this->adjoint();
  const double det = 
    adj.mm[0][0] * this->mm[0][0] +
    adj.mm[0][1] * this->mm[1][0] +
    adj.mm[0][2] * this->mm[2][0];
  
  // Tolerance check removed here.
  if(det == 0.0)
  {
    throw std::domain_error("Matrix is singular.");
  }

  return adj * (1.0 / det);
}

const Vector Matrix::dot(const Vector &v) const
{
  return Vector(
    v.x() * this->mm[0][0] + v.y() * this->mm[0][1] + v.z() * this->mm[0][2],
    v.x() * this->mm[1][0] + v.y() * this->mm[1][1] + v.z() * this->mm[1][2],
    v.x() * this->mm[2][0] + v.y() * this->mm[2][1] + v.z() * this->mm[2][2]
  );
}

const Matrix Matrix::dot(const Matrix &m) const
{
  return Matrix(
    this->row(0).dot(m.column(0)),
    this->row(0).dot(m.column(1)),
    this->row(0).dot(m.column(2)),
    this->row(1).dot(m.column(0)),
    this->row(1).dot(m.column(1)),
    this->row(1).dot(m.column(2)),
    this->row(2).dot(m.column(0)),
    this->row(2).dot(m.column(1)),
    this->row(2).dot(m.column(2))
  );
}

const Matrix Matrix::operator*(const Matrix & m) const
{
  return this->dot(m);
}

const bool Matrix::operator==(const Matrix &m) const
{
  bool result = true;
  for(size_t i = 0; result && i < 3; i++)
  {
    for(size_t j = 0; result && j < 3; j++)
    {
      result &= this->mm[i][j] == m(i,j);
    }
  }
  return result;
}

const Vector Matrix::row(size_t i) const
{
  return Vector(
    this->mm[i][0],
    this->mm[i][1],
    this->mm[i][2]
  );
}

const Vector Matrix::column(size_t j) const
{
  return Vector(
    this->mm[0][j],
    this->mm[1][j],
    this->mm[2][j]
  );
}

const Vector Matrix::operator*(const Vector &v) const
{
  return this->dot(v);
}

const Matrix Matrix::operator*(const float f) const
{
  return this->scale(f);
}

const Matrix Matrix::operator/(const float f) const
{
  return this->scale(1.0 / f);
}

const Matrix Matrix::operator+(const Matrix &o) const
{
  return Matrix(
    this->mm[0][0] + o.mm[0][0],
    this->mm[0][1] + o.mm[0][1],
    this->mm[0][2] + o.mm[0][2],
    this->mm[1][0] + o.mm[1][0],
    this->mm[1][1] + o.mm[1][1],
    this->mm[1][2] + o.mm[1][2],
    this->mm[2][0] + o.mm[2][0],
    this->mm[2][1] + o.mm[2][1],
    this->mm[2][2] + o.mm[2][2]
  );
}

const Matrix Matrix::operator-(const Matrix &o) const
{
  return Matrix(
    this->mm[0][0] - o.mm[0][0],
    this->mm[0][1] - o.mm[0][1],
    this->mm[0][2] - o.mm[0][2],
    this->mm[1][0] - o.mm[1][0],
    this->mm[1][1] - o.mm[1][1],
    this->mm[1][2] - o.mm[1][2],
    this->mm[2][0] - o.mm[2][0],
    this->mm[2][1] - o.mm[2][1],
    this->mm[2][2] - o.mm[2][2]
  );
}

const double Matrix::operator()(size_t i, size_t j) const
{
  return this->mm[i][j];
}

const Matrix vr::operator*(const float f, const Matrix &m)
{
  return m.scale(f);
}

const std::string Matrix::__str__() const
{
  return toString();
}

const std::string Matrix::__repr__() const
{
  return toString();
}

const std::string Matrix::toString() const
{
  std::stringstream ss;
  ss << "(" <<
    this->row(0) << ",\n" <<
    this->row(1) << ",\n" <<
    this->row(2) << ")";
  return ss.str();
}
