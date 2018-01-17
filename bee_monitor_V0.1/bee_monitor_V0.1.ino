
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h> 

#define DHTtype DHT11
DHT dht(2, DHTtype, 11);

const char* ssid = "Fletcher_2.4G";
const char* password = "Griffendog16"; 

float temp,humidity;

WiFiClient client;

const int channelID = 392543;
String APIkey = "1UL61NXLNNC4J9DW";
const char* server = "api.thingspeak.com";
const int sleepDuration = 4; //Deep Sleep Duration in minutes

void setup() {
 
 Serial.begin(115200); 
 Serial.println("Serial Test"); //Test Serial Output
 dht.begin(); 
 WiFi.begin(ssid, password);
 Serial.print("\n\r \n\rWorking to connect");

 while (WiFi.status() != WL_CONNECTED) { //Wait for connectiom
    delay(500);
    Serial.print(".");
  }
  
  //Serial Output 
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

   if (client.connect(server, 80)) {
   readDHT();//collect humidity and temperature data

    // Construct API request body
    String body = "field1=";
           body += String(temp);
           body += "&field2=";
           body += String(humidity);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + APIkey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(body.length());
    client.print("\n\n");
    client.print(body);
    client.print("\n\n");  
    
    }
  client.stop();
  ESP.deepSleep(sleepDuration*60*1000000);//Enter deep sleep, code executes from beginning on boot
}


void loop() {                
  
}

void readDHT(){

    humidity = dht.readHumidity();   // Read humidity (percent)
    temp = dht.readTemperature(0);   // Read temperature as Fahrenheit
                                     // Check if any reads failed and exit early (to try again).                 
    if (isnan(humidity) || isnan(temp)) {
      Serial.println("Failed to read from DHT sensor");
      readDHT(); 
      }
}
