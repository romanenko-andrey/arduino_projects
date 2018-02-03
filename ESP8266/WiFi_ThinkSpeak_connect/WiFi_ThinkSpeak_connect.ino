#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

// PIN-configs
const int button = D0;

// WiFi config
const char ssid[] = "uikworkwifi";
const char password[] = "uik12345678";

// ThingSpeak config
const unsigned long channel_id = 408018;
const char write_api_key[] = "Z527ZSTL1PSYQARK";
const char read_api_key[] = "J3TMXJP113AXKLPE";
const char user_api_key[] = "ZBY7V8P556FXOND2";

// Server, file, and port
const char hostname[] = "api.thingspeak.com";
cont String uri[] = "/channels/" + String(channel_id) + "/feeds?api_key="
const String uri_with_read_api_key  = uri + read_api_key;
const String uri_with_write_api_key = uri + write_api_key;
const String uri_with_user_api_key  = uri + user_api_key;
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
  
  // Connect to ThingSpeak
  ThingSpeak.begin(client);
  // randomize initialization 
  randomSeed(micros());
}

void loop()
{
  long start_time = millis();
  float temp = random(1, 51);
  float voltage = analogRead(0) * 3.3 / 1024; 
  Serial.print("Posting temperature = " + String(temp, 1) + "°C; ");
  Serial.print("and analog_input voltage = " + String(voltage, 1) + "V ");
  Serial.println("to ThingSpeak");
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, voltage);
  ThingSpeak.writeFields(channel_id, write_api_key);
  
  read_feeds("&average=10");
  //readRaw();
  readStatus();

  readLastInfo(1);
  readStatus();
   
  //wait for 30s bitween cycles
  while ( (millis() - start_time) < 30000 ){
    delay(1000);  
    int val = analogRead(0);    // считываем напряжение с аналогового входа
    Serial.println(val); 
    if ( digitalRead(button) == LOW ){
      Serial.println("button pressed");
      clear_channel();
      delay(5000);
      start_time -= 30000;
    }
    
  }
  
}

void readRaw(){
  //return ThingSpeak.readRaw(channel_id, "feeds/average=10", read_api_key);
  String response = ThingSpeak.readRaw(channel_id, String("feeds/days=1"), read_api_key);
  Serial.print("Response: "); 
  Serial.print(response);
}

void readStatus(){
  int resultCode = ThingSpeak.getLastReadStatus();
  Serial.print("Last HTTP status codes was: "); 
  Serial.println(resultCode);
}

void readLastInfo(int field_no) {
  float value = ThingSpeak.readFloatField(channel_id, field_no, read_api_key);
  Serial.print("Latest value of chanel #: " + String(field_no) + " = "); 
  Serial.println(value); 
}

void clear_channel(){
  // Connect to server
  Serial.print("Connecting to ");
  Serial.println(hostname);
  if ( client.connect(hostname, port) == 0 ) {
    Serial.println("Connection failed");
    return;
  }

  // Send request for file from server
  client.print("DELETE " + uri_with_user_api_key + " HTTP/1.1\r\n" + 
                "Host: " + hostname + "\r\n" +
                "Connection: close\r\n\r\n");
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

void read_feeds(String params){
  // Connect to server
  Serial.print("Connecting to ");
  Serial.print(hostname);
  Serial.print("with params ");
  Serial.println(uri_with_read_api_key + params);
  if ( client.connect(hostname, port) == 0 ) {
    Serial.println("Connection failed");
  }

  // Send request for file from server
  client.print("GET " + uri_with_read_api_key + params + " HTTP/1.1\r\n" + 
                "Host: " + hostname + "\r\n" +
                "Connection: close\r\n\r\n");
  delay(500);
/*
  // Print reply from server
  while ( client.available() ) {
    String ln = client.readStringUntil('\r');
    Serial.print(ln);
  }
*/
// Look for "feeds," and store whatever comes next
  String res = "";
  for ( unsigned long i = 0; i < 500; i++ ) {
    if ( client.find("feeds") ) {
      /*for (int i = 0; i < 7; i++) {
        client.readStringUntil('created_at');
        delay(10);
      }*/
      while ( client.available() ) {
        String ln = client.readStringUntil(',');
        res = res + ln + "\r\n";
        //Serial.print(ln);
      }
      break;
    }
    delay(10);
  }
  Serial.println();
  Serial.println(res);
  

  // Close TCP connection
  client.stop();
  Serial.println();
  Serial.println("Connection closed");
}
