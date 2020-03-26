#include <ESP8266WiFi.h>
#include <DHT.h>

// Thingspeak and WiFi
String apiKey = "apiKey";
const char* wifi_ssid = "WIFI-SSID";
const char* wifi_password = "WIFI-Password";

// Variables and Constants
const char* server = "api.thingspeak.com";
#define DHTPIN 3 // Pin for DHT11

// Initialisation
DHT dht(DHTPIN, DHT11);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("Startup...");
  delay(10);
  dht.begin();
  
  WiFi.begin(wifi_ssid, wifi_password);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  
  WiFi.begin(wifi_ssid, wifi_password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read data from DHT sensor!");
    return;
  }

  if (client.connect(server,80)) { // "184.106.153.149" or "api.thingspeak.com"
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(temperature);
    postStr +="&field2=";
    postStr += String(humidity);
    postStr += "\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: "+String(server)+"\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
   }
  client.stop();
  
  Serial.println("Temperature:"+String(temperature)+" "+"Humidity:"+String(humidity));
  
  // Wait 20 seconds because Thingspeak needs a minimum delay of 15 seconds between updates
  delay(20000);
}
