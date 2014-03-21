from SimpleCV import *
from time import *
from math import *
import ImageDraw

imgOne = Image("1.jpg")
imgTwo = Image("2.jpg")
imgThree = Image("3.jpg")
imgFour = Image("4.jpg")
imgFive = Image("5.jpg")

imgList = [imgOne,imgTwo,imgThree,imgFour,imgFive]

def getBlobs(img):
	thresh = 1
	blobs = img.findBlobs(-1, thresh)
	while len(blobs)>2 and thresh<500:
		thresh+=2
		blobs = img.findBlobs(-1,thresh)
	return blobs

def subtractColor(img, color):
	return img-img.colorDistance(color)

def distanceFromColor(pixel, color):
	return sqrt((pixel[0]-color[0])**2+
				(pixel[1]-color[1])**2+
				(pixel[2]-color[2])**2)

def closestColorR(img, color):
	minDist = 255**3
	for x in range(img.width/2, img.width):
		for y in range(img.height/2):
			colorDist = distanceFromColor(img.getPixel(x,y), color)
			if colorDist < minDist:
				minDist = colorDist
				loc = (x,y)
	return loc

def closestColorL(img, color):
	minDist = 255**3
	for x in range(img.width/2):
		for y in range(img.height/2):
			colorDist = distanceFromColor(img.getPixel(x,y), color)
			if colorDist < minDist:
				minDist = colorDist
				loc = (x,y)
	return loc

def assembleMosaic(imgList):
	cornerPoints = []
	pastePoints = []
	mosaicWidth = 0
	mosaicHeight = 0
	colorOrder = [Color.BLACK, Color.RED, Color.ORANGE, Color.YELLOW, Color.GREEN, Color.BLUE]
	
	for index in range(len(imgList)):
		lPoint = closestColorL(imgList[index], colorOrder[index])
		rPoint = closestColorR(imgList[index], colorOrder[index+1])
		#imgList[index].drawCircle(lPoint, 50, Color.RED, 10)
		#imgList[index].drawCircle(rPoint, 50, Color.RED, 10)
		#imgList[index].show()
		#sleep(1)
		cornerPoints.append(lPoint)
		cornerPoints.append(rPoint)
		if index == 0:
			mosaicWidth = imgList[index].width
			mosaicHeight = imgList[index].height
			pastePoints.append((0,0))
		else:
			mosaicWidth += imgList[index].width
			mosaicHeight += imgList[index].height
			# X is found as the position the previous image was pasted at + its rPoint x + current lPoint x
			# Y is simply the distance between the current lPoint y and the prevoius rPoint y
			mosaicPos = (pastePoints[index-1][0]+cornerPoints[(index-1)*2+1][0]-lPoint[0],
				lPoint[1]-cornerPoints[(index-1)*2+1][1])
			pastePoints.append(mosaicPos)
		print pastePoints[index]

	mosaic = Image((mosaicWidth, mosaicHeight))
	mosaicDraw = ImageDraw.Draw(mosaic.getPIL())

	for index in range(len(pastePoints)):
		mosaic.getPIL().paste(imgList[index].getPIL(), pastePoints[index])
	mosaic.getPIL().show()
	sleep(2)

assembleMosaic(imgList)