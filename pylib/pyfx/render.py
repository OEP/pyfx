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
    cam = scene.camera()
    im = native.Image(int(self.quality[0]*cam.aspectRatio()), self.quality[0])
    im.addProperties(native.Properties(self.meta))
    box = scene.densityBox()

    ## add lights to scene if provided
    for (i, (light, step, frustum)) in enumerate(self._lights):
      frustum = frustum or native.Camera.boundingFrustum(light.position(), box)
      if self.light_fov: frustum.setFOV(self.light_fov)
      with timer.print_on_finish("Light %s" % (i+1)):
        scene.addLight(light, step, frustum)

    ## render the image in patches
    patch_w = self.patch_size
    patch_h = self.patch_size
    patch_cols = int(math.ceil(im.width() / patch_w))
    patch_rows = int(math.ceil(im.height() / patch_h))
    patch_count = patch_rows * patch_cols
    count = 0
    with timer.print_on_finish("Frame %d" % self.frame):
      for y0 in range(0, im.height(), patch_h):
        h = min(im.height() - y0, patch_h)
        for x0 in range(0, im.width(), patch_w):
          w = min(im.width() - x0, patch_w)
          count += 1
          with timer.print_on_finish("Patch %d/%d" % (count, patch_count)):
            patch_im = native.Image(w, h)
            scene.render(patch_im, self.quality[1], self.quality[2],
                         im.width(), im.height(), x0, y0)
            patch_im.write("patch.%04d.exr" % count)
            im.replace(patch_im, x0, y0)
    
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
