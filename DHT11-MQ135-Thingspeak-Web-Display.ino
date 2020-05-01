#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "MQ135.h"

// Thingspeak and WiFi
String apiKey = "apiKey";
const char* wifi_ssid = "WIFI-SSID";
const char* wifi_password = "WIFI-Password";

// Variables and Constants
#define PIN_DHT 3 // Pin for DHT11
#define PIN_MQ 0  // Pin for MQ-135
#define LCD_COLUMNS 16
#define LCD_ROWS 2
const char* server = "api.thingspeak.com";
#define INTERVAL_THINGSPEAK 20 // Thingspeak delay

// Initialisation
DHT dht(PIN_DHT, DHT11);
MQ135 gasSensor = MQ135(PIN_MQ);
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);

WiFiClient client;
ESP8266WebServer webserver(80);

float humidity;
float temperature;
float rzero;
float ppm;
unsigned long previousMillis;
unsigned long currentMillis;


void setup() {
  Serial.begin(115200);
  Serial.println("Startup...");

  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); 
  lcd.print("Startup...");
  
  dht.begin();
  
  lcd.setCursor(0, 1);
  lcd.print("Connect WiFi");
  setupWiFi();
  lcd.clear();
  lcd.print("IP Address:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(10000);

  webserver.on("/", handleWebserverRequest_index);
  webserver.on("/dht11", handleWebserverRequest_dht11);
  webserver.on("/mq135", handleWebserverRequest_mq135);
  webserver.begin();

  int warmup = 30;
  lcd.clear();
  lcd.print(" IoT Air-Sensor ");
  for(int i = warmup; i > 0; i--){
    lcd.setCursor(0, 1);
    lcd.print("Warming up: "+String(i)+"s ");
    delay(1000);
  }

  lcd.clear();
  delay(1000);
  
}

void loop() {
  //Refresh sensor values
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  rzero = gasSensor.getRZero();
  ppm = gasSensor.getPPM();

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

  // Wait 20 seconds because Thingspeak needs a minimum delay of 15 seconds between updates
  currentMillis = millis();
  if(currentMillis - previousMillis >= INTERVAL_THINGSPEAK*1000){
    previousMillis = currentMillis;
    sendToThingspeak();
  }

  // Handle webserver requests
  webserver.handleClient();

  // Update display
  updateDisplay();
  
  //Serial.println("Temperature:"+String(t)+" "+"Humidity:"+String(h));
  Serial.println("Temperature:"+String(temperature)+" "+"Humidity:"+String(humidity)+" "+"rZero:"+String(rzero)+" "+"rZeroC:"+String(rzero_corrected)+" "+"PPM:"+String(ppm)+" "+"PPMC:"+String(ppm_corrected));
  //Serial.println("Temperature:"+String(temperature)+" "+"Humidity:"+String(humidity)+" "+"rZero:"+String(rzero)+" "+"rZeroC:"+String(rzero_corrected)+" "+"PPM:"+String(ppm)+" "+"PPMC:"+String(ppm_corrected)+" "+"Resistance:"+String(resistance)+" "+"ResistanceC:"+String(resistance_corrected));
  
  delay(1000);
}

void setupWiFi(){
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

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendToThingspeak(){
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
}

void handleWebserverRequest_index(){
  String message = "{\"temperature\":"+String(temperature)+", \"humidity\":"+String(humidity)+", \"ppm\":"+String(ppm)+", \"rzero\":"+String(rzero)+"}";
  webserver.send(200, "application/json", message);
}

void handleWebserverRequest_dht11(){
  String message = "{\"temperature\":"+String(temperature)+", \"humidity\":"+String(humidity)+"}";
  webserver.send(200, "application/json", message);
}

void handleWebserverRequest_mq135(){
  String message = "{\"ppm\":"+String(ppm)+", \"rzero\":"+String(rzero)+"}";
  webserver.send(200, "application/json", message);
}

void updateDisplay(){
  // Print first line
  lcd.setCursor(0, 0); 
  lcd.print("T: "+String(int(temperature))+(char)223+"C | H: "+String(int(humidity))+"%");

  // Print second line
  lcd.setCursor(0, 1);
  lcd.print("PPM: "+String(int(ppm)));
}
