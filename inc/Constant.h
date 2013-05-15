#ifndef CONSTANT_H_
#define CONSTANT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class Constant : public Volume<U,V>
  {
    private:
      const U m_Data;
    protected:
    public:
      Constant(const U &data)
        : m_Data(data) {}

      const U eval(const Vector&) const
      {
        return m_Data;
      }

      const V grad(const Vector&) const
      {
        V data;
        return data;
      }

      const Box getBBox() const
      {
        return Box::NIL;
      }
  };
}

#endif
