/* 
  This a simple example of the aREST Library for the ESP8266 WiFi chip. 
  See the README file for more details.
 
  Written in 2015 by Marco Schwartz under a GPL license. 
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>

//#define ESP8266
int LED_PIN = 2;

//IPAddress staticip(192,168,2,107);
//IPAddress gatewayip(192,168,2,1);
//IPAddress subnetmask(255,255,255,255);

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "Virus"; //This is case sensitive. Be careful.
const char* password = "pr@veen123";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
  
  // Function to be exposed
  rest.function("led",ledControl);
  
  // Give name and ID to device
  rest.set_id("2");
  rest.set_name("esp8266_NEW");

  //Set Static IP 
  //WiFi.config(staticip, gatewayip, subnetmask);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
 
}

// Custom function accessible by the API
int ledControl(String command) {
  
  // Get state from command
  int state = command.toInt();
  
  digitalWrite(LED_PIN,state);
  return 1;
}

