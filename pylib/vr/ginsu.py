"""
Ginsu tool functions.

The Ginsu scalar field is in the module vrend.
"""

import numpy as np

from vr import vrend

def image_slice(image_path, canny_threshold=0):
  """
  Create a Vector2DSequence from the given ``image_path``.
  """
  import cv2
  import cv2.cv as cv
  seq = vrend.Vector2DSequence()

  ## Read image, convert to 8-bit unsigned, grayscale and blur.
  im = cv2.imread(image_path)
  width, height, depth = im.shape
  im = (255 * im).astype(np.uint8)
  im = cv2.cvtColor(im, cv.CV_RGB2GRAY)
  im = cv2.blur(im, (2,2))

  ## Perform canny edge detection
  im = cv2.Canny(im, canny_threshold, 2 * canny_threshold)

  ## Get the contours.
  contours, hierarchy = cv2.findContours(im, cv.CV_RETR_LIST,
    cv.CV_CHAIN_APPROX_SIMPLE)

  ## Construct the Vector2DSequence
  for contour in contours:
    for points in contour:
      for point in points:
        point = map(float, point)
        point = (x1/x2 for (x1, x2) in zip(point, (width, height)))
        seq.push_back(vrend.Vector2D(*point))
  return seq
