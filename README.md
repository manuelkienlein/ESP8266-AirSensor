# ESP8266-AirSensor
Using a ESP8266 to collect and analyze data about the air (temperature, humidity, etc.)

## DHT11-Thingspeak
This sketch sends temperature and humidity data from the DHT11 sensor to Thingspeak.

### Requirements
* NodeMCU ESP8266
* DHT11

### Wiring
| DHT11 Pins | ESP8266 Pins |
| GND | G  |
| VCC | 3V |
| Signal | RX (GPIO3) |