#include <assert.h>
#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include "Image.h"
#include "macros.h"

using namespace vr;
using std::string;

Image::Image(size_t width, size_t height, size_t depth)
{
  m_Values = new float[width * height * depth];
  m_Frequency = new size_t[width * height * depth];
  m_Shape[0] = width;
  m_Shape[1] = height;
  m_Shape[2] = depth;
  initialize();
}

Image::~Image()
{
  delete [] m_Values;
  delete [] m_Frequency;
}

void Image::initialize()
{
  _fill(0.0f, 0);
}

void Image::add(size_t i, size_t j, float value)
{
  _set(i, j, value, 1);
}

void Image::add(size_t i, size_t j, float *value)
{
  _set(i, j, value, 1);
}

void Image::add(size_t i, size_t j, const Color &value)
{
  _set(i, j, value, 1);
}

void Image::setChannel(size_t i, size_t j, size_t c, float value)
{
  m_Frequency[index(i,j,c)] = 1;
  m_Values[index(i,j,c)] = value;
}

void Image::set(size_t i, size_t j, float value)
{
  _set(i, j, value, -1);
}

void Image::set(size_t i, size_t j, float *value)
{
  _set(i, j, value, -1);
}

void Image::set(size_t i, size_t j, const Color &value)
{
  _set(i, j, value, -1);
}

void Image::fill(float value)
{
  _fill(value, -1);
}

void Image::fill(const Color &value)
{
  _fill(value, -1);
}

void Image::fill(float *value)
{
  _fill(value, -1);
}

void Image::fillAdd(float value)
{
  _fill(value, 1);
}

void Image::fillAdd(const Color &value)
{
  _fill(value, 1);
}

void Image::fillAdd(float *value)
{
  _fill(value, 1);
}

void Image::_fill(float value, const int mode)
{
  float tmp[depth()];
  for(size_t i = 0; i < depth(); i++)
  {
    tmp[i] = value;
  }
  _fill(tmp, mode);
}

void Image::_fill(const Color &value, const int mode)
{
  assert(depth() == 4);
  float c[] = { value.red(), value.green(), value.blue(), value.alpha() };
  _fill(c, mode);
}

void Image::_fill(const float *value, const int mode)
{
  float *p = m_Values;
  size_t *c = m_Frequency;
  for(size_t i = 0; i < size(); i++, p++, c++)
  {
    if(mode > 0)
    {
      *p += mode * value[i % depth()];
      *c += mode;
    }
    else
    {
      *p = value[i % depth()];
      *c = abs(mode);
    }
  }
}

void Image::_set(size_t i , size_t j, float value, const int mode)
{
  float tmp[depth()];
  for(size_t k = 0; k < depth(); k++)
  {
    tmp[k] = value;
  }
  _set(i, j, tmp, mode);
}

void Image::_set(size_t i, size_t j, const Color &value, const int mode)
{
  assert(depth() == 4);
  float tmp[] = {value.red(), value.green(), value.blue(), value.alpha()};
  _set(i, j, tmp, mode);
}

void Image::_set(size_t i, size_t j, const float *value, const int mode)
{
  float *p = m_Values + index(i,j);
  size_t *c = m_Frequency + index(i,j);

  for(size_t k = 0; k < depth(); k++, p++, c++)
  {
    if(mode > 0)
    {
      *p += mode * value[k];
      *c += mode;
    }
    else
    {
      *p = value[k];
      *c = abs(mode);
    }
  }
}

void Image::_normalize(float *values) const
{
  float *p = m_Values;
  size_t *c = m_Frequency;
  for(size_t i = 0; i < size(); i++, p++, c++, values++)
  {
    *values = (*c > 0) ? *p / *c : *p;
  }
}

void Image::read(size_t i, size_t j, float *out) const
{
  float *p = m_Values + index(i,j);
  size_t *c = m_Frequency + index(i,j);
  
  for(size_t i = 0; i < depth(); i++, p++, c++)
  {
    *out = SAFE_NORMALIZE(*p, *c);
  }
}

float Image::read(size_t i, size_t j, size_t channel) const
{
  float *p = m_Values + index(i,j);
  size_t *c = m_Frequency + index(i,j);

  p += channel;
  c += channel;
  return SAFE_NORMALIZE(*p, *c);
}

const Color Image::readColor(size_t i, size_t j) const
{
  float *p = m_Values + index(i,j);
  size_t *c = m_Frequency + index(i,j);

  return Color(
    SAFE_NORMALIZE(p[0], c[0]),
    SAFE_NORMALIZE(p[1], c[1]),
    SAFE_NORMALIZE(p[2], c[2]),
    SAFE_NORMALIZE(p[3], c[3]));
}

