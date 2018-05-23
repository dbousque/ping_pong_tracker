

import cv2
from ping_pong import get_possible_balls
import random
import time

def draw_square(image, top, bottom, left, right):
  width = 6
  left -= width * 2
  left = 0 if left < 0 else left
  right += width * 2
  right = image.shape[1] - 1 if right >= image.shape[1] else right
  top -= width * 2
  top = 0 if top < 0 else top
  bottom += width * 2
  bottom = image.shape[0] - 1 if bottom >= image.shape[1] else bottom
  for y in range(bottom - top):
    if y < 0 or y >= image.shape[0]:
      continue
    if y < width or y >= bottom - top - width:
      for x in range(right - left):
        image[top + y, left + x] = [0, 0, 255]
    for i in range(width):
      image[top + y, left + i] = [0, 0, 255]
      image[top + y, right - i] = [0, 0, 255]

image = cv2.imread('../ping_pong.jpg')
# print(image)
# print(image.shape)
start_time = time.time()
possible_balls = get_possible_balls(image)
print('time taken', time.time() - start_time)
# print(possible_balls)
print(len(possible_balls))
updated_pixels = 0
for ball in possible_balls:
  # print('ball')
  ball_color = [random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]
  top = ball[0][0]
  bottom = ball[0][0]
  left = ball[0][1]
  right = ball[0][1]
  for pixel in ball:
    image[pixel] = (image[pixel] + ball_color) / 2
    updated_pixels += 1
    if pixel[0] < top:
      top = pixel[0]
    if pixel[0] > bottom:
      bottom = pixel[0]
    if pixel[1] < left:
      left = pixel[1]
    if pixel[1] > right:
      right = pixel[1]
  draw_square(image, top, bottom, left, right)
print('all balls done')
print('updated_pixels', updated_pixels)
cv2.namedWindow('image', cv2.WINDOW_NORMAL)
cv2.imshow('image', image)
cv2.waitKey(0)
cv2.destroyAllWindows()
print('end')