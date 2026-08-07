<div align="center">

# 🍒💣 Ch3rryB0mb

### Wireless Networking & RF Learning Platform for the ESP32 CYD (Cheap Yellow Display)

My first C++ and embedded-development project, built with PlatformIO.

[![Build](https://img.shields.io/badge/build-passing-brightgreen)](#)
[![Platform](https://img.shields.io/badge/platform-ESP32-E7352C)](https://www.espressif.com/)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979D)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/language-C++-00599C)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/status-early_development-orange)](#project-status)

</div>

---

## About Ch3rryB0mb

Ch3rryB0mb is an open-source educational firmware for the ESP32 platform that turns inexpensive hardware into a portable wireless networking laboratory.

Rather than focusing on offensive capabilities, Ch3rryB0mb helps students, hobbyists and professionals explore, visualize and understand how modern wireless technologies actually work.

Wi-Fi, Bluetooth, LoRa, NRF24 and Sub-GHz communication become visible, measurable and interactive, allowing users to learn through experimentation on their own devices and networks.

Ch3rryB0mb is built to spark curiosity, not disruption. Every feature is designed to help people learn how wireless technology works through observation, experimentation and visualization.

The goal isn't to attack networks. The goal is to understand them.

> Ch3rryB0mb; Sweet but Explosive.

# 🍒💣 Planned Features

Ch3rryB0mb is built as a modular collection of educational apps. Each app is designed to help users explore, understand and experiment with wireless technologies in a safe and responsible way.

> **Note:** This is a wishlist and roadmap. Features will be checked off as they are implemented.

| Status | App                             | Description                                                                                                   |
| :----: | ------------------------------- | ------------------------------------------------------------------------------------------------------------- |
|    ☐   | **📡 WiFi Explorer**            | Discover and inspect nearby Wi-Fi networks, including security, channels, vendors and signal quality.         |
|    ☐   | **📡 WiFi Channel Analyzer**    | Visualize channel usage, interference and channel overlap.                                                    |
|    ☐   | **📡 WiFi Packet Lab**          | Learn networking by sending and observing protocols such as ARP, ICMP, DNS, TCP and HTTP on your own network. |
|    ☐   | **📡 WiFi Network Health**      | Analyze signal strength, channel congestion, roaming quality and security recommendations.                    |
|    ☐   | **📡 WiFi Wardrive**            | Map Wi-Fi networks with GPS and export results to CSV, GPX, KML or GeoJSON.                                   |
|    ☐   | **📶 BLE Explorer**             | Discover nearby BLE devices and inspect advertisements, manufacturers, services and signal strength.          |
|    ☐   | **📶 BLE Fox Hunt**             | Locate Bluetooth devices using live signal strength, audio feedback and distance estimation.                  |
|    ☐   | **📶 BLE Device Logger**        | Record and revisit discovered BLE devices over time.                                                          |
|    ☐   | **📻 NRF24 Spectrum Explorer**  | Visualize activity across the 2.4 GHz ISM band using NRF24 radio modules.                                     |
|    ☐   | **📻 NRF24 Packet Lab**         | Send, receive and inspect NRF24 packets while experimenting with payloads, acknowledgements and data rates.   |
|    ☐   | **📻 NRF24 Messenger**          | Exchange encrypted messages between Ch3rryB0mb devices.                                                       |
|    ☐   | **📡 LoRa Explorer**            | Explore LoRa communication while experimenting with frequency, spreading factor and bandwidth.                |
|    ☐   | **📡 LoRa Packet Lab**          | Build and analyze LoRa communication while learning about airtime, reliability and range.                     |
|    ☐   | **📡 LoRa Messenger**           | Create secure long-range messaging between Ch3rryB0mb devices.                                                |
|    ☐   | **📡 LoRa Telemetry**           | Transmit and visualize sensor data over long distances.                                                       |
|    ☐   | **📻 CC1101 Spectrum Explorer** | Visualize Sub-GHz radio activity across supported frequency ranges.                                           |
|    ☐   | **📻 CC1101 Signal Analyzer**   | Measure RSSI, modulation and channel occupancy.                                                               |
|    ☐   | **📻 CC1101 Packet Lab**        | Experiment with your own Sub-GHz communication protocols and compare modulation settings.                     |
|    ☐   | **📍 GPS Dashboard**            | View live position, speed, altitude, satellites and positioning accuracy.                                     |
|    ☐   | **📍 GPS Logger**               | Record routes and export GPX, CSV and KML tracks.                                                             |
|    ☐   | **📍 RF Mapper**                | Combine GPS with Wi-Fi, BLE and RF scans to create wireless coverage maps.                                    |
|    ☐   | **💳 RFID Explorer**            | Read and inspect supported RFID and NFC tags.                                                                 |
|    ☐   | **💳 RFID Writer**              | Write supported RFID and NFC tags for educational and laboratory use.                                         |
|    ☐   | **💳 Tag Creator**              | Create tags containing text, URLs, contact cards and Wi-Fi credentials.                                       |
|    ☐   | **💳 Tag Library**              | Store, organize and reuse frequently used tag templates.                                                      |
|    ☐   | **💳 Access Card Analyzer**     | Explore supported RFID cards to learn about memory layout, sectors and stored data.                           |
|    ☐   | **🔐 Secure Radio**             | Experiment with encrypted communication over NRF24 and LoRa.                                                  |
|    ☐   | **🌐 Protocol Visualizer**      | Watch network protocols move through the OSI model in real time.                                              |
|    ☐   | **🌐 Packet Timeline**          | Visualize complete communication flows from ARP and DNS to TCP, TLS and HTTP.                                 |
|    ☐   | **📈 Signal Recorder**          | Capture wireless measurements for later analysis.                                                             |
|    ☐   | **📈 Waterfall Display**        | Visualize radio activity over time.                                                                           |
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
|  ☐ | **MFRC522 RFID/NFC Reader**     | Read, write and explore compatible RFID and NFC tags.                           |
|  ☐ | **NEO-M8N GPS Module**          | GPS positioning, wardriving and RF mapping.                                     |

### Planned Hardware

The modular design allows additional hardware to be supported in the future, including:

* External battery management
* MicroSD storage
* I²C sensors
* Environmental sensors
* Additional RF modules
* External antennas
* Expansion boards

## Schematics and Wiring

tbd.

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
