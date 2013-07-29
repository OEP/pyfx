#ifndef UNIFORMPRN_H__
#define UNIFORMPRN_H__

#include <random>
#include "PRN.h"

namespace vr
{
  class UniformPRN : public PRN
  {
    public:
      UniformPRN() : m_Distribution(std::uniform_real_distribution<float>(0.0f,1.0f)) {}
      ~UniformPRN() {}

      const float eval();
      void setSeed(const unsigned long);
    protected:
      std::mt19937_64 m_Engine;
      std::uniform_real_distribution<float> m_Distribution;
  };
}
#endif
