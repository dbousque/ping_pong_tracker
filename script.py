

import cv2
import numpy as np
import math
import random

MAGIC_WAND_TOLERANCE = 50.0

def color_distance(c1, c2):
  r1, g1, b1 = int(c1[0]), int(c1[1]), int(c1[2])
  r2, g2, b2 = int(c2[0]), int(c2[1]), int(c2[2])
  return math.sqrt((r1 - r2) ** 2 + (g1 - g2) ** 2 + (b1 - b2) ** 2)

def explore_magic_wand_at(image, y, x, mask, selection_id):
  pixels_to_explore = []
  pixels_to_explore = [[y, x, image[y, x]]]
  while len(pixels_to_explore) > 0:
    y, x, color_to_compare = pixels_to_explore.pop()
    if y < 0 or x < 0 or y >= image.shape[0] or x >= image.shape[1]:
      continue
    distance = color_distance(image[y, x], color_to_compare)
    target_distance = MAGIC_WAND_TOLERANCE if mask[y, x, 0] == 0 else mask[y, x, 1]
    if distance < target_distance:
      mask[y, x, 0] = selection_id
      mask[y, x, 1] = distance
      color = image[y, x]
      pixels_to_explore.append([y + 1, x, color])
      pixels_to_explore.append([y - 1, x, color])
      pixels_to_explore.append([y, x + 1, color])
      pixels_to_explore.append([y, x - 1, color])

def magic_wand(image):
  mask = np.zeros((image.shape[0], image.shape[1], 2))
  selection_id = 1
  for y in range(image.shape[0]):
    for x in range(image.shape[1]):
      if mask[y, x, 0] != 0:
        continue
      explore_magic_wand_at(image, y, x, mask, selection_id)
      selection_id += 1

  selections = [[]] * (selection_id - 1)
  for y in range(image.shape[0]):
    for x in range(image.shape[1]):
      selections[int(mask[y, x, 0]) - 1].append((y, x))
  return selections

def is_possible_ball(selection):
  return random.randint(0, 5) == 0

def handle_image(image):
  selections = magic_wand(image)
  print('selections done')
  possible_balls = list(filter(is_possible_ball, selections))
  print('possible_balls detected')
  for ball in possible_balls:
    print('ball')
    for pixel in ball:
      image[pixel] = [255, 0, 0]
  print('all balls done')

def handle_webcam():
  cv2.namedWindow('preview')
  vc = cv2.VideoCapture(0)

  if vc.isOpened(): # try to get the first frame
    rval, frame = vc.read()
  else:
    rval = False

  while rval:
    # cv2.imshow('preview', frame)
    rval, frame = vc.read()
    handle_image(frame)
    cv2.imshow('preview', image)
    key = cv2.waitKey(20)
    if key == 27: # exit on ESC
      break
  cv2.destroyAllWindows()

image = cv2.imread('ping_pong.jpg')
handle_image(image)
print('showing image')
cv2.namedWindow('image')
cv2.imshow('image', image)
cv2.waitKey(0)
cv2.destroyAllWindows()
# handle_webcam()