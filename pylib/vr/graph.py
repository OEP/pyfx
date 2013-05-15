class VolumeGraph:

  def __init__(self):
    self._top = None
    self._instances = []
    self._named = {}

  @classmethod
  def fromtree(cls, tree):
    """Create a node graph from a node tree."""
    graph = cls()
    cls._fromtree(graph, tree)
    return graph

  @classmethod
  def _fromtree(cls, graph, tree):
    """Recursive helper method for fromtree."""
    from vr import common
    ## If string, assume it is in our graph dict somewhere.
    if isinstance(tree, basestring):
      return graph[tree]

    ## If it's a raw field, track it.
    if common.isfield(tree):
      graph._track(tree)
      return tree

    ## If another volume graph, merge.
    if isinstance(tree, VolumeGraph):
      graph.merge(tree)
      return tree.top()

    ## If callable, 
    if hasattr(tree, '__call__'):
      tmp = tree()
      graph.add(tmp)
      return tmp

    if not isinstance(tree, (tuple, list)):
      return tree

    assert len(tree) > 0

    name = None
    nodeType = None
    args = None
    first = tree[0]
    evalArgs = []

    if isinstance(first, basestring):
      if first in graph:
        return graph[first]
      name = first
      nodeType = tree[1]
      args = tree[2:]
    else:
      nodeType = tree[0]
      args = tree[1:]

    for arg in args:
      tmp = cls._fromtree(graph, arg)
      evalArgs.append(tmp)

    node = nodeType(*evalArgs)

    graph.add(node, name)
    return node

  def add(self, node, name=None):
    self._top = node
    self._track(node)

    if name in self._named:
      raise ValueError("Duplicate named node `%s`" % name)

    if name != None:
      self._named[name] = node

    return node

  def merge(self, other):
    for i in other._instances:
      self._track(i)
    self._named = dict(self._named.items() + other._named.items())

    self._top = other._top

  def evalAt(self, name, x):
    return self._eval(self._named[name], x)

  def eval(self, x):
    return self._eval(self._top, x)

  def _eval(self, node, point):
    assert not isinstance(point, basestring)
    from vr import vrend

    if isinstance(point, vrend.Vector):
      return node.eval(point)
    else:
      return node.eval(vrend.Vector(point[0], point[1], point[2]))

  def get(self, name):
    return self._named.get(name)

  def top(self):
    return self._top

  def __getitem__(self, name):
    return self._named[name]

  def __contains__(self, name):
    return name in self._named

  def _track(self, node):
    if node not in self._instances:
      self._instances.append(node)
