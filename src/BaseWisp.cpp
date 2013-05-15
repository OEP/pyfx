#include "BaseWisp.h"

using namespace vr;

void BaseWisp::initialize()
{
  Box *b = m_Box;
  if(b == NULL)
  {
    m_GarbageBox = new Box(getBBox());
    b = m_GarbageBox;
  }
  if(m_Grid == NULL)
  {
    m_Grid = new ScalarSparseGrid(
      b, m_Resolution, 0.0, m_PartitionSize
    );

    addPoints();
  }
}

void BaseWisp::setSeed(const unsigned long seed)
{
  m_PRN.setSeed(seed);
}

const float BaseWisp::eval(const Vector &p) const
{
  // Just promise to splat before you eval!
  return m_Grid->eval(p);
}

const Vector BaseWisp::grad(const Vector &p) const
{
  return m_Grid->grad(p);
}

const Box BaseWisp::getBBox() const
{
  if(m_Box) return *m_Box;
  return calcBBox();
}
