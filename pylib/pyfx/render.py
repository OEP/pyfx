"""
Render functions.
"""
import math
from . import path, timer, atomize, native

QUALITY_BEAUTY = (1080, 1, 0.001)
QUALITY_MID = (540, 1, 0.001)
QUALITY_QUICK = (480, 1, 0.1)

_qualityMap = {
  'quick': QUALITY_QUICK,
  'mid': QUALITY_MID,
  'beauty': QUALITY_BEAUTY
}
  
def get_quality(quality, overrides=None):
  parms = _qualityMap[quality]
  if overrides == None: return parms
  return tuple((x if y == None else y for (x, y) in zip(parms, overrides)))


class Render(object):
  """
  Class for handling renders as well as writing grids.
  """
  def __init__(self,
      quality=QUALITY_QUICK,
      patch_size = 100,
      dsm_size = 256,
      dsm_step = 0.1,
      scatter=1.0,
      meta={},
      name="out",
      frame=1,
      light_fov = None,
      output="./"):
    self.quality = quality
    self.patch_size = patch_size
    self.dsm_size = dsm_size
    self.dsm_step = dsm_step
    self.scatter = scatter
    self.meta = meta
    self.name = name
    self.frame = frame
    self.output = output
    self._lights = []
    self.light_fov = light_fov

  @property
  def meta(self):
    return self._meta

  @meta.setter
  def meta(self, meta):
    self._meta = {}
    for key in meta:
      self._meta[str(key)] = str(meta[key])

  @property
  def quality(self):
    return self._quality

  @quality.setter
  def quality(self, value):
    if value in _qualityMap:
      self._quality = _qualityMap[value]
    elif len(value) == 3:
      self._quality = tuple(value)
    else:
      raise ValueError("Improper quality: %s", value)

  def add_light(self, light, step=None, frustum=None):
    self._lights.append((light, step or self.dsm_step, frustum))

  @property
  def framepath(self):
    return path.image_frame(self.name, self.frame, self.output)
  
  @property
  def gridpath(self):
    return path.grid_frame(self.name, self.frame, self.output)

  def render_scene(self, scene):
    print("Render quality: ", self.quality)
    print("Scattering constant: ", scene.kappa())
    cam = scene.camera()
    im = native.Image(int(self.quality[0]*cam.aspectRatio()), self.quality[0])
    im.addProperties(native.Properties(self.meta))
    box = scene.densityBox()

    print(box.llc(), box.urc())

    for (i, (light, step, frustum)) in enumerate(self._lights):
      frustum = frustum or native.Camera.boundingFrustum(light.position(), box)
      if self.light_fov: frustum.setFOV(self.light_fov)

      with timer.print_on_finish("Light %s" % (i+1)):
        scene.addLight(light, step, frustum)

    count = 0
    patch_cols = int(math.ceil(im.width() / self.patch_size))
    patch_rows = int(math.ceil(im.height() / self.patch_size))
    patch_count = patch_rows * patch_cols
    with timer.print_on_finish("Frame %d" % self.frame):
      for y0 in range(0, im.height(), self.patch_size):
        y1 = y0 + self.patch_size
        for x0 in range(0, im.width(), self.patch_size):
          x1 = x0 + self.patch_size
          count += 1
          with timer.print_on_finish("Patch %d/%d" % (count, patch_count)):
            scene.render(im, self.quality[1], self.quality[2], x0, y0, x1, y1)
    
    im.write(self.framepath)
    print("Wrote image to '%s'" % self.framepath)

  def render_density(self, cam, density, color=None):
    """
    Quickly render a density field, optionally with color.
    """
    density = atomize(density)
    color = atomize(color)
    scene = native.Scene(cam, density.top, color.top,
      self.scatter,
      self.dsm_size)
    self.render_scene(scene)
