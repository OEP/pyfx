#include "BoxedCutter.h"

using namespace vr;

const float BoxedCutter::eval(int i, const Vector &p) const
{
  /*Vector pXformed = ((p - bBox.llc()) / res);
  Vector ndx(i % (int)bBox.sizeX(), i / (int)bBox.sizeX(), i / (int)(bBox.sizeX() * bBox.sizeY())
  return(i == (int)(pXformed[0] + pXformed[1] * bBox.sizeX() 
    + pXformed[2] * bBox.sizeX() * bBox.sizeY()));*/
  return getBBox(i).contains(p);
}

const Vector BoxedCutter::grad(int i, const Vector &p) const
{
  throw std::runtime_error("BoxedCutter::grad(): Not implemented.");
}

const Box BoxedCutter::getBBox(int i) const
{
  Vector ndx(i % (int)(nUnits[0]), (i % (int)(nUnits[0] * nUnits[1])) / (int)(nUnits[1]), i / (int)(nUnits[0] * nUnits[1]));
  //std::cout << ndx * res + bBox.llc() << std::endl;
  return Box(ndx.componentProduct(res) + bBox.llc(), ndx.componentProduct(res) + bBox.llc() + res);
  //return bBox;
}
