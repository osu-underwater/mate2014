#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back
char foo;

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  char Size[4];
  if(packetSize)
  {
    if(packetSize == 4)
    {
      IPAddress remote = Udp.remoteIP();
    
      // read the packet into packetBufffer
      Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
      char Command[4];
      for(int nCom = 0; nCom < 4; nCom++)
        {
        Command[nCom] = packetBuffer[nCom];
        }
            
      if (Command[0] == 't')
      {
        digitalWrite(13, HIGH);
      }
      else if (Command[0] == 'r')
      {
        digitalWrite(12, HIGH);
      }
      else if (Command[0] == 'e')
      {
        digitalWrite(10, HIGH);
      }   
      else if (Command[0] == 'w')
      {
        analogWrite(11, 65);
      }      
      else
      {
        digitalWrite(13, LOW);
        digitalWrite(12, LOW);
        analogWrite(11, 0);
      }

      // send a reply, to the IP address and port that sent us the packet we received
      itoa(packetSize, Size, 10);
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(ReplyBuffer);
      Udp.write(packetBuffer);
      Udp.write(Size);
      Udp.endPacket();
    delay(1000);
    }
    else
    {
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write("NOPE");
      Udp.endPacket();
    }
  }
}

