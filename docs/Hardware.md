<div align="center">

# 🍒💣 Ch3rryB0mb Hardware
### Wireless Networking & RF Learning Platform for the ESP32 CYD (Cheap Yellow Display)

</div>

## Hardware You'll Need

Ch3rryB0mb is designed around the ESP32 Cheap Yellow Display (CYD) and a collection of inexpensive, widely available radio modules. The goal is to build an affordable handheld wireless networking laboratory using off-the-shelf hardware.

> **Note:** Hardware support is a work in progress and will expand over time.

## MVP Acces Point Only mode

| Status | Hardware                        | Purpose                                                                         |
| :----: | ------------------------------- | ------------------------------------------------------------------------------- |
|  ☐ | **ESP32 CYD (ESP32-2432S028R)** | Main controller with touchscreen, Wi-Fi and Bluetooth.                          |
|  ☐ | **MicroSD Card Reader**         | Storage                                  |

## Networking and RF Modules

| Status | Hardware                        | Purpose                                                                         |
| :----: | ------------------------------- | ------------------------------------------------------------------------------- |
|  ☐ | **NRF24L01+ PA/LNA**            | 2.4 GHz transceiver for custom wireless communication and spectrum exploration. |
|  ☐ | **CC1101**                      | Sub-GHz transceiver supporting 315 / 433 / 868 / 915 MHz experimentation.       |
|  ☐ | **SX1262 LoRa Module**          | Long-range LoRa communication and telemetry experiments.                        |
|  ☐ | **NEO-M8N GPS Module**          | GPS positioning, wardriving and RF mapping.                                     |

# Ch3rryB0mb Wiring Diagrams

This document describes the hardware used by Ch3rryB0mb and how
external modules are connected to the ESP32-2432S028R (CYD).

> [!WARNING]
> Ch3rryB0mb is still under development.
> Pin assignments and hardware connections may change.

## ESP32 CYD

Main controller:

- ESP32-2432S028R
- ESP32
- 2.8" display
- Resistive touchscreen

---

## MicroSD Card Reader

### Module

3.3 V SPI MicroSD / TF card reader.

### Module pins

| SD Module | Function | CYD Pin |
|-----------|----------|---------|
| 3V3       | Power    | TBD     |
| CS        | Chip Select | TBD  |
| MOSI      | SPI Data Out | TBD |
| CLK       | SPI Clock | TBD   |
| MISO      | SPI Data In | TBD |
| GND       | Ground   | TBD     |

> CYD pin assignments will be added after hardware verification.

### Connection

The SD reader will not be permanently soldered directly to the CYD.

The planned connection is:

CYD → Perfboard → JST-XH connector → SD reader

This makes the SD reader removable and allows the wiring to be
replaced or modified without soldering directly to the CYD.

### Connector

Planned connector:

- JST-XH
- 2.54 mm pitch
- 6 pins

Pinout: TBD

### Wiring diagram

Coming soon.
