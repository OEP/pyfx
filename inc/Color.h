#ifndef COLOR_H_
#define COLOR_H_

#include <ostream>

namespace vr
{
  class Color
  {
    private:
      float m_RGBA[4];
    protected:
    public:
      Color();
      Color(float);
      Color(float, float);
      Color(float, float, float);
      Color(float, float, float, float);
      Color(const Color&);

      static const int RED, GREEN, BLUE, ALPHA;

      float red() const;
      float green() const;
      float blue() const;
      float alpha() const;

      const Color componentMin(const Color&) const;
      const Color componentMax(const Color&) const;

      const Color operator+(const Color&) const;

      const Color operator-(const Color&) const;
      const Color operator*(const Color&) const;
      const Color operator*(const double) const;
      const Color operator|(const Color&) const;
      const Color& operator=(const Color&);
      const bool operator==(const Color&) const;
      const double operator[](const int) const;

      Color& operator+=(const Color&);

      const size_t __len__() const;
      const float __getitem__(size_t) const;

      friend const Color operator*(const double, const Color &);
  };

  const Color operator*(const double, const Color &);
  std::ostream& operator<<(std::ostream&, const Color&);
}

#endif
