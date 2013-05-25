"""
Resource loader.
"""

import os

from vr import vob, vrend

SEARCH_PATH = (
  "./",
  "./vrdata",
  "./.vrdata",
  "%s/.vrdata" % os.environ.get('HOME'),
  os.environ.get('VREND_DATA', None)
)

class ResourceLoader(object):

  def __init__(self):
    self.cache = { }
    self.paths = list((x for x in filter(lambda x: x, SEARCH_PATH)))

  def __search_paths(self, relative_path):
    for path in self.paths:
      full_path = os.path.join(path, relative_path)
      if os.path.isfile(full_path): return full_path
    return False

  def __find_item(self, type, name, ext):
    relative_path = os.path.join(type, name + "." + ext)
    return self.__search_paths(relative_path)

  def __find_item_or_fail(self, type, name, ext):
    path = self.__find_item(type, name, ext)
    if not path:
      raise IOError('Could not find %s named %s in: %s' % (
        type, name, ", ".join(self.paths)))
    return path

  def load_vob(self, vob_name):
    path = self.__find_item_or_fail('vob', vob_name, 'vob')
    return self.cache.get(path) or self.cache.setdefault(path, vob.VOB.load(path))

  def load_obj(self, obj_name):
    path = self.__find_item_or_fail('obj', obj_name, 'obj')
    return self.cache.get(path) or \
      self.cache.setdefault(path, vrend.Model.load(path))

  def __load_image(self, ext, name):
    path = self.__find_item_or_fail(ext, name, ext)
    return self.cache.get(path) or \
      self.cache.setdefault(path, vrend.Image.fromFile(path))

  def __getattr__(self, name):
    """
    Maybe it's an image!
    """
    if not name.startswith("load_"):
      raise AttributeError("No such attribute: %s" % name)
    
    pre, load, extension = name.partition("load_")
    return lambda x: self.__load_image(extension, x)
