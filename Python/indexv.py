# -*- coding: utf-8 -*-
import cv2
from PIL import Image
from PlantLineDetection import LineDetection
import numpy as np
import time

cap = cv2.VideoCapture('../videos/video-lines.mp4')

ld = LineDetection()

while(True):
    # Capture frame-by-frame
	ret, frame = cap.read()

	if ret == True:
		# convert frame as Image
		img = Image.fromarray(frame)

		# Set the image from algorithm
		ld.setImage(img)
		
		# Line detect
		ld.detect()

		# Get results
		#results = ld.getResultsFrame()
		results = ld.getRaspberryPiResultsFrame()

		# Display results
		cv2.imshow("Results", np.uint8(results))
	else:
		break

	# ESC to stop algorithm
	key = cv2.waitKey(7) % 0x100
	if key == 27:
		break

# Clear all windows
cv2.destroyAllWindows()