void Image::setProperty(const string &name, const string &value)
{
  m_Properties[name] = value;
}

void Image::addProperties(const Properties &props)
{
  for(Properties::const_iterator it = props.begin(); it != props.end(); it++)
  {
    m_Properties[it->first] = it->second;
  }
}

Image* Image::fromFile(const char *filename)
{
  ImageInput *in = ImageInput::create(filename);
  char buf[256];

  if(!in)
  {
    sprintf(buf, "Error opening %s for reading.", filename);
    throw std::runtime_error(buf);
  }

  ImageSpec spec;
  in->open (filename, spec);

  Image *im = new Image(spec.width, spec.height, spec.nchannels);
  float* pixels = new float[spec.width * spec.height * spec.nchannels];
  in->read_image(TypeDesc::FLOAT, pixels);

  unsigned long index = 0;
  for(size_t j = 0; j < spec.height; j++)
  {
    for(size_t i = 0; i < spec.width; i++)
    {
      for(size_t c = 0; c < spec.nchannels; c++)
      {
        im->setChannel(i, spec.height - j - 1, c, pixels[index++]);
      }
    }
  }

  delete [] pixels;
  return im;
}

void Image::write(const char *filename) const
{
  float *imgdata = new float[size()];
  _normalize(imgdata);

  ImageOutput *out = ImageOutput::create(filename);

  if(!out)
  {
    delete [] imgdata;
    return;
  }
  
  ImageSpec spec(width(), height(), depth(), TypeDesc::FLOAT);

  for(Properties::const_iterator it = m_Properties.begin(); 
    it != m_Properties.end(); it++)
  {
    spec.attribute(it->first, it->second);
  }

  out->open(filename, spec);
  out->write_image(TypeDesc::FLOAT, imgdata);
  out->close();

  delete out;
  delete [] imgdata;
}

size_t Image::size() const
{
  return width() * height() * depth();
}

size_t Image::width() const
{
  return m_Shape[0];
}

size_t Image::height() const
{
  return m_Shape[1];
}

size_t Image::pixels() const
{
  return width() * height();
}

size_t Image::depth() const
{
  return m_Shape[2];
}

size_t Image::index(const size_t i, const size_t j) const
{
  return ((height() - j - 1) * width() * depth() + i * depth());
}

size_t Image::index(const size_t i, const size_t j, const size_t c) const
{
  return index(i, j) + c;
}

const bool Image::inBounds(size_t i, size_t j) const
{
  return i >= 0 && j >= 0 && i < width() && j < height();
}

const bool Image::inBounds(size_t i, size_t j, size_t c) const
{
  return inBounds(i,j) && c >= 0 && c < depth();
}

void Image::copyPixels(std::vector<Color> &buf) const
{
  for(size_t j = 0; j < height(); j++)
  {
    for(size_t i = 0; i < width(); i++)
    {
      buf.push_back( readColor(i,j) );
    }
  }
}

const float Image::evalChannel(const Vector2D &p, size_t c, float background) const
{
  const Vector2D idx = p.componentProduct(Vector2D(width(), height()));
  const size_t i = idx.X(), j = idx.Y();
  const float
    wx = (1 - (idx.X() - i)),
    wy = (1 - (idx.Y() - j));

  return
    wx * wy * evalChannel(i, j, c, background) +
    (1-wx) * wy * evalChannel(i+1, j, c, background) +
    wx * (1-wy) * evalChannel(i, j+1, c, background) +
    (1-wx) * (1-wy) * evalChannel(i+1, j+1, c, background);
}

const size_t Image::__len__() const
{
  return pixels();
}

const Color Image::__getitem__(size_t i) const
{
  if(i >= pixels())
  {
    throw std::out_of_range("Index out of bounds.");
  }

  const float *p = m_Values + (i * depth());
  const size_t *c = m_Frequency + (i * depth());
  
  if(depth() == 4)
  {
    return Color(
      SAFE_NORMALIZE(p[0], c[0]),
      SAFE_NORMALIZE(p[1], c[1]),
      SAFE_NORMALIZE(p[2], c[2]),
      SAFE_NORMALIZE(p[3], c[3]));
  }

  throw std::runtime_error("Unsupported image depth.");
}

const float Image::evalChannel(size_t i, size_t j, size_t c, float background) const
{
  if(!inBounds(i, j, c)) return background;
  return read(i, j, c);
}
