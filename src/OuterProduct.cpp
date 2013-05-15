#include "OuterProduct.h"

using namespace vr;

const Matrix OuterProduct::eval(const Vector &p) const
{
  using openvdb::OPENVDB_VERSION_NAME::math::outerProduct;

  const Vector
    v1 = m_V1->eval(p),
    v2 = m_V2->eval(p);

  return outerProduct(v1, v2);
}

/*
const int OuterProduct::grad(const Vector &p) const
{
  throw std::exception("Not implemented.");
}

const Box OuterProduct::getBBox() const
{
  throw std::exception("Not implemented.");
}
*/
