

import cv2
from ping_pong import get_possible_balls
import random
import time

def draw_square(image, top, bottom, left, right):
  width = 6
  left -= width * 2
  right += width * 2
  top -= width * 2
  bottom += width * 2
  for y in range(bottom - top):
    if top + y < 0 or top + y >= image.shape[0]:
      continue
    if y < width or y >= bottom - top - width:
      for x in range(right - left):
        if left + x >= 0 and left + x < image.shape[1]:
          image[top + y, left + x] = [0, 0, 255]
    for i in range(width):
      if left + i >= 0 and left + i < image.shape[1]:
        image[top + y, left + i] = [0, 0, 255]
      if right - i >= 0 and right - i < image.shape[1]:
        image[top + y, right - i] = [0, 0, 255]

def handle_image(image):
  start_time = time.time()
  possible_balls = get_possible_balls(image)
  time_taken = time.time() - start_time
  for ball in possible_balls:
    # print('ball')
    ball_color = [random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]
    top = ball[0][0]
    bottom = ball[0][0]
    left = ball[0][1]
    right = ball[0][1]
    for pixel in ball:
      image[pixel] = (image[pixel] + ball_color) / 2
      if pixel[0] < top:
        top = pixel[0]
      if pixel[0] > bottom:
        bottom = pixel[0]
      if pixel[1] < left:
        left = pixel[1]
      if pixel[1] > right:
        right = pixel[1]
    draw_square(image, top, bottom, left, right)
  return time_taken

def image_test():
  cv2.namedWindow('image', cv2.WINDOW_NORMAL)
  image = cv2.imread('../ping_pong3.jpg')
  start_time = time.time()
  _ = get_possible_balls(image)
  time_taken1 = time.time() - start_time
  time_taken2 = handle_image(image)
  cv2.imshow('image', image)
  cv2.waitKey(0)
  cv2.destroyAllWindows()
  print(time_taken1)
  print(time_taken2)

def video_test():
  cv2.namedWindow('image', cv2.WINDOW_NORMAL)
  video = cv2.VideoCapture('../sample_video.mp4')
  while video.isOpened():
    ret, frame = video.read()
    time_taken = handle_image(frame)
    print(time_taken)
    cv2.imshow('image', frame)
    key = cv2.waitKey(0)
    if key == 27: # exit on ESC
      break
  cv2.destroyAllWindows()

video_test()