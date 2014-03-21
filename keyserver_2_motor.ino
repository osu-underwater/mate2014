// This version of the server is designed to run with only the front two of the four motors attached to the ROV
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h> 
//#include <TimerOne.h>
#include <Servo.h>


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
//ROV Arduino
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//Ryan Arduino
//byte mac[] = {  
//  0x90, 0xAD, 0xD2, 0x0D, 0x8B, 0x23 };
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
  pinMode(11,OUTPUT); // Motor A
  pinMode(12,OUTPUT); // Motor B
  pinMode(13,OUTPUT); // Arduino LED
  pinMode(22,OUTPUT); // H-Bridge A
  pinMode(24,OUTPUT); // H-Bridge B
  pinMode(26,OUTPUT); // Valve A-Up
  pinMode(28,OUTPUT); // Valve A-Down
  pinMode(30,OUTPUT); // Valve B-Up
  pinMode(32,OUTPUT); // Valve B-Down
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
    uint8_t MotorDirection = packetBuffer[2];
    uint8_t ButtonMap = packetBuffer[3];

    if (MotorDirection == 3)
    {
      digitalWrite(22,HIGH);
      digitalWrite(24,HIGH);
    }
    else
    {
      digitalWrite(22,LOW);
      digitalWrite(24,LOW);
    }

    if (ButtonMap & (1 << 2))
    {
      digitalWrite(26,LOW);
      digitalWrite(28,HIGH);
      digitalWrite(30,LOW);
      digitalWrite(32,HIGH);
    }
    else if (ButtonMap & (1 << 3))
    {
      digitalWrite(26,HIGH);
      digitalWrite(28,LOW);
      digitalWrite(30,HIGH);
      digitalWrite(32,LOW);
    }
    else
    {
      digitalWrite(26,LOW);
      digitalWrite(28,LOW);
      digitalWrite(30,LOW);
      digitalWrite(32,LOW);
    }
    
    MOTORA.write(SpeedA);
    MOTORB.write(SpeedB);
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write(packetBuffer);
    Udp.write(packetBuffer[0]);
    Udp.write(packetBuffer[1]);
    Udp.write(packetBuffer[2]);
    Udp.write(packetBuffer[3]);
    Udp.endPacket();
    }
  }
}