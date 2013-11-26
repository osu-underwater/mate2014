Solenoid Driver
===================================

Hey folks!
This is code for the tiny26 solenoid driver, written by your wonderful friend Marshal Horn.

Currently this is untested.  I'll get around to that over break.  But, here's what it should do:

1. Wait for a high vale on PB6
2. Wait for 1 byte from SPI
3. Check the data to make sure you aren't trying to extend and retract a piston at the same time.  (If you are, it won't switch that solenoid either direction)
4. Update the solenoid positions


To be implemented:
------------------------
Watchdog.

A watchdog is a timer used to make sure something is still active, for safety reasons.  If we are in the middle of a descent, and the data cable between the solenoid board and the Arduino ggets unplugged, what do we want to do?  Probably the safest thing to do would be to stop the buyancy pistons where they are.  This can be done with a watchdog.  When the watchdog timer "expires", the hydraulic valves would be disabled, and this would hopefully prevent the ROV from running into things.

The watchdog should be "fed" whenever a valid SPI message is received.  If one isn't received for a while, then we assume the cable is disconnected, and we disable the outputs
