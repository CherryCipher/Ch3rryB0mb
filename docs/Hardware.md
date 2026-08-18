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

## Networking and RF Modules

| Status | Hardware                        | Purpose                                                                         |
| :----: | ------------------------------- | ------------------------------------------------------------------------------- |
|  ☐ | **NRF24L01+ PA/LNA**            | 2.4 GHz transceiver for custom wireless communication and spectrum exploration. |
|  ☐ | **NRF24 HW-200 module board**     | breakout borad with power regulations for the NRF module                         |
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

![Wiring Diagram](wiring/Ch3rryB0mb.svg?v=2)

## NRF24 Hardware Connection

The NRF24L01+ PA/LNA module is connected to Ch3rryB0mb through an HW-200 NRF24 adapter board.

The adapter board is powered from the CYD P5 connector, while the SPI signals are soldered directly to the ESP32-WROOM module pads.

> [!WARNING]
> The 5V connection is connected to the **HW-200 adapter board only**.
>
> Do **not** connect 5V directly to a bare NRF24L01+ module.  
> The HW-200 contains its own voltage regulator and provides the NRF24 module with the required 3.3V supply.

### Power

Power for the HW-200 adapter is taken from the CYD P5 connector.

| CYD connection | HW-200 | Function |
|---|---|---|
| P5 5V | VCC | Adapter power |
| P5 GND | GND | Ground |

### SPI Connections

The SPI signals are soldered directly to the ESP32-WROOM module pads.

These pins use the ESP32 hardware SPI bus shared with the microSD card.

| ESP32 GPIO | HW-200 | Function |
|---|---|---|
| GPIO 23 | MO | MOSI |
| GPIO 19 | MI | MISO |
| GPIO 18 | SCK | SPI clock |
| GPIO 22 | CSN | NRF24 chip select |
| GPIO 27 | CE | NRF24 chip enable |

The NRF24 `IRQ` pin is currently not connected.

### Complete Wiring Overview

```text
CYD / ESP32                         HW-200 NRF24 adapter
────────────────────────────────────────────────────────

P5 5V       ──────────────────────> VCC/VIN
P5 GND      ──────────────────────> GND

GPIO 23     ──────────────────────> MO
GPIO 19     <────────────────────── MI
GPIO 18     ──────────────────────> SCK
GPIO 22     ──────────────────────> CSN
GPIO 27     ──────────────────────> CE

                                      IRQ   Not connected
```

### Shared SPI Bus

The NRF24 and microSD card share the ESP32 hardware SPI bus:

```text
MOSI = GPIO 23
MISO = GPIO 19
SCK  = GPIO 18
```

Each device has its own chip-select signal:

```text
microSD CS = GPIO 5
NRF24 CSN  = GPIO 22
```

Because both peripherals share the same SPI bus, only the device whose chip-select line is active communicates on the bus.

### Physical Installation

The SPI connections are soldered directly to the corresponding ESP32-WROOM module pads on the CYD PCB.

The 5V and ground connections are taken from the P5 connector instead of from the ESP32 module.

<!-- Suggested reference photographs:

1. P5 5V and GND connections.
2. ESP32-WROOM SPI breakout soldering.
3. Complete wiring to the HW-200 adapter.
4. Installed NRF24L01+ PA/LNA module and antenna.

 Add hardware photographs here -->
