#include <ostream>
#include <stdexcept>

#include "Color.h"

using namespace std;
using namespace vr;

const int
  Color::RED = 0,
  Color::GREEN = 1,
  Color::BLUE = 2,
  Color::ALPHA = 3;

Color::Color()
{
  m_RGBA[0] =
    m_RGBA[1] = 
    m_RGBA[2] = 
    m_RGBA[3] = 0.0f;
}

Color::Color(float grey)
{
  m_RGBA[0] = m_RGBA[1] = m_RGBA[2] = grey;
  m_RGBA[3] = 1.0f;
}

Color::Color(float grey, float alpha)
{
  m_RGBA[0] = m_RGBA[1] = m_RGBA[2] = grey;
  m_RGBA[3] = alpha;
}

Color::Color(float r, float g, float b)
{
  m_RGBA[0] = r;
  m_RGBA[1] = g;
  m_RGBA[2] = b;
  m_RGBA[3] = 1.0f;
}

Color::Color(float r, float g, float b, float a)
{
  m_RGBA[0] = r;
  m_RGBA[1] = g;
  m_RGBA[2] = b;
  m_RGBA[3] = a;
}

Color::Color(const Color &other)
{
  m_RGBA[0] = other.m_RGBA[0];
  m_RGBA[1] = other.m_RGBA[1];
  m_RGBA[2] = other.m_RGBA[2];
  m_RGBA[3] = other.m_RGBA[3];
}

float Color::red() const
{
  return m_RGBA[0];
}

float Color::green() const
{
  return m_RGBA[1];
}

float Color::blue() const
{
  return m_RGBA[2];
}

float Color::alpha() const
{
  return m_RGBA[3];
}

const Color Color::componentMin(const Color &c) const
{
  return Color(
    std::min(red(), c.red()),
    std::min(green(), c.green()),
    std::min(blue(), c.blue()),
    std::min(alpha(), c.alpha()));
}

const Color Color::componentMax(const Color &c) const
{
  return Color(
    std::max(red(), c.red()),
    std::max(green(), c.green()),
    std::max(blue(), c.blue()),
    std::max(alpha(), c.alpha()));
}

const Color Color::operator+(const Color &c) const
{
  return Color(red() + c.red(),
    green() + c.green(),
    blue() + c.blue(),
    alpha() + c.alpha());
}

const Color Color::operator-(const Color &c) const
{
  return Color(red() - c.red(),
    green() - c.green(),
    blue() - c.blue(),
    alpha() - c.alpha());
}

const Color Color::operator-() const
{
  return Color(-red(), -green(), -blue(), -alpha());
}

const Color Color::operator*(const Color &c) const
{
  return Color(red() * c.red(),
    green() * c.green(),
    blue() * c.blue(),
    alpha() * c.alpha());
}

const Color Color::operator*(double amt) const
{
  return Color(red() * amt,
    green() * amt,
    blue() * amt,
    alpha() * amt);
}

const Color Color::operator|(const Color &c) const
{
  const double a = c.alpha();
  const Color newColor = (1-a) * (*this) + a * c;
  return Color(newColor.red(), newColor.green(), newColor.blue(), 1.0);
}

const Color& Color::operator=(const Color &c)
{
  m_RGBA[0] = c.m_RGBA[0];
  m_RGBA[1] = c.m_RGBA[1];
  m_RGBA[2] = c.m_RGBA[2];
  m_RGBA[3] = c.m_RGBA[3];
  return c;
}

const bool Color::operator==(const Color &c) const
{
  return 
    m_RGBA[0] == c.m_RGBA[0] &&
    m_RGBA[1] == c.m_RGBA[1] &&
    m_RGBA[2] == c.m_RGBA[2] &&
    m_RGBA[3] == c.m_RGBA[3];
}

Color& Color::operator+=(const Color &c)
{
  m_RGBA[0] += c.m_RGBA[0];
  m_RGBA[1] += c.m_RGBA[1];
  m_RGBA[2] += c.m_RGBA[2];
  m_RGBA[3] += c.m_RGBA[3];
  return *this;
}

const Color vr::operator*(double amt, const Color &c)
{
  return c * amt;
}

const size_t Color::__len__() const
{
  return 4;
}

const float Color::__getitem__(size_t i) const
{
  if(i < 0 || i >= 4)
  {
    throw std::out_of_range("Index out of bounds.");
  }
  return m_RGBA[i];
}

const double Color::operator[](const int i) const
{
  return m_RGBA[i];
}

ostream& vr::operator<<(ostream &out, const Color &c)
{
  out << "Color["
    << c.red() << ", "
    << c.green() << ", "
    << c.blue() << ", "
    << c.alpha() << "]";
  return out;
}
