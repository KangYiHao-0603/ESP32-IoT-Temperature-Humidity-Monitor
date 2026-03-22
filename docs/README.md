# Documentation

## Hardware Setup

![Hardware Setup](setup.jpg)

- ESP32
- DHT11 sensor
- LED

---

## System Architecture

---

## Test Results

### Serial Monitor
![Serial Output](serial.png)

### Web Interface
![Web Interface](web.png)

---

## Observations

- Temperature updates every 3 seconds
- LED turns ON when temperature exceeds threshold in AUTO mode
- Manual mode overrides automatic control

---

## Issues and Debug

- Wi-Fi connection requires 2.4 GHz network
- DHT11 reading may fail if read too frequently
