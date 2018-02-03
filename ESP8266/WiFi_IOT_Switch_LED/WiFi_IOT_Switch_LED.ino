#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

// PIN-configs
const int button = D0;
const int led = D1;
const int led_OK = D2;

// WiFi config
const char ssid[] = "uikworkwifi";
const char password[] = "uik12345678";

// IFTTT config
const String ifttt_trigger = "my_reminder";
const String ifttt_key = "cHxPsKJw4Rni22JXbFok_j";

// ThingSpeak config
const unsigned long channel_id = 418536;
const char write_api_key[] = "2N0JKZHSTVSO504C";
const char read_api_key[] = "CEZS7115ALS46ML7";
const char user_api_key[] = "";


// Server, file, and port
const String hostname = "maker.ifttt.com";
const String uri = "/trigger/" + ifttt_trigger + "/with/key/" + ifttt_key;
const int port = 80;

//WiFi client
WiFiClient client;



void setup(){ 
  // Init buttom
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led_OK, OUTPUT);
  
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
}

String body, req;
int btn_prev = HIGH;

void loop()
{
  
  Serial.println("Connecting to ThingSpeak ...");
  long val = ThingSpeak.readLongField (channel_id, 1, read_api_key);
  Serial.println( "Last value is " + String(val) );
  if (val > 0) {
    digitalWrite(led, HIGH);
    Serial.println("Led - ON");
  } else {
    digitalWrite(led, LOW);
    Serial.println("Led - OFF");
  }
  
  long start_time = millis();
  //при нажатии на кнопку
  //отправить SMS со значением напряжения на аналоговом входе
  //мне на электронную почту через IFTTT      
  while ( (millis() - start_time) < 1000 ){
    int btn_state = digitalRead(button);
    //if ( (btn_prev == HIGH) && (btn_state == LOW) ) {
    
    if (btn_state == LOW) {
      digitalWrite(led_OK, HIGH);
      long mv = analogRead(0) * 3300 / 1024;
      // Construct HTTP request
      body = "{\"value1\": \" voltage = " + String(mv) + " mV\"}"; 
                      
      req = "POST " + uri + " HTTP/1.1\r\n" + 
            "Host: " + hostname + "\r\n" +
            "Content-Type: application/json\r\n" +
            "Content-Length: " + body.length() + "\r\n" +
            "Connection: close\r\n\r\n" +
            body + "\r\n";
      //Send request to Server and wait unswer
      sent_to_IFTTT();
      digitalWrite(led_OK, LOW);
      break;
    }
    // Store previous button state
    btn_prev = btn_state;
  }
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


