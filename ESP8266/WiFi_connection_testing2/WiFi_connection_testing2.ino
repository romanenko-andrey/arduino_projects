#include <SoftwareSerial.h>
#include <WiFi.h>
//#include <ESP8266WiFi.h>
//#include <Adafruit_ESP8266.h>

SoftwareSerial esp(10,9); // Rx, Tx

// WiFi config
String ssid = "uikworkwifi";
String password = "uik12345678";

// Server settings 
const String hostname = "adafruit.com";
const String uri = "/testwifi/index.html";
const int port = 80;

void setup()
{
  Serial.begin(9600);  // connect to COM
  esp.begin(9600);      // connect to ESP8266 
  //Serial.setTimeout(100);
  //esp.setTimeout(100);
  
  // Talk to ESP8266
  Serial.println("Init ESP8266....");
  esp.println("AT");
  delay(100);
  if ( esp.find("OK") == 0 ){
    Serial.println("ESP8266 not found");
    while(1);
  }
  // connect to WiFi
  Serial.println("Connecting to WiFi....");
  esp.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  
  if ( wait_ESP_answer("OK") == 0 ){
    Serial.println("Could not connect to WiFi");
    while(1);
  } else {
    Serial.println("Connected");
  }
}

void loop()
{
 // Open TCP connection
  esp.println("AT+CIPSTART=\"TCP\",\"" + hostname + "\"," + port);

  if ( wait_ESP_answer("OK") == 0 ){
    Serial.println("Could not open TCP connection");
  } else {
    Serial.println("Connected to TCP");
  }
 
  // Construct HTTP request
  String req = "GET " + uri + " HTTP/1.1\r\n" +
                        "Host: " + hostname + "\r\n" +
                        "Connection: close\r\n" +
                        "\r\n";
  int len = req.length();
  
  // Send our request length
  esp.print("AT+CIPSEND=");
  esp.println(len);

  if ( wait_ESP_answer(">") == 0 ){
    Serial.println("Error 2");
  } 
 
  // Send our http request
  esp.print(req);
  if ( wait_ESP_answer("SEND OK\r\n") == 0 ) {
    Serial.println("Error 3");
  }
  
  // Wait for a response from the server
   String string = "Not found";
   if (wait_ESP_answer(":") == 0) {
    Serial.println("Answer not found");
  }


  // Print html response
  //while ( esp.available() ) {
  //  string = esp.readString();
  //  Serial.println(string);
  // soft.println();
  //}
  
  // Look for "this," and store whatever comes next
  
  for ( unsigned long i = 0; i < 50; i++ ) {
    string = esp.readString();
    Serial.println(string);
  }
  
  // Close TCP connection
  esp.println("AT+CIPCLOSE=0");
  if ( wait_ESP_answer("OK") == 0 ) {
    Serial.println("Error close connection");
  } else {
    Serial.println();
    Serial.println("Connection closed");
  }
  
  delay(10000);
  
}

void testESP(){
  Serial.println("Send AT command to ESP8266....");
  esp.println("AT");
  delay(1000);
  if (esp.available() > 0) {
     String incomingString = esp.readString();
     Serial.print("I received: ");
     Serial.println(incomingString);
  }
  if ( Serial.available() > 0) {
    Serial.print("I received to serial: ");
    Serial.println( Serial.readString() );
  } 
}

int wait_ESP_answer(char req[]){
  int tests = 10;
  Serial.print("request = ");
  Serial.println(req);
  while ( tests > 0 ) {
   if (( esp.available()) && (esp.find(req) == 0 )){
      break;
   }
   Serial.print(".");
   delay(300);
   tests--;
  } 
  Serial.print(" tests = ");
  Serial.println(tests);
  return tests;
}

