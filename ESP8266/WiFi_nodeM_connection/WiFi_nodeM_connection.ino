#include <ESP8266WiFi.h>

// WiFi config
const char ssid[] = "uikworkwifi";
const char password[] = "uik12345678";

// Server, file and port
const char hostname[]= "adafruit.com";
const String uri = "/testwifi/index.html";

//const char hostname[]= "https://query.yahooapis.com";
//const String uri = "/v1/public/yql?q=select%20item.condition%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22kiyv%22)&u=c&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys";

//https://query.yahooapis.com/v1/public/yql?q=select%20item.condition%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22kiyv%22)&u=c&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys

const int port = 80;

//WiFi client
WiFiClient client;

IPAddress ip;

void setup(){ 
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
  ip = WiFi.localIP();
  Serial.println(ip);
}

void loop()
{
  // Connect to server
  Serial.print("Connecting to ");
  Serial.println(hostname);
  if (client.connect(hostname, port) == 0){
    Serial.println("Connection failed");
  }
  //Send request for file
  client.print("GET " + uri + " HTTP/1.1\r\n" +
               "Host: " + hostname + "\r\n" +
               "Connection: close\r\n" +
               "\r\n");
  delay(500);
  
  // Print all reply from server
  
  while ( client.available() ){
    String ln = client.readStringUntil('\r');
    Serial.print(ln);
  }
  
  /*
  // Look for "this," and store whatever comes next
  String string = "Not found";
  for ( unsigned long i = 0; i < 500; i++ ) {
    if ( client.find("this, ") ) {
      string = client.readStringUntil('\n');
      while ( client.available() ) {
        client.read();
      }
      break;
    }
    delay(10);
  }
  Serial.println(string);
  */
  //Close TCP connection
  client.stop();
  Serial.println();
  Serial.println("Connection close");
  delay(10000);
}
