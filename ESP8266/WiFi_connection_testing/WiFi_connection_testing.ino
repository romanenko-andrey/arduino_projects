#include <SoftwareSerial.h>
//#include <ESP8266WiFi.h>
#include <Adafruit_ESP8266.h>

SoftwareSerial soft(8,9); // Rx, Tx

// WiFi config
String ssid = "uikworkwifi";
String password = "uik12345678";
byte at[]={0x41, 0x54, 0x13, 0x10};  

void setup()
{
  Serial.begin(9600);  // connect to ESP8266
  soft.begin(9600);      // connect to COM
  // Talk to ESP8266
  soft.println("Init ESP8266....");
  Serial.println("AT");
  delay(100);
  if ( Serial.find("OK") == 0 ){
    soft.println("ESP8266 not found");
    //while(1);
  }
  // connect to WiFi
  soft.println("Connecting to WiFi....");
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(100);
  if ( Serial.find("OK") == 0 ){
    soft.println("Could not connect to WiFi");
    //while(1);
  } else {
    soft.println("Connected");
  }
}

void loop()
{
  soft.println("Send AT command to ESP8266....");
  //Serial.write(0x41);
  //Serial.write(0x54);
  //Serial.write(0x13);
  //Serial.write(0x10);
  Serial.write((uint8_t*)at, sizeof(at));
  delay(10);
  if (Serial.available() > 0) {
     String incomingString = Serial.readString();
     soft.print("I received: ");
     soft.println(incomingString);
  }
  delay(1000);
}
