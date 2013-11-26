import time
import socket
import serial
import curses
import traceback

UDP_IP = "192.168.1.177"
UDP_PORT = 8888

def main(stdscr):
    global screen
    screen = stdscr.subwin(23, 79, 0, 0)
    screen.box()
    screen.hline(2, 1, curses.ACS_HLINE, 77)
    screen.refresh()
    throttle = 0
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    screen.addstr("UDP_IP = {} ".format(UDP_IP))
    screen.addstr("UDP_PORT = {}".format(UDP_PORT))

    while True:
	c = stdscr.getch()
        if c == curses.KEY_UP: throttle = throttle + 5
        elif c == curses.KEY_DOWN: throttle = throttle -5
        elif c == ord('q'): break

	if throttle > 255: throttle = 255
	if throttle < 0: throttle = 0
	sock.sendto(chr(throttle), (UDP_IP, UDP_PORT))
	
	recvstr = sock.recv(4096)
	if recvstr: screen.addstr(chr(recvstr[0]))

if __name__=='__main__':
    try:
        stdscr=curses.initscr()
        curses.noecho()
        curses.cbreak()

        stdscr.keypad(1)
        main(stdscr)

        stdscr.keypad(0)
        curses.echo()
        curses.nocbreak()
        curses.endwin()

    except:
        stdscr.keypad(0)
        curses.echo()
	curses.echo()
	curses.nocbreak()
	curses.endwin()
	traceback.print_exc()


