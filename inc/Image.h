#ifndef IMAGE_H_
#define IMAGE_H_

#include "Color.h"
#include "types.h"
#include <vector>

namespace vr
{
  class Image
  {
    private:
      size_t m_Shape[3];
      std::vector< std::vector<size_t> > m_Frequency;
      std::vector< std::vector<float>  > m_Values;

      Properties m_Properties;
    protected:
      void _normalize(float *values) const;

      size_t index(size_t i, size_t j) const;
    public:
      typedef std::vector<float> Pixel;
      typedef std::vector<size_t> PixelCount;

      Image(size_t width, size_t height, size_t depth=4);
      ~Image();

      static Image* fromFile(const char *filename);

      void initialize();

      void fill(float value, size_t freq=1);
      void fill(const Image::Pixel &pixel, size_t freq=1);
      void fill(const Color &value, size_t freq=1);
      
      void add(size_t i, size_t j, float value);
      void add(size_t i, size_t j, const Image::Pixel &p);
      void add(size_t i, size_t j, const Color &value);

      void set(size_t i, size_t j, float value, size_t freq=1);
      void set(size_t i, size_t j, const Image::Pixel &p, size_t freq=1);
      void set(size_t i, size_t j, const Color &value, size_t freq=1);

      void setChannel(size_t i, size_t j, size_t c, float value, size_t freq=1);

      const bool inBounds(size_t i, size_t j) const;
      const bool inBounds(size_t i, size_t j, size_t c) const;

      const float evalChannel(const Vector2D&, size_t c, float bg=0.0f) const;
      const float evalChannel(size_t i, size_t j, size_t c, float bg=0.0f) const;
      void read(size_t i, size_t j, Image::Pixel &out) const;
      float read(size_t i, size_t j, size_t c) const;
      const Color readColor(size_t i, size_t j) const;

      void setProperty(const std::string &name, 
        const std::string &value);
      void addProperties(const Properties &props);

      void write(const char *filename) const;

      void copyPixels(std::vector<Color> &buf) const;

      size_t width() const;
      size_t height() const;
      size_t depth() const;
      size_t pixels() const;
      size_t size() const;
      const std::vector<Image::Pixel>* data() const;
      
      const size_t __len__() const;
      const Color __getitem__(size_t) const;
  };
}

#endif
