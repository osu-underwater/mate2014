#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h> 
#include <TimerOne.h>
#include <TimerThree.h>
#include <Servo.h>


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 8888;      // local port to listen on

Servo CCUD;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Timer1.initialize(256);
  Timer3.initialize(256);
  CCUD.attach(13);
  CCUD.writeMicroseconds(1500);
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(4,OUTPUT);
}

void loop() 
{
  // if there's data available, read a packet
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  int packetSize = Udp.parsePacket();
  char Size[4];
  if(packetSize)
  {
    if(packetSize < 256)
      {
      IPAddress remote = Udp.remoteIP();
    
      // read the packet into packetBufffer
      Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
      char SpeedX = packetBuffer[0];
      char SpeedY = packetBuffer[1];
      char Speed3 = packetBuffer[2];
      char Speed4 = packetBuffer[3];
      char SpeedL = SpeedX + SpeedY - 128;
      char SpeedR = SpeedY - SpeedX + 128;
      char SpeedZ = Speed3 - 100;
      if(SpeedL < 0)
        {SpeedL = 0;}
      else if (SpeedL > 255)
        {SpeedL = 255;}
      if(SpeedR < 0)
        {SpeedR = 0;}
      else if (SpeedR > 255)
        {SpeedR = 255;}
      if(SpeedZ < 0)
        {SpeedZ = 0;}
      else if (SpeedZ > 255)
        {SpeedZ = 255;}
      Timer1.pwm(11, SpeedL);
      Timer3.pwm(12, SpeedR);
      CCUD.write(SpeedZ);
      // send a reply, to the IP address and port that sent us the packet we received
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      //Udp.write(packetBuffer);
      Udp.write(SpeedL);
      Udp.write(SpeedR);
      Udp.write(Speed3);
      Udp.write(Speed4);
      Udp.endPacket();
      }
  }
} 


