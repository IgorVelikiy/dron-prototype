#include <ESP8266WiFi.h>

// CHIP_EN - 3.3V
// IO15 - GND
// IO0 - GND
// tx to pa10

#define UART_BAUD 115200
#define packTimeout 5 // ms (if nothing more on UART, then send packet)
#define bufferSize 8192

const char *ssid = "mywifi";
const char *pw = "qwerty123";
IPAddress ip(192, 168, 0, 1);
IPAddress netmask(255, 255, 255, 0);
const int port = 9876;

//////////////////////////////////////////////////////////////////////////

#include <WiFiClient.h>
WiFiServer server(port);
WiFiClient client;

uint8_t buf1[bufferSize];
uint8_t i1=0;

uint8_t buf2[bufferSize];
uint8_t i2=0;

void setup() {

  delay(500);
  Serial.begin(UART_BAUD);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, netmask);
  WiFi.softAP(ssid, pw);
  Serial.println("Starting TCP Server");
  server.begin();
}


void loop() {
  if(!client.connected()) 
  {
    client = server.available(); 
    return;
  }
  if(client.available()) 
  {
    while(client.available()) 
    {
      buf1[i1] = (uint8_t)client.read();
      if(i1<bufferSize-1) i1++;
    }
    Serial.write(buf1, i1);
    i1 = 0;
  }
  if(Serial.available()) 
  {
    while(1) 
    {
      if(Serial.available()) 
      {
        buf2[i2] = (char)Serial.read();
        if(i2<bufferSize-1) i2++;
      } 
      else 
      {
        delay(packTimeout);
        if(!Serial.available()) 
        {
          break;
        }
      }
    }
    client.write((char*)buf2, i2);
    i2 = 0;
  }
}