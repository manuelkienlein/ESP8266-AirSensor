#include <ESP8266WiFi.h>
#include <DHT.h>
#include "MQ135.h"

// Thingspeak and WiFi
String apiKey = "apiKey";
const char* wifi_ssid = "WIFI-SSID";
const char* wifi_password = "WIFI-Password";

// Variables and Constants
#define PIN_DHT 3 // Pin for DHT11
#define PIN_MQ 0  // Pin for MQ-135
const char* server = "api.thingspeak.com";

// Initialisation
DHT dht(DHTPIN, DHT11);
MQ135 gasSensor = MQ135(PIN_MQ);
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

  float rzero = gasSensor.getRZero();
  float ppm = gasSensor.getPPM();

  // Unused at the moment
  float rzero_corrected = gasSensor.getCorrectedRZero(temperature, humidity);
  float ppm_corrected = gasSensor.getCorrectedPPM(temperature, humidity);
  float resistance = gasSensor.getResistance();
  float resistance_corrected = gasSensor.getCorrectedResistance(temperature, humidity);
  int sensorValue = analogRead(PIN_MQ);
  
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
    postStr +="&field3=";
    postStr += String(ppm);
    postStr +="&field4=";
    postStr += String(rzero);
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

  // Output for Serial Plotter and Serial Monitor
  
  //Serial.println("Temperature:"+String(t)+" "+"Humidity:"+String(h));
  Serial.println("Temperature:"+String(temperature)+" "+"Humidity:"+String(humidity)+" "+"rZero:"+String(rzero)+" "+"rZeroC:"+String(rzero_corrected)+" "+"PPM:"+String(ppm)+" "+"PPMC:"+String(ppm_corrected));
  //Serial.println("Temperature:"+String(temperature)+" "+"Humidity:"+String(humidity)+" "+"rZero:"+String(rzero)+" "+"rZeroC:"+String(rzero_corrected)+" "+"PPM:"+String(ppm)+" "+"PPMC:"+String(ppm_corrected)+" "+"Resistance:"+String(resistance)+" "+"ResistanceC:"+String(resistance_corrected));
  
  //delay(1000);
  
  // Wait 20 seconds because Thingspeak needs a minimum delay of 15 seconds between updates
  delay(20000);
}
