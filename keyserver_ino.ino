#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h> 
//#include <TimerOne.h>
#include <Servo.h>


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 8888;      // local port to listen on

Servo THRUSTL;
Servo THRUSTR;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  //Timer1.initialize(256);
  THRUSTL.attach(11);
  THRUSTL.writeMicroseconds(1500);
  THRUSTR.attach(12);
  THRUSTR.writeMicroseconds(1500);
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
      uint8_t SpeedX = packetBuffer[0];
      long SpeedXL = SpeedX-90;
      if(SpeedXL<0)
        SpeedXL = 0;
      if(SpeedXL>90)
        SpeedXL = 90;
      long SpeedXR = 90-SpeedX;
      if(SpeedXR<0)
        SpeedXR = 0;
      if(SpeedXR>90)
        SpeedXR=90;
      uint8_t SpeedY = 115 - packetBuffer[1];
      signed long SpeedZ = packetBuffer[2];
      signed long SpeedExtra = packetBuffer[3];
      uint8_t SpeedL = SpeedY + SpeedXL;
      uint8_t SpeedR = SpeedY + SpeedXR;
//      SpeedL -= 65;
  //    SpeedR -= 65;
      if(SpeedL<25)
        SpeedL = 25;
  //    if(SpeedL > 115 && SpeedL < 181)
  //      SpeedL = 115;
      if(SpeedR < 25 || SpeedR>180)
        SpeedR = 25;
      if(SpeedR >115 && SpeedR <181)
        SpeedR = 115;
      SpeedL = ((SpeedL-25)/90.0)*30+25;
      SpeedR = ((SpeedR-25)/90.0)*30+25;
      if(packetBuffer[1]>90){
        SpeedL = 25;
        SpeedR = 25;
      }
      
      //Timer1.pwm(11, Speed);
      //Timer1.pwm(12, Speed);
      THRUSTL.write(SpeedL);
      THRUSTR.write(SpeedR);
      // send a reply, to the IP address and port that sent us the packet we received
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      //Udp.write(packetBuffer);
      Udp.write(SpeedX);
      Udp.write(SpeedY);
      Udp.write(SpeedL);
      Udp.write(SpeedR);
      Udp.endPacket();
      }
  }
} 


