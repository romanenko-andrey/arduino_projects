#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

// PIN-configs
const int button = D0;

// WiFi config
const char ssid[] = "uikworkwifi";
const char password[] = "uik12345678";

// IFTTT config
const String ifttt_trigger = "my_reminder";
const String ifttt_key = "cHxPsKJw4Rni22JXbFok_j";

// Server, file, and port
const String hostname = "maker.ifttt.com";
const String uri = "/trigger/" + ifttt_trigger + "/with/key/" + ifttt_key;
const int port = 80;

//WiFi client
WiFiClient client;

void setup(){ 
  // Init buttom
  pinMode(button, INPUT);
  
  // Init Serial
  Serial.begin(9600);
  delay(100);
  
  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected!");
  
  Serial.print("IP Adress = ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  // randomize initialization 
  randomSeed(micros());
}

String body, req;
int btn_prev = HIGH;

void loop()
{
  long mv = analogRead(0) * 3300 / 1024;
  // Construct HTTP request
  body = "{\"value1\": \" voltage = " + String(mv) + " mV\"}"; 
                  
  req = "POST " + uri + " HTTP/1.1\r\n" + 
        "Host: " + hostname + "\r\n" +
        "Content-Type: application/json\r\n" +
        "Content-Length: " + body.length() + "\r\n" +
        "Connection: close\r\n\r\n" +
        body + "\r\n";
        
  int btn_state = digitalRead(button);
  
  if ( (btn_prev == HIGH) && (btn_state == LOW) ) {
    //Send request to Server and wait unswer
    sent_to_IFTTT();
  }
  
  // Store previous button state
  btn_prev = btn_state;
  delay(100);
}

void sent_to_IFTTT(){
  // Connect to server
  Serial.print("Connecting to ");
  Serial.println(hostname);
  if ( client.connect(hostname, port) == 0 ) {
    Serial.println("Connection failed");
  }

  // Send request for file from server
  Serial.println("HTTP request");
  Serial.print(req);
  
  client.print(req);
  delay(500);
  
  // Print reply from server
  while ( client.available() ) {
    String ln = client.readStringUntil('\r');
    Serial.print(ln);
  }

  // Close TCP connection
  client.stop();
  Serial.println();
  Serial.println("Connection closed");
}


