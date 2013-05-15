#ifndef UNIFORMPRN_H__
#define UNIFORMPRN_H__

#include "PRN.h"

namespace vr
{
  class UniformPRN : public PRN
  {
    public:
      UniformPRN() {}
      ~UniformPRN() {}

      const float eval();
      void setSeed(const unsigned long);
    protected:
  };
}
#endif
