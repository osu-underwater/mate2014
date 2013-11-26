import time
import socket

UDP_IP = "192.168.1.177"
UDP_PORT = 8888

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
a = 1

while a == 1:
    MESSAGE = raw_input("Value?")
    sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
    time.sleep(1.0)
    print sock.recv(4096)
