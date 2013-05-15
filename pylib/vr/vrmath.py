"""
Additional math utilities not provided by Python or other packages.
"""

import math

def sech(x):
  return (2 * math.cosh(x)) / (math.cosh(2*x) + 1)
