# ESP32-IoT-Temperature-Humidity-Monitor
Built an ESP32-based IoT monitoring system that reads temperature and humidity from a DHT11 sensor, hosts a web interface over Wi-Fi, and supports remote LED control with auto/manual mode switching.
## Features
- Read temperature and humidity data from DHT11
- Display sensor data on Serial Monitor
- Host a web page on ESP32 using Wi-Fi
- Show real-time temperature and humidity on the web page
- Remote LED ON/OFF control from a mobile phone browser
- Support AUTO / MANUAL mode switching
- Automatically turn on LED when temperature reaches the threshold in AUTO mode

## Hardware
- ESP32 development board
- DHT11 temperature and humidity sensor
- LED
- 220Ω resistor
- Breadboard
- Jumper wires

## Software / Technologies
- Arduino IDE
- ESP32 Wi-Fi
- WebServer library
- SimpleDHT library
- C++

## System Architecture
```text
DHT11 Sensor --> ESP32 --> Wi-Fi --> Mobile Browser
                    |
                    --> LED Control
```
## Pin Configuration
- In `src/main.ino.cpp` edit line #14 to use UART, I²C, or analog sensor input:
```
// INPUT MODES:
#define pinDHT11 = 15;
#define LED_PIN = 2;
```
DHT11 data pin: GPIO15

LED pin: GPIO2

## How to Run

1.Install Arduino IDE and ESP32 board package

2.Install required libraries:

WiFi.h

WebServer.h

SimpleDHT.h

3.Connect the hardware

4.pdate the Wi-Fi SSID and password in the code

5.Upload the code to ESP32

6.Open Serial Monitor and check the IP address

7.Open the IP address in a mobile phone browser connected to the same Wi-Fi network

## Demo
### Web Interface

![Web Interface](doc/Hardware_Setup.jpg)

### Hardware Setup

![Hardware](doc/Hardware_Setup.jpg)

### Serial Output

![Serial](doc/Serial_monitor.png)
Web Page

Real-time temperature and humidity display

Remote LED control buttons

AUTO / MANUAL mode buttons
