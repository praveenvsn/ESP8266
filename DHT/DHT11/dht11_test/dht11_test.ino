// Import required libraries
#include <ESP8266WiFi.h>
#include "DHT.h"

//#define DEBUG

// replace with your channel’s thingspeak API key,
String apiKey = "ZG8IGZFRKP7SXQN2";

// WiFi parameters
const char* ssid = "sony";
const char* password = "pr@veen123";

// Pin
#define DHTPIN 2

// Use DHT11 sensor
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Host
const char* host = "api.thingspeak.com";
WiFiClient client;

void setup() {
  
  // Start Serial
  Serial.begin(115200);
  delay(10);
  
  // Init DHT 
  dht.begin();

  // We start by connecting to a WiFi network
  #ifdef DEBUG
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  #endif
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
    Serial.print(".");
    #endif
  }
  #ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  #endif
}

float prevTemperature = 0.0;
float prevHumidity = 0;
void loop() {
  float h = dht.readHumidity();
  delay(2000);
  float t = dht.readTemperature();
  
  /*
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(5000);
    return;
  }
  */
  if(isnan(h))
    h = prevHumidity;
  else
    prevHumidity = h;
  if(isnan(t))
    t = prevTemperature;
  else
    prevTemperature = t;
    
  if (client.connect(host,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(t);
           postStr +="&field2=";
           postStr += String(h);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
     #ifdef DEBUG
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees Celcius Humidity: "); 
     Serial.print(h);
     Serial.println("% send to Thingspeak");    
     #endif
  }
  client.stop();

  #ifdef DEBUG
  Serial.println("Waiting...");    
  #endif
  // thingspeak needs minimum 15 sec delay between updates
  delay(20000);
 
}



