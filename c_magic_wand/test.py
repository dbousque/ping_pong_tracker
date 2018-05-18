

import cv2
from ping_pong import get_possible_balls

image = cv2.imread('../ping_pong.jpg')
possible_balls = get_possible_balls(image)
#print(possible_balls)
print(len(possible_balls))
for ball in possible_balls:
  # print('ball')
  for pixel in ball:
    image[pixel] = [255, 0, 0]
print('all balls done')
cv2.namedWindow('image', cv2.WINDOW_NORMAL)
cv2.imshow('image', image)
cv2.waitKey(0)
cv2.destroyAllWindows()