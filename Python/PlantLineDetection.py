# -*- coding: utf-8 -*-
from PIL import Image, ImageDraw
import time
import numpy as np
import cv2

class LineDetection():
    images = None
    data = None
    height = None
    width = None

    # define the adaptative thresholds and properties
    heightThreshold = 0
    colorThreshold = [70, 255]
    size = [320, 180]
    #size = [1024, 768]

    # results display properties
    resultFrameSize = [220, 160]
    grid = [3,3]

    # for benchmark
    startTime = 0
    stopTime = 0
    fps = 0
    frameCounter = 0
    printParameters = True
    printLineParameters = False
    printHoughParameters = False

    # lines
    lines = None
    finalLines = None
    parallelLineDistance = 20 # px

    def __init__(self):
        self.setParametersDefault()

    def setParametersDefault(self):
        self.images = list()
        self.data = list()
        self.height = 0
        self.width = 0
        self.startTime = 0
        self.stopTime = 0
        self.fps = 0
        self.lines = list()
        self.finalLines = list()

    def setImage(self, image):
        # refresh parameters
        self.setParametersDefault()

        self.startTime = time.time()

        # resize image
        image.thumbnail(self.size, Image.ANTIALIAS)

        self.width = image.size[0]
        self.height = image.size[1]

        # create the blank image 8 bits
        blankImage = Image.new("L", image.size)

        # load the images from the algorithm
        self.images.append(list([image.copy(),"Original Image"]))
        self.images.append(list([blankImage.copy(),"Green Extraction"]))
        self.images.append(list([blankImage.copy(),"Threshold Filter"]))
        self.images.append(list([blankImage.copy(),"Object Detection"]))
        self.images.append(list([blankImage.copy(),"Skeletonization"]))
        self.images.append(list([image.copy(),"Hough Lines"]))
        self.images.append(list([image.copy(),"Final Result"]))

        # load the data of images
        for image in self.images:
            self.data.append(image[0].load())

    def detect(self):
        for y in xrange(self.height):
            isLine = False
            x1 = x2 = None
            for x in xrange(self.width):
                r,g,b = self.data[0][x,y]

                # green extraction (method by GUO-QUAN JIANG)
                greyLevel = 0
                if y > self.heightThreshold:
                    if (2*g) <= (r + b):
                        greyLevel = 0
                    elif (2*g) >= r+b+255:
                        greyLevel = 255
                    else:
                        greyLevel = (2*g)-r-b
                else:
                    greyLevel = 0

                self.data[1][x,y] = greyLevel

                # threshold filter
                if not (self.colorThreshold[0] <= greyLevel <= self.colorThreshold[1]):
                    greyLevel = 0
                else:
                    greyLevel = 255

                self.data[2][x,y] = greyLevel

                # detect the object
                greyLevel = self.data[2][x,y]
                if isLine == False and greyLevel == 255:
                    x1 = x2 = x
                    isLine = True
                elif isLine == True and greyLevel == 255:
                    x2 = x
                elif isLine == True and greyLevel == 0:
                    self.data[3][x1,y] = 255
                    self.data[3][x2,y] = 255
                    if x2 != 0:
                        self.data[4][(x1+x2)//2,y] = 255
                    isLine = False
                    x1 = x2 = None

        img = np.asarray(self.images[4][0])
        #linesHough = cv2.HoughLines(img,2,np.pi/90,40, 100, 1)
        linesHough = cv2.HoughLines(img,2,np.pi/45,30)
        if linesHough != None:
            for rho,theta in linesHough[0]:
                a = np.cos(theta)
                b = np.sin(theta)
                x0 = a*rho
                y0 = b*rho
                x1 = int(x0 + 1000*(-b))
                y1 = int(y0 + 1000*(a))
                x2 = int(x0 - 1000*(-b))
                y2 = int(y0 - 1000*(a))
                self.lines.append([x1,y1,x2,y2])
                if self.printHoughParameters == True: print "rho=", rho, "\ttheta=", theta, "\ta=" , a , "\tb=" , b , "\tx0=" , x0 , "\ty0=" , y0 , "\tx1=" , x1 , "\ty1=" , y1 , "\tx2=" , x2 , "\ty2=" , y2

        draw = ImageDraw.Draw(self.images[5][0])
        draw2 = ImageDraw.Draw(self.images[6][0])

        lastLine = None
        for line in self.lines:
            x1,y1,x2,y2 = line

            draw.line(line, fill=(0,255,0,128), width=2)

            # calculate the new points on image with equation of the line
            y3 = 2
            x3 = (x2*y3+x1*y2-x2*y1-x1*y3)/(y2-y1)

            y4 = self.height - y3
            x4 = (x2*y4+x1*y2-x2*y1-x1*y4)/(y2-y1)

            l = x3,y3,x4,y4

            if lastLine == None:
                lastLine = list()
                lastLine.append(l)
                draw2.line(l, fill=(0,255,0,128), width=2)
            else:
                count = 0
                for ll in lastLine:
                    p1,p2 = self.intersectPoint(ll, l)
                    if 0 < p1 < self.width and 0 < p2 < self.height:
                        count += 1
                    if self.isParallelLines(ll, l) is True:
                        if self.lineDistance(ll, l) < self.parallelLineDistance:
                            count += 1
                if count == 0:
                    self.finalLines.append(l)
                    draw2.line(l, fill=(0,255,0,128), width=2)
                lastLine.append(l)

        # statistics
        self.frameCounter += 1
        self.stopTime = time.time()
        self.fps = 1 / (self.stopTime - self.startTime)

        if self.printParameters == True: print "size=", self.width, "x", self.height, "heightThreshold=", self.heightThreshold, "colorThreshold=", self.colorThreshold, "numLines=", len(self.lines), "finalLines=", len(self.finalLines), "fps=", self.fps

    def isParallelLines(self, l1, l2):
        x1,y1,x2,y2 = l1
        x3,y3,x4,y4 = l2

        m1 = 0
        m2 = 0

        if (x2-x1) != 0:
            m1 = (y2-y1)/(x2-x1)
        if (x4-x3) != 0:
            m2 = (y4-y3)/(x4-x3)

        if m1 == m2:
            return True

        return False

    def lineDistance(self, l1, l2):
        x1,y1,x2,y2 = l1
        x3,y3,x4,y4 = l2

        if x1 < x3:
            return x3 - x1
        if x1 >= x3:
            return x1 - x3

    def intersectPoint(self, l1, l2):
        x1,y1,x2,y2 = l1
        x3,y3,x4,y4 = l2

        # a1x + b1y + c1 = 0
        # a2x + b2y + c2 = 0
        a1 = y1 - y2
        b1 = x2 - x1
        c1 = (x1*y2) - (x2*y1)

        a2 = y3 - y4
        b2 = x4 - x3
        c2 = (x3*y4) - (x4*y3)

        #a1,b1,c1 = 2,1,4
        #a2,b2,c2 = 1,-1,-1

        if self.printLineParameters == True:
            print "Equacao da reta:"
            print "r1: ", a1, "x + ", b1, "y + ", c1, " = 0"
            print "r2: ", a2, "x + ", b2, "y + ", c2, " = 0"

            print "Equacao da reta ajustada:"
            print "r1: ", a1, "x + ", b1, "y  = ", -c1
            print "r2: ", a2, "x + ", b2, "y  = ", -c2

        # mount the linear system
        a = np.matrix([[a1,b1],[a2,b2]])
        b = np.matrix([[-c1],[-c2]])

        # determinant
        detA = np.linalg.det(a)

        if detA != 0:
            # solve the eq ax=b
            p = np.linalg.solve(a,b)
            return p[0][0], p[1][0]
        else:
            # discards the line by placing the intersection point outside the image
            return -1, -1

    def getRaspberryPiResultsFrame(self):
		numCols = self.grid[0]
		numRows = self.grid[1]
		heightVar = 50
		fill = 255
		results = Image.new('RGB', (numCols*self.resultFrameSize[0], (numRows*self.resultFrameSize[1])+heightVar))
		contx = 0
		conty = 0
		for image in self.images:
			imageCopy = image[0].copy()
			imageCopy.thumbnail(self.resultFrameSize)
			draw = ImageDraw.Draw(imageCopy)
			draw.text((5, 5), image[1], fill)
			results.paste(imageCopy,(contx*self.resultFrameSize[0], conty*self.resultFrameSize[1]))
			contx += 1
			if contx == numCols:
				conty += 1
				contx = 0
		# print the variables
		varStatus = Image.new('RGB', (results.size[0], heightVar))
		draw = ImageDraw.Draw(varStatus)

		draw.text((5, 5), ("frame resolution = %s x %s" %(self.width, self.height)), fill)
		draw.text((5, 20), ("frame counter = %s" %(self.frameCounter)), fill)
		draw.text((5, 35), ("time per frame = %.2f" %(self.stopTime - self.startTime)), fill)

		draw.text((200, 5), ("heightThreshold = %s" %(self.heightThreshold)), fill)
		draw.text((200, 20), ("colorThreshold = %s" %(self.colorThreshold)), fill)
		draw.text((200, 35), ("fps = %.2f" %(self.fps)), fill)

		draw.text((400, 5), ("numberOfLines = %s" %(len(self.lines))), fill)
		draw.text((400, 20), ("finalLines = %s" %(len(self.finalLines))), fill)

		draw.text((results.size[0]-80, heightVar-15), "ESC for stop.", fill)
		results.paste(varStatus,(0, results.size[1]-heightVar))
		return results

    def getResultsFrame(self):
        numCols = self.grid[0]
        numRows = self.grid[1]
        heightVar = 50
        fill = (255,255,255)
        results = Image.new('RGB', (numCols*self.resultFrameSize[0], (numRows*self.resultFrameSize[1])+heightVar))
        contx = 0
        conty = 0
        for image in self.images:
            imageCopy = image[0].copy()
            imageCopy.thumbnail(self.resultFrameSize)
            draw = ImageDraw.Draw(imageCopy)
            draw.text((5, 5), image[1], fill)
            results.paste(imageCopy,(contx*self.resultFrameSize[0], conty*self.resultFrameSize[1]))
            contx += 1
            if contx == numCols:
                conty += 1
                contx = 0
        # print the variables
        varStatus = Image.new('RGB', (results.size[0], heightVar))
        draw = ImageDraw.Draw(varStatus)

        draw.text((5, 5), ("frame resolution = %s x %s" %(self.width, self.height)), fill)
        draw.text((5, 20), ("frame counter = %s" %(self.frameCounter)), fill)
        draw.text((5, 35), ("time per frame = %.2f" %(self.stopTime - self.startTime)), fill)

        draw.text((200, 5), ("heightThreshold = %s" %(self.heightThreshold)), fill)
        draw.text((200, 20), ("colorThreshold = %s" %(self.colorThreshold)), fill)
        draw.text((200, 35), ("fps = %.2f" %(self.fps)), fill)

        draw.text((400, 5), ("numberOfLines = %s" %(len(self.lines))), fill)
        draw.text((400, 20), ("finalLines = %s" %(len(self.finalLines))), fill)

        draw.text((results.size[0]-80, heightVar-15), "ESC for stop.", fill)
        results.paste(varStatus,(0, results.size[1]-heightVar))
        return results

if __name__ == "__main__":
	import indexv
