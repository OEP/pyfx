#include <assert.h>
#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include "Image.h"
#include "macros.h"

using namespace vr;
using std::string;

Image::Image(size_t width, size_t height, size_t depth)
{
  m_Shape[0] = width;
  m_Shape[1] = height;
  m_Shape[2] = depth;
  initialize();
}

Image::~Image()
{
}

void Image::initialize()
{
  fill(0.0f, 0);
}

void Image::add(size_t i, size_t j, float value, size_t wt)
{
  Image::Pixel p(depth(), value);
  add(i, j, p, wt);
}

void Image::add(size_t i, size_t j, const Image::Pixel &p, size_t wt)
{
  const size_t idx = index(i,j);

  for(size_t c = 0; c < depth(); c++)
  {
    m_Values[idx][c]    += p[c];
    m_Frequency[idx][c] += wt;
  }
  
}

void Image::add(size_t i, size_t j, const Color &value, size_t wt)
{
  Image::Pixel p(depth(), 0.0f);
  p[0] = value.red();
  p[1] = value.green();
  p[2] = value.blue();
  p[3] = value.alpha();

  add(i, j, p, wt);
}

void Image::setChannel(size_t i, size_t j, size_t c, float value, size_t wt)
{
  Image::PixelCount wts(depth(), wt);
  m_Frequency[index(i,j)]    = wts;
  m_Values[index(i,j)][c]    = value;
}

void Image::set(size_t i, size_t j, float value, size_t wt)
{
  Image::Pixel p(depth(), value);
  set(i, j, p, wt);
}

void Image::set(size_t i, size_t j, const Image::Pixel &p, size_t wt)
{
  m_Values[index(i,j)] = p;
  m_Frequency[index(i,j)].clear();
  m_Frequency[index(i,j)].resize(depth(), wt);
}

void Image::set(size_t i, size_t j, const Color &value, size_t wt)
{
  Image::Pixel p(depth(), 0.0f);
  p[0] = value.red();
  p[1] = value.green();
  p[2] = value.blue();
  p[3] = value.alpha();

  set(i, j, p, wt);
}

void Image::fill(float value, size_t freq)
{
  Image::Pixel p(depth(), value);
  fill(p, freq);
}

void Image::fill(const Color &value, size_t freq)
{
  Image::Pixel p(depth(), 0.0f);
  p[0] = value.red();
  p[1] = value.green();
  p[2] = value.blue();
  p[3] = value.alpha();

  fill(p, freq);
}

void Image::fill(const Image::Pixel &p, size_t freq)
{
  m_Values.clear();
  m_Frequency.clear();

  Image::PixelCount freqs(depth(), freq);

  m_Values.resize(pixels(), p);
  m_Frequency.resize(pixels(), freqs);
}

void Image::_normalize(float *values) const
{
  for(size_t j = 0; j < height(); j++)
  {
    for(size_t i = 0; i < width(); i++)
    {
      for(size_t c = 0; c < depth(); c++)
      {
        *values = read(i, j, c);
        values++;
      }
    }
  }
}

void Image::read(size_t i, size_t j, Image::Pixel &p) const
{
  for(size_t c = 0; c < depth(); c++)
  {
    p[i] = read(i, j, c);
  }
}

float Image::read(size_t i, size_t j, size_t channel) const
{
  return SAFE_NORMALIZE(
    m_Values[index(i,j)][channel],
    m_Frequency[index(i,j)][channel]
  );
}

const Color Image::readColor(size_t i, size_t j) const
{
  return Color(
    read(i, j, 0),
    read(i, j, 1),
    read(i, j, 2),
    read(i, j, 3)
  );
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
  for(int j = 0; j < spec.height; j++)
  {
    for(int i = 0; i < spec.width; i++)
    {
      for(int c = 0; c < spec.nchannels; c++)
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
  return ((height() - j - 1) * width() + i);
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
  const size_t i = idx.x(), j = idx.y();
  const float
    wx = (1 - (idx.x() - i)),
    wy = (1 - (idx.y() - j));

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

  const size_t
    jj = i / width(),
    ii = i % width();

  if(depth() == 4)
  {
    return readColor(ii, jj);
  }

  throw std::runtime_error("Unsupported image depth.");
}

const float Image::evalChannel(size_t i, size_t j, size_t c, float background) const
{
  if(!inBounds(i, j, c)) return background;
  return read(i, j, c);
}
