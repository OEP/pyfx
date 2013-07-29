#include "UniformPRN.h"
using namespace vr;

const float
UniformPRN::eval()
{
  return m_Distribution(m_Engine);
}

void
UniformPRN::setSeed(const unsigned long seed)
{
  m_Engine.seed(seed);
}
