# iot-plant-monitor-ethernet-ethernet-wifi

## Project Description

This project implements a smart agricultural monitoring system for real-time plant health surveillance using embedded sensor networks and local web-based monitoring.

The system monitors multiple environmental and plant-health parameters including soil moisture, ambient temperature, humidity, and plant orientation to detect conditions such as irrigation requirement, excessive heat, and storm-induced tilt or uprooting.

Three sensors are interfaced with the embedded controller:

- ADXL345 accelerometer for plant tilt/orientation monitoring
- DHT11 temperature and humidity sensor
- Soil moisture sensor for irrigation assessment

Two communication architectures were developed:

### Ethernet-Based Version (Arduino Nano + ENC28J60)

The first implementation uses an Arduino Nano with an ENC28J60 Ethernet module to transmit sensor data over a wired local network.

Because the Arduino Nano has limited SRAM and storage, sensor processing was optimized to enable lightweight real-time HTTP data serving to a locally hosted webpage on a connected laptop.

This implementation demonstrates:

- SPI-based Ethernet communication
- Embedded HTTP server implementation
- Resource-constrained firmware optimization
- Real-time local dashboard monitoring

### Wi-Fi-Based Version (ESP32)

A second implementation was developed using ESP32 with built-in Wi-Fi connectivity.

The ESP32 hosts an HTTP server over the local wireless network, allowing any browser on the same network to access real-time sensor data.

### Alert Conditions

Threshold-based monitoring logic includes:

- Dry Soil Alert:
  Triggered when soil moisture exceeds threshold → LED ON

- High Temperature Alert:
  Triggered when temperature > 35°C → buzzer activated

- Storm/Uprooting Detection:
  Triggered when accelerometer detects abnormal tilt/orientation → buzzer activated

### Real-Time Dashboard

The embedded web interface displays:

- Temperature
- Humidity
- Soil moisture status
- Plant orientation values
- Alert status

### Engineering Concepts Demonstrated

This project demonstrates:

- Embedded C / Arduino firmware development
- Sensor interfacing
- ADC data acquisition
- I2C communication
- SPI Ethernet communication
- HTTP protocol implementation
- TCP/IP networking
- IoT dashboard development
- Resource-constrained embedded optimization
- Protocol migration (Ethernet → Wi-Fi)

### Applications

- Precision agriculture
- Smart irrigation systems
- Greenhouse monitoring
- Crop health monitoring
- Remote environmental sensing
