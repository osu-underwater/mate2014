import time
import socket
import pyjoy
import serial
import pygame

joy = []
recvstr = "  "

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
    print "Depress trigger (button 0) to quit\n"

command = [0,0,0,0]

while True:
    e = pygame.event.wait()
    if (e.type == pygame.JOYAXISMOTION or e.type == pygame.JOYBUTTONDOWN):
        if e.type == pygame.JOYBUTTONDOWN:
            if (e.dict['button'] == 0):
                print "Bye!\n"
                quit()
        if e.type == pygame.JOYAXISMOTION:
            naxis = e.dict['axis']
            nvalue = int((((e.dict['value'] + 1)/2) * 180.0))
            command[naxis] = nvalue
    MESSAGE = chr(command[0])
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
    recvstr = sock.recv(4096)
    if not (len(recvstr) > 1): print (recvstr[0])
    else: print (recvstr[0]), "  ", ord(recvstr[1])
    print "command = %d %d %d %d" % (command[0], command[1], command[2], command[3])
    print "MESSAGE = %d" % ord(MESSAGE)
    print "naxis = %d, nvalue = %f value = %f" % (naxis, nvalue, e.dict['value'])
