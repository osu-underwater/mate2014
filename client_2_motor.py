# This version of the client is designed to run with only the front two of the four motors attached to the ROV
import time
import socket
import pyjoy
import serial
import pygame
from math import *

MAX_MOTOR_VALUE = 180 # Sets the highest motor value that is able to be sent to the Arduino
MIN_MOTOR_VALUE = 25  # Sets the lowest motor value that is able to be sent to the Arduino

joy = []
recvstr = "  "

# A list of axis values from the joystick
joyAxis = [0,0,0,0]
# Each boolean represents the value from the corresponding joystick button
joyButtons = [False, False, False, False, False, False]

UDP_IP = "192.168.1.177"
UDP_PORT = 8888

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
pygame.joystick.init()
pygame.display.init()
if not pygame.joystick.get_count():
	print"\nplug in a joystick\n"
	quit()
print "\n%d joystick(s) detected" % pygame.joystick.get_count()
for i in range(pygame.joystick.get_count()):
	myjoy = pygame.joystick.Joystick(i)
	myjoy.init()
	joy.append(myjoy)
	print "Depress lower rack of buttons to quit\n"

command = [0,0,0,0]

while True:
	e = pygame.event.wait()
	if (e.type == pygame.JOYAXISMOTION or e.type == pygame.JOYBUTTONDOWN or e.type == pygame.JOYBUTTONUP):
		# Read and change information in joyAxis or joyButtons
		if e.type == pygame.JOYBUTTONDOWN:
			if (e.dict['button'] in range(6,12)):
				print "Bye!\n"
				quit()
			else:
				joyButtons[e.dict['button']] = True

		if e.type == pygame.JOYBUTTONUP:
			if (e.dict['button'] in range(0,6)):
				joyButtons[e.dict['button']] = False

		if e.type == pygame.JOYAXISMOTION:
			axis = e.dict['axis']
			value = e.dict['value']
			joyAxis[axis] = value

		# Construct Arduino command from all data
		xAxis = joyAxis[0]
		if xAxis == 0: # Prevents a divide by zero error
			xAxis = 10**-24
		yAxis = -joyAxis[1]
		rotAxis = joyAxis[2]

		mag = abs(yAxis)

		lRatio = (xAxis+1)/2
		rRatio = (xAxis-1)/-2
		#print theta/pi
		motorA = mag * lRatio
		motorA *= MAX_MOTOR_VALUE # Values usable by the Arduino Servo library
		motorB = mag * rRatio
		motorB *= MAX_MOTOR_VALUE

		if motorA < MIN_MOTOR_VALUE:
			motorA = MIN_MOTOR_VALUE
		if motorB < MIN_MOTOR_VALUE:
			motorB = MIN_MOTOR_VALUE

		# Motor direction sent as a binary number with each bit representing the motors A-B
		# x x
		# A B
		motorDirection = 0
		if yAxis > 0:
			motorDirection = 0b11
		else:
			motorDirection = 0b00

		# Create a binary button map from joyButtons
		buttonMap = 0
		for index, value in enumerate(joyButtons):
			if value:
				buttonMap |= 2**index

		# Build and output command list
		command = [int(motorA), int(motorB), motorDirection, buttonMap]
	MESSAGE = [chr(command[0]), chr(command[1]), chr(command[2]), chr(command[3])]
	sock.sendto("".join(MESSAGE), (UDP_IP, UDP_PORT))
	recvstr = sock.recv(4096)
	print "MESSAGE = {} {} {} {}".format(ord(MESSAGE[0]), ord(MESSAGE[1]), ord(MESSAGE[2]), ord(MESSAGE[3]))
	print "Received: {} {} {} {}".format(ord(recvstr[0]), ord(recvstr[1]), ord(recvstr[2]), ord(recvstr[3]))