from contextlib import contextmanager
import time

@contextmanager
def print_on_finish(name=None):
  if name:
    print("Begin Timer (%s)" % name)
  else:
    print("Begin Timer")

  t0 = time.time()
  yield
  t1 = time.time()
  pretty = pretty_time(t1 - t0)

  if name:
    print("End Timer (%s) %s" % (name, pretty))
  else:
    print("End Timer %s" % pretty)


def pretty_time(time):
  """Returns human-readable time."""
  hours, rest = divmod(int(time),3600)
  minutes, seconds = divmod(rest, 60)
  return "%02d:%02d:%02d" % (hours, minutes, seconds) 
