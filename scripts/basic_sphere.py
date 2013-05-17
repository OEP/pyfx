#!/usr/bin/env python
"""
Renders a sphere centered at the origin with radius 1.

This mainly is just to demonstrate the stuff you can do with command line
parameters. By default the camera sits at (0,0,0) looking down the Z-axis, so
this script will render a grey image. You can place the camera by doing this:

$> python basic_sphere.py --eye 0 0 4 --view 0 0 -1

Or you can add a white light to the scene by doing this:

$> python basic_sphere.py --light 0 8 0 1 --dsm-size 50 --dsm-step 0.2 [...]

"""

from vr.vrend import *
from vr import cmd, color
from vr.atoms import atomize

def main():
  parser = cmd.getopt_render(default_name='sphere')
  (opt, args) = parser.parse_args()

  density = atomize(Sphere)
  s1 = density.translate(Vector(1,0,0))
  s2 = density.translate(Vector(-1,0,0))
  density = s1.blinn(s2, density).clamp()

  camera = cmd.fetch_camera(opt)
  renderer = cmd.fetch_renderer(opt)

  renderer.render_density(camera, density, density.shade(color.CLEMSON_ORANGE))

if __name__ == "__main__":
  main()
