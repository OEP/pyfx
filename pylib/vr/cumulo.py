"""
Cumulo cloud algorithm.
"""

from vr import vrend
from vr.atoms import atomize
from vr.factory import FractalSumFactory

class Cumulo(object):
  def __init__(self,
      fsfactory = FractalSumFactory(),
      billow = (vrend.Constant, 1),
      frequency = 1.0,
      gradient = None,
      rscale = 1.0,
      box = vrend.Box(1),
      resolution = vrend.Vector(0.1, 0.1, 0.1),
      iterations = 1,
      clampRadius = 1.0):
    self.fsfactory = fsfactory
    self.fspn = self.fsfactory.create()
    self.noise = vrend.FractalSumField(self.fspn)
    self.billow = billow
    self.frequency = frequency
    self.rscale = rscale
    self.clampRadius = clampRadius
    self.resolution = resolution
    self.iterations = iterations
    self.box = box
    self.gradient = gradient

  def compute_density(self, density):
    """
    Compute a new noise map and slap on the cumulo function.
    """
    if self.gradient:
      density = (vrend.ReplaceGradient, density, self.gradient)
    return_value = atomize(density)
    res = self.resolution

    for i in range(self.iterations):
      noise = self.compute_layer(i)
      cmfn = self.get_cumulo_function(return_value, noise)
      next_grid = vrend.DenseGrid(self.box, res, 0)

      next_grid.stamp(cmfn.top())
      return_value = next_grid
      if self.gradient:
        return_value = (vrend.ReplaceGradient, return_value, self.gradient)

    return_value = atomize(return_value)
    return_value.adopt(self.box)
    return return_value

  def get_cumulo_function(self, density, noise):
    """
    Returns the Cumulo addition function. Use this if you are re-using
    computed noise.

    Pyroclastic S(x) = S(x) + |N(Y(x))|
    """
    density = atomize(density)
    noise = atomize(noise)
    surface = density.find_surface()
    rhs = abs(noise.pass_through(surface))
    return density + rhs

  def compute_layer(self, n):
    noise = self.noise
    for i in range(n+1):
      noise = self.get_layer(noise)
    return noise

  def get_layer(self, previousNoise):
    """
    Returns the Cumulo layer function.

    N_n = r * N_{n-1}(x f) * Clamp(|N_{n-1}(x) / Q|, 0, 1)^billow
    """
    previousNoise = atomize(previousNoise)
    one = self.rscale
    two = previousNoise.scale(1.0 / self.frequency)
    three = abs(previousNoise).clamp(q=self.clampRadius) ** self.billow
    return one * two * three
