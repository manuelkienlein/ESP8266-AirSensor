# ESP8266-AirSensor
Using a ESP8266 to collect and analyze data about the air (temperature, humidity, etc.)

This project is about building an IoT device that collects information about the air.
The sensor data should then be sent to Thingspark or an MQTT broker.
Data on temperature, humidity and air quality in your room can be viewed online.

In addition, I would like to integrate a small web server that provides the sensor data via a REST API.

In the future I might want to add a display and set up a notification system that will send you a notification when there is bad air in your room.

## DHT11-Thingspeak
This sketch sends temperature and humidity data from the DHT11 sensor to Thingspeak.

### Hardware Requirements
* NodeMCU ESP8266 Board
* 4.7k Ohm Resistor
* DHT11 Sensor

### Wiring
| DHT11 Pins | ESP8266 Pins |
|------------|--------------|
| GND        | G            |
| VCC        | 3V           |
| Signal     | RX (GPIO3)   |

## DHT11-MQ135-Thingspeak
This sketch sends data from DHT11 and MQ135 to Thingspeak.

### Hardware Requirements
* NodeMCU ESP8266 Board
* 4.7k Ohm Resistor
* DHT11 Sensor
* MQ135 Sensor

### Software Libraries
MQ135 by GeorgK: https://github.com/GeorgK/MQ135
