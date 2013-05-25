"""
Various step functions.
"""
import math

def tanh(q, q0=0.0, L=1.0):
  "Hyperbolic tangent step function."
  return (1.0 + math.tanh((q - q0) / L)) / 2.0

def dtanh(q, q0=0.0, L=1.0):
  "Derivative of hyperbolic tangent step function."
  from vrmath import sech
  sech_result = sech((q - q0) / L)
  return sech_result * sech_result / (2 * L)
