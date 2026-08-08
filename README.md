<div align="center">

# 🍒💣 Ch3rryB0mb

### Wireless Networking & RF Learning Platform for the ESP32 CYD (Cheap Yellow Display)

My first C++ and embedded-development project, built with PlatformIO.

<b>Full Disclaimer:</b> Ch3rryB0mb is partly vibe-coded using ChatGPT and Copilot.
<br> However every generated line is read, reviewed and understood === No code is pushed I don't fully understand.
<br> I want to learn, not just generate code...

[![Build](https://img.shields.io/badge/build-passing-brightgreen)](#)
[![Platform](https://img.shields.io/badge/platform-ESP32-E7352C)](https://www.espressif.com/)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979D)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/language-C++-00599C)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/status-early_development-orange)](#project-status)

</div>

---

## About Ch3rryB0mb

Ch3rryB0mb is an open-source educational firmware for the ESP32 platform that turns inexpensive hardware into a portable wireless networking laboratory.

Rather than focusing on offensive capabilities, C3B0 aims to help visualize and understand how wireless technologies work.

Wi-Fi, Bluetooth, LoRa, NRF24 and Sub-GHz communication are made visible, measurable and interactive.

Ch3rryB0mb is built to spark curiosity, not disruption. Every feature is designed to help people learn how wireless technology works through observation, experimentation and visualization.

The goal isn't to attack networks. The goal is to understand them.

> Ch3rryB0mb; Sweet but Explosive.

# 🍒💣 Planned Features

Ch3rryB0mb is built as a modular collection of educational apps. Each app is designed to help users explore, understand and experiment with wireless technologies in a safe and responsible way.

> **Note:** This is a wishlist and roadmap. Features will be checked off as they are implemented.

| Status | Feature                  | Description                                                                                                   |
| :----: | -------------------      | ------------------------------------------------------------------------------------------------------------- |
|    ☐   | **📡 WiFi Lab**         | Discover and inspect nearby Wi-Fi networks, visualize channel usage, interference and channel overlap, send and observe protocols such as ARP, ICMP, DNS, TCP and HTTP. Analyze signal strength, RSSI, SNR, channel congestion, roaming quality and security|
|    ☐   | **📡 Wardriving**       | Map Wi-Fi networks with GPS and export results to CSV, GPX, KML or GeoJSON.|
|    ☐   | **📡 AP Mode**          | Turn your Ch3rryB0mb into a configurable Wi-Fi access point. Experiment with channels, security, connected clients and RF statistics. |
|    ☐   | **📶 BLE Explorer**     | Discover nearby BLE devices and inspect advertisements, manufacturers, services and signal strength.          |
|    ☐   | **📶 BLE Fox Hunt**     | Locate Bluetooth devices using live signal strength, audio feedback and distance estimation.                  |
|    ☐   | **📻 NRF24 Lab**        | Visualize activity across the 2.4 GHz ISM band using NRF24 radio modules, Send, receive and inspect NRF24 packets. With Ch3rryN0des exchange encrypted messages between Ch3rryB0mb devices.                                     |
|    ☐   | **📡 LoRa Lab**         | Explore LoRa communication, experiment with frequency, spreading factor and bandwidth. Create secure long-range messaging between Ch3rryB0mb and Ch3rryN0de devices.               |
|    ☐   | **📻 CC1101 Lab**       | Visualize Sub-GHz radio activity, measure RSSI, modulation and channel occupancy. Experiment with your own Sub-GHz communication protocols and compare modulation settings.|
|    ☐   | **📍 GPS Dashboard**            | View live position, speed, altitude, satellites and positioning accuracy.                                     |
|    ☐   | **📍 RF Mapper**                | Combine GPS with Wi-Fi, BLE and RF scans to create wireless coverage maps.                                    |
|    ☐   | **📈 Live Spectrum**            | Monitor RF activity across all supported radio modules in real time.                                          |


# 🔧 Hardware

Ch3rryB0mb is designed around the ESP32 Cheap Yellow Display (CYD) and a collection of inexpensive, widely available radio modules. The goal is to build an affordable handheld wireless networking laboratory using off-the-shelf hardware.

> **Note:** Hardware support is a work in progress and will expand over time.

| Status | Hardware                        | Purpose                                                                         |
| :----: | ------------------------------- | ------------------------------------------------------------------------------- |
|  ☐ | **ESP32 CYD (ESP32-2432S028R)** | Main controller with touchscreen, Wi-Fi and Bluetooth.                          |
|  ☐ | **NRF24L01+ PA/LNA**            | 2.4 GHz transceiver for custom wireless communication and spectrum exploration. |
|  ☐ | **CC1101**                      | Sub-GHz transceiver supporting 315 / 433 / 868 / 915 MHz experimentation.       |
|  ☐ | **SX1262 LoRa Module**          | Long-range LoRa communication and telemetry experiments.                        |
|  ☐ | **NEO-M8N GPS Module**          | GPS positioning, wardriving and RF mapping.                                     |

## Building

This project uses [PlatformIO](https://platformio.org/).

Build the ESP32 firmware:

```powershell
pio run -e esp32dev
```

## Contributing

Ch3rryB0mb is currently a personal learning project, but feedback, suggestions and educational contributions are welcome.

## License

No specific license has been selected yet, but the project will be open source.
