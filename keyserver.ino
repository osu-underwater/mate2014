#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h> 
//#include <TimerOne.h>
#include <Servo.h>


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0x90, 0xAD, 0xD2, 0x0D, 0x8B, 0x23 };
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 8888;      // local port to listen on

Servo MOTORA;
Servo MOTORB;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  //Timer1.initialize(256);
  MOTORA.attach(11);
  MOTORA.writeMicroseconds(1500);
  MOTORB.attach(12);
  MOTORB.writeMicroseconds(1500);
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
}

void loop() 
{
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    if(packetSize < 256)
      {
      IPAddress remote = Udp.remoteIP();
    
      // read the packet into packetBufffer
      Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
      uint8_t SpeedA = packetBuffer[0];
      uint8_t SpeedB = packetBuffer[1];
      
      THRUSTL.write(SpeedA);
      THRUSTR.write(SpeedB);
      // send a reply, to the IP address and port that sent us the packet we received
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      //Udp.write(packetBuffer);
      Udp.write(packetBuffer[0]);
      Udp.write(packetBuffer[1]);
      Udp.write(packetBuffer[2]);
      Udp.write(packetBuffer[3]);
      Udp.write(packetBuffer[4]);
      Udp.write(packetBuffer[5]);
      Udp.endPacket();
      }
  }
} 


