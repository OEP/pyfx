"""
Filename utilities.
"""

import getpass

GRID_EXT = "vob"
IMAGE_EXT = "exr"

def grid_filename(name, path):
  return path_filename(name, path, GRID_EXT)

def grid_frame(name, frame, path):
  return path_frame(name, frame, path, GRID_EXT)

def image_filename(name, path):
  return path_filename(name, path, IMAGE_EXT)

def image_frame(name, frame, path):
  return path_frame(name, frame, path, IMAGE_EXT)

def path_filename(name, path, ext):
  user = getpass.getuser()
  return "%s_%s.%s" % (user, name, ext) if path == "" \
    else "%s/%s_%s.%s" % (path, user, name, ext)

def path_frame(name, frame, path, ext):
  base = path_filename(name, path, ext)
  (other, ext) = base.rsplit(".", 1)
  return "%s.%04d.%s" % (other, frame, ext)
