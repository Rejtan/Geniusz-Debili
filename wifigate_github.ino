// Code made by Rejtan
// https://github.com/Rejtan/Geniusz-Debili
//
// http://yourespipaddress/garage opens/closes the garage door (connect esp8266 as a internal button to your gate opener)


#include <ESP8266WiFi.h>


//////////////////////////////////////////////////
const char* ssid = "examplename"; //your wifi name
const char* password = "examplepasswd"; //your wifi password
 //////////////////////////////////////////////////
int ledPin = 12; // GPIO12---D6 of NodeMCU or you can change it if you want


WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

///////////////////////////////////////
  
  IPAddress ip(192,168,55,222);   //Here you have to set your esp ip (because dynamic ip in this case would be a pain)
  IPAddress gateway(192,168,55,1);   //Remeber to change this
  IPAddress subnet(255,255,255,0);   //And this is almost all the time the same but check too
 
//////////////////////////////////////
  
  
  WiFi.config(ip, gateway, subnet);
 
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
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  
  if (request.indexOf("/garage") != -1)  {
    digitalWrite(ledPin, HIGH);             //This opens transistor for half a second which acts like internal garage button
    
    delay(500);
    digitalWrite(ledPin, LOW);
  }
  
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.println("<br><br>");
  client.println("<a href=\"/garage\"\"><button>Open/Close </button></a>"); 
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
