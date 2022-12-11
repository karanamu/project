#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
int _moisture,sensor_analog;
const int sensor_pin = A0;
 
const char* ssid     = "R Sridevi";
const char* password = "kaumudi03";
 
const char* host = "api.thingspeak.com";
const char* APIKey = "DGBYTNSHZYNQWQDW";
 
void setup()
{
    Serial.begin(115200);  
 
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
 
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
 

 
void loop()
{
     sensor_analog = analogRead(sensor_pin);
  _moisture = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
  Serial.print("Moisture = ");
  Serial.print(_moisture);  /* Print Temperature on the serial window */
  Serial.println("%");
  delay(1000); 
    Serial.print("connecting to ");
    Serial.println(host);
    
 
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }
 
    // We now create a URI for the request
    String url = "/update";
    url += "?api_key=";
    url += APIKey;
    url += "&field1=";
    
 
    Serial.print("Requesting URL: ");
    Serial.println(url);
 
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
     
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }
 
    Serial.println();
    Serial.println("closing connection");
}
