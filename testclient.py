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
            nvalue = int((((e.dict['value']+ 1)/2) * 255.0))
            command[naxis] = nvalue
    MESSAGE = [chr(command[0]), chr(command[1]), chr(command[2]), chr(command[3])]
    sock.sendto(("".join(MESSAGE)), (UDP_IP, UDP_PORT))
    recvstr = sock.recv(4096)
#    if not (len(recvstr) > 1): print (recvstr[0]*256)
#    else: print (recvstr[0]*256+recvstr[1])
    print "command = {} {} {} {}".format(command[0], command[1], command[2], command[3])
    print "Message sent: {}".format("".join(MESSAGE))
    print "recvstr: {} {} {} {}".format(recvstr[0], recvstr[1], recvstr[2], recvstr[3])
    print "received: {} {} {} {}".format(ord(recvstr[0]), ord(recvstr[1]), ord(recvstr[2]), ord(recvstr[3]))
    print "naxis = {}, nvalue = {} value = {}".format(naxis, nvalue, e.dict['value'])
