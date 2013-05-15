#include "Matrix.h"

using namespace vr;

const Matrix matrix::rotation(const Vector& axis, const double angle)
{
   using openvdb::OPENVDB_VERSION_NAME::math::outerProduct;

   const double cosa = cos(angle);
   const double sina = sin(angle);
   const Vector ax = axis / sqrt( axis.dot(axis) ); 
   const Matrix op = outerProduct( ax, ax );
   Matrix result = matrix::EYE * cosa 
                 + op * ( 1.0 - cosa )
                 + matrix::PAULI[0] * axis[0] * sina 
                 + matrix::PAULI[1] * axis[1] * sina 
                 + matrix::PAULI[2] * axis[2] * sina;
   return result;
}
