"""
Simple grid storage format.
"""

import vr.vrend as vrend
import vr.common as common
import math
import struct
from datetime import datetime

## Constants
BOX = 'box'
FRUSTUM = 'frustum'
DENSE = 'dense'
SPARSE = 'sparse'
SHAPES = (BOX, FRUSTUM)
STRUCTURES = (DENSE, SPARSE)

## Properties
PROP_URC = "urc"
PROP_LLC = "llc"
PROP_EYE = "eye"
PROP_VIEW = "view"
PROP_UP = "up"
PROP_RIGHT = "right"
PROP_HFOV = "hfov"
PROP_AR = "ar"
PROP_NEARPLANE = "nearplane"
PROP_FARPLANE = "farplane"

STRUCTURE_MAP = {
  (1, DENSE): vrend.ScalarDenseGrid,
  (3, DENSE): vrend.VectorDenseGrid,
  (4, DENSE): vrend.ColorDenseGrid,
  (1, SPARSE): vrend.ScalarSparseGrid,
  (3, SPARSE): vrend.VectorSparseGrid,
  (4, SPARSE): vrend.ColorSparseGrid
}

SHAPE_MAP = {
  BOX: vrend.Box,
  FRUSTUM: vrend.Camera
}

class VOB:

  @classmethod
  def fromgrid(cls, grid):
    vobject = VOB()
    vobject._gridInstance = grid
    vobject._shapeInstance = grid.griddable()
    vobject._partitionSize = 8
    vobject._depth = common.getdepth(grid)
    vobject._default = grid.defaultValue()
    vobject._resolution = grid.resolution()
    vobject._shape = grid.griddable().getShape()
    vobject._structure = "sparse" if common.issparse(grid) else "dense"
    vobject._properties = vrend.Properties()
    grid.griddable().getProperties(vobject._properties)
    return vobject

  @classmethod
  def load(cls, path, structure=DENSE, partitionSize=8):
    if structure not in STRUCTURES:
      raise ValueError("Unknown structure type: %s" % structure)
    vobject = VOB()
    vobject._partitionSize = partitionSize
    with open(path, "r") as fp:
      while True:
        line = fp.readline()
        if line is None: raise ValueError("Unexpected EOF")
        line = line.strip()
        if len(line) == 0 or line[0] == "#":
          continue
        ## no-arg commands
        if line == "blk":
          break

        (cmd, args) = line.split(" ", 1)
        if cmd == "depth":
          vobject._depth = int(args)
        elif cmd == "default":
          vobject._default = common.parsemixed(args)
        elif cmd == "resolution":
          vobject._resolution = common.parsevector(args)
        elif cmd == "shape":
          vobject._shape = args.strip()
        elif cmd == "structure":
          vobject._structure = args.strip()
        elif cmd == "prop":
          (name, value) = args.split(' ', 1)
          vobject._properties[name] = value
        else:
          raise ValueError("Unrecognized command: " + cmd)
      vobject._readblock(fp)
      return vobject

  def __init__(self):
    self._partitionSize = 8
    self._depth = None
    self._default = None
    self._resolution = None
    self._shape = None
    self._properties = {}
    self._gridInstance = None
    self._shapeInstance = None

  def defaultValue(self):
    return self._defaultValue

  def getprop(self, name):
    return self._properties[name]

  def setprop(self, name, value):
    self._properties[str(name)] = str(value)

  def getvecprop(self, name):
    return common.parsevector(self.getprop(name))

  def getfloatprop(self, name):
    return float(self.getprop(name))

  def getgrid(self):
    return self._gridinstance()

  def write(self, path):
    self.setprop('info-write-timestamp', datetime.now().ctime())
    with open(path, "w") as fp:
      fp.write("depth %d\n" % self._depth)
      fp.write("default %s\n" % common.strmixed(self._default))
      fp.write("resolution %s\n" % common.strvector(self._resolution))
      fp.write("shape %s\n" % self._shape)
      fp.write("structure %s\n" % self._structure)
      
      for (key, val) in self._properties.items():
        fp.write("prop %s %s\n" % (key, val))
      fp.write("blk\n")

      grid = self._gridInstance
      for k in range(grid.NZ()):
        for j in range(grid.NY()):
          for i in range(grid.NX()):
            value = grid.evalGrid(i,j,k)
            if value == self._default:
              continue
            fmt = ">HHH" + self._depth * "f"
            values = (i,j,k) + common.tuplemixed(value)
            packed = struct.pack(fmt, *values)
            fp.write(packed)

  def _shapeclass(self):
    return SHAPE_MAP[self._shape]

  def _gridclass(self):
    return STRUCTURE_MAP[(self._depth, self._structure)]

  def _gridinstance(self):
    if self._gridInstance != None: return self._gridInstance
    (self._gridInstance, self._shapeInstance) = self.new_gridinstance()
    return self._gridInstance

  def new_gridinstance(self, res=None):
    kls = self._gridclass()
    shp = self._shapeinstance()

    if res is None: res = self._resolution

    if self._structure == SPARSE:
      return (kls(
        shp,
        res,
        self._default,
        self._partitionSize), shp)
    elif self._structure == DENSE:
      return (kls(
        shp,
        res,
        self._default), shp)
    else:
      raise ValueError("Invalid structure: " + self._structure)

  def new_shapeinstance(self):
    kls = self._shapeclass()
    if self._shape == BOX:
      self._shapeInstance = \
        kls(self.getvecprop(PROP_LLC), self.getvecprop(PROP_URC))
      return self._shapeInstance
    elif self._shape == FRUSTUM:
      up = self.getvecprop(PROP_UP)
      right = self.getvecprop(PROP_RIGHT)
      self._shapeInstance = kls(up, right)
      shp = self._shapeInstance
      shp.setNearPlane(self.getfloatprop(PROP_NEARPLANE))
      shp.setFarPlane(self.getfloatprop(PROP_FARPLANE))
      shp.setFOV(math.degrees(self.getfloatprop(PROP_HFOV)))
      shp.setAspectRatio(self.getfloatprop(PROP_AR))
      return shp
    raise ValueError("Invalid shape: %s" % self._shape)

  def _shapeinstance(self):
    if self._shapeInstance != None: return self._shapeInstance
    self._shapeInstance = self.new_shapeinstance()
    return self._shapeInstance

  def _readblock(self, fp):
    grid = self._gridinstance()
    fmt = ">HHH" + self._depth * "f"
    bytesPerRead = struct.calcsize(fmt)
    seq = fp.read(bytesPerRead)
    
    while len(seq) == bytesPerRead:
      unpacked = struct.unpack(fmt, seq)
      (i, j, k) = unpacked[:3]
      rest = unpacked[3:]
      native = common.tonative(rest)
      self._gridInstance.set(i, j, k, native)
      seq = fp.read(bytesPerRead)

    if len(seq) > 0:
      raise IOError("Excess bytes on tail.")
