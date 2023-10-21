# ESP8266-AirSensor
Using a ESP8266 to collect and analyze data about the air (temperature, humidity, etc.)

This project is about building an IoT device that collects information about the air.
The sensor data should then be sent to Thingspeak or an MQTT broker.
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
| Circuit Diagram | Breadboard Wiring |
|-----------------|-------------------|
| ![Circuit Diagram of DHT11-Thingspeak](/docs/DHT11-Thingspeak_circuitDiagram.png) | ![Breadboard visualisation of DHT11-Thingspeak](/docs/DHT11-Thingspeak_breadboard.png) |

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

### Wiring
| Circuit Diagram | Breadboard Wiring |
|-----------------|-------------------|
| ![Circuit Diagram of DHT11-MQ135-Thingspeak](/docs/DHT11-MQ135-Thingspeak_circuitDiagram.png) | ![Breadboard visualisation of DHT11-MQ135-Thingspeak](/docs/DHT11-MQ135-Thingspeak_breadboard.png) |

| DHT11 Pins | ESP8266 Pins | MQ135 Pins |
|------------|--------------|------------|
| GND        | G            | GND        |
| VCC        | 3V           |            |
| Signal     | RX (GPIO3)   |            |
|            | A0           | A0         |
|            | VU (5V)      | VCC        |

### Setup
1. At first, the sensor has to be calibrated. For this, connect the sensor to the circuit and leave it powered on for 24 hours to burn in.
2. Then put it into outside air for one hour at preferably at 20°C and 35% relative humidity. After that the calibration value should have settled.
Now we have to read out the calibration value: `rzero = gasSensor.getRZero();`
3. After that we replace the predefined RZero constant with our determined value in the file `MQ135.h` from the MQ135 sensor library: `#define RZERO 890`
(Note: Every sensor has a own specific RZero!)

## DHT11-MQ135-Thingspeak-Web
This sketch is a refactored version of the previous sketch [DHT11-MQ135-Thingspeak](#DHT11-MQ135-Thingspeak).
Additionally a webserver was included which provides the sensor data in JSON format.

### JSON Output
| URL                         | Example result                                                          |
|-----------------------------|-------------------------------------------------------------------------|
| `http://<IP-Address>/`      | `{"temperature":23.60, "humidity":31.00, "ppm":443.96, "rzero":979.75}` |
| `http://<IP-Address>/dht11` | `{"temperature":23.60, "humidity":31.00}`                               |
| `http://<IP-Address>/mq135` | `{"ppm":443.96, "rzero":979.75}`                                        |

## DHT11-MQ135-Thingspeak-Web-Display
In order to add a display to this setup, we too extend the previous installation [DHT11-MQ135-Thingspeak-Web](#DHT11-MQ135-Thingspeak-Web).
The source code can be found in [DHT11-MQ135-Thingspeak-Web-Display.ino](DHT11-MQ135-Thingspeak-Web-Display.ino) using address `0x27` for the display.

### Hardware Requirements
* NodeMCU ESP8266 Board
* 4.7k Ohm Resistor
* DHT11 Sensor
* MQ135 Sensor
* 16x2 LCD Display
* I2C LCD Interface Adapter

### Wiring
We do the same wiring like above plus some additional connections between the LCD-Display and the ESP8266 board.

| Display Pins | ESP8266 Pins |
|--------------|--------------|
| SCL          | D1           |
| SDA          | D2           |
| VCC          | VU           |
| GND          | G            |

![Breadboard visualisation of DHT11-MQ135-Thingspeak-Web-Display](/docs/DHT11-MQ135-Thingspeak-Web-Display_breadboard.png)

