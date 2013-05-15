"""
Common functions for many parts of vrend.

Includes functions for printing, type-checking, and type coercion.
"""
from vr import vrend, graph
from inspect import isclass

def tuplevector(v):
  """Converts vector to tuple."""
  return (v.X(), v.Y(), v.Z())

def tuplecolor(c):
  return (c.red(), c.green(), c.blue(), c.alpha())

def tonative(thing):
  """
  Convert a tuple to native containing object.
  """
  if len(thing) == 1: return thing[0]
  if len(thing) == 3: return vrend.Vector(*thing)
  if len(thing) == 4: return vrend.Color(*thing)
  if len(thing) == 9: return vrend.Matrix(*thing)
  raise ValueError("Unabled to cast as native: " + thing)

def tuplemixed(thing):
  if isinstance(thing, vrend.Vector): return tuplevector(thing)
  if isinstance(thing, vrend.Color): return tuplecolor(thing)
  if isinstance(thing, float): return (thing,)
  return tuple(thing)

def strmixed(thing):
  if isinstance(thing, vrend.Vector): return strvector(thing)
  if isinstance(thing, vrend.Color): return strcolor(thing)
  return str(thing)

def strvector(v):
  """Converts a vector to human-readable string."""
  return "%f %f %f" % (v.X(), v.Y(), v.Z())

def strcolor(c):
  """Converts a color to human-readable string."""
  return "%f %f %f %f" % (c.red(), c.green(), c.blue(), c.alpha())

def parsefloats(v):
  """Parse a string as delimited sequence of floats"""
  return tuple(map(float, v.split(None)))

def parsemixed(v):
  args = parsefloats(v)
  if len(args) == 1: return float(v)
  elif len(args) == 3: return parsevector(v)
  elif len(args) == 4: return parsecolor(v)
  raise ValueError("Unhandled depth: " + len(args))

def parsevector(v):
  """Parse a string as space-delimited vector."""
  args = parsefloats(v)
  return vrend.Vector(*args)

def parsecolor(c):
  args = parsefloats(c)
  return vrend.Color(*args)

def isvolumetype(thing):
  return isclass(thing) and (
    isscalartype(thing) or
    isvectortype(thing) or
    iscolortype(thing) or
    ismatrixtype(thing))

def isscalartype(thing):
  return isclass(thing) and issubclass(thing,vrend.ScalarField)

def isvectortype(thing):
  return isclass(thing) and issubclass(thing,vrend.VectorField)

def iscolortype(thing):
  return isclass(thing) and issubclass(thing,vrend.ColorField)

def ismatrixtype(thing):
  return isclass(thing) and issubclass(thing,vrend.MatrixField)

def _issometree(thing, tester):
  return tester(thing) or (
    isinstance(thing, tuple) and
    len(thing) > 0 and
    tester(thing[0]))

def isvolumetree(thing):
  return _issometree(thing,
    lambda x: hasattr(x, '__call__'))

def isscalartree(thing): return _issometree(thing, isscalartype)
def isvectortree(thing): return _issometree(thing, isvectortype)
def iscolortree(thing): return _issometree(thing, iscolortype)
def ismatrixtree(thing): return _issometree(thing, ismatrixtype)

def isgraph(thing): return isinstance(thing, graph.VolumeGraph)

def isfield(thing):
  return isscalarfield(thing) or \
    isvectorfield(thing) or \
    iscolorfield(thing) or \
    ismatrixfield(thing)

def isscalarfield(thing): return isinstance(thing, vrend.ScalarField)
def isvectorfield(thing): return isinstance(thing, vrend.VectorField)
def iscolorfield(thing): return isinstance(thing, vrend.ColorField)
def ismatrixfield(thing): return isinstance(thing, vrend.MatrixField)

def isscalar(thing): return isscalartree(thing) or isscalarfield(thing)
def isvector(thing): return isvectortree(thing) or isvectorfield(thing)
def iscolor(thing): return iscolortree(thing) or iscolorfield(thing)
def ismatrix(thing): return ismatrixtree(thing) or ismatrixfield(thing)
def isvolume(thing):
  return isscalar(thing) or \
    isvector(thing) or \
    ismatrix(thing) or \
    iscolor(thing)

def isdense(thing):
  return isinstance(thing, vrend.ScalarDenseGrid) or \
    isinstance(thing, vrend.VectorDenseGrid) or \
    isinstance(thing, vrend.ColorDenseGrid) or \
    isinstance(thing, vrend.MatrixDenseGrid)

def issparse(thing):
  return isinstance(thing, vrend.ScalarSparseGrid) or \
    isinstance(thing, vrend.VectorSparseGrid) or \
    isinstance(thing, vrend.ColorSparseGrid) or \
    isinstance(thing, vrend.MatrixSparseGrid)

def isvob(thing):
  import vob
  return isinstance(thing, vob.VOB)

def getdepth(thing):
  if isscalar(thing): return 1
  if isvector(thing): return 3
  if iscolor(thing): return 4
  if ismatrix(thing): return 9
  raise ValueError("Not a volume type.")

def getsparsegridtype(thing):
  if isscalar(thing): return vrend.ScalarSparseGrid
  elif isvector(thing): return vrend.VectorSparseGrid
  elif ismatrix(thing): return vrend.MatrixSparseGrid
  elif iscolor(thing): return vrend.ColorSparseGrid
  else: raise ValueError("Not a volume type.")

def as_vob(thing):
  """
  Coerce thing into a VOB object.
  """
  import vob
  if isvob(thing): return thing
  try:
    return vob.VOB.fromgrid(thing.top())
  except TypeError:
    return vob.VOB.fromgrid(thing)

def asvolumegraph(thing):
  if isgraph(thing): return thing
  elif isvolumetree(thing): return graph.VolumeGraph.fromtree(thing)
  elif isfield(thing):
    vg = graph.VolumeGraph()
    vg.add(thing)
    return vg
  elif thing == None:
    return graph.VolumeGraph()
  raise ValueError("Can't convert to volume graph: " + str(thing))

def asdensegrid(thing, resolution, defaultValue, griddable=None):
  vg = asvolumegraph(thing)
  top = vg.top()
  if griddable == None: griddable = top.getBBox()
  grid = vrend.DenseGrid(griddable, resolution, defaultValue)
  grid.stamp(top)
  gridGraph = asvolumegraph(grid)
  gridGraph._track(griddable)
  return gridGraph

def assparsegrid(thing, resolution, defaultValue, partitionSize,
    griddable=None):
  vg = asvolumegraph(thing)
  top = vg.top()
  gridType = getsparsegridtype(top)
  if griddable == None: griddable = top.getBBox()
  grid = gridType(griddable, resolution, defaultValue, partitionSize)
  grid.stamp(top)
  gridGraph = asvolumegraph(grid)
  gridGraph._track(griddable)
  return gridGraph
