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

| Status | Hardware | Purpose |
| :----: | --- | --- |
| V | **NRF24L01+ PA/LNA** | 2.4 GHz transceiver for custom wireless communication and spectrum exploration. |
| V | **NRF24 HW-200 Adapter Board** | OPTIONAL: adapter board with a voltage regulator allowing the NRF24 module to be powered from 5V. |
| ☐ | **CC1101** | Sub-GHz transceiver supporting 315 / 433 / 868 / 915 MHz experimentation. |
| ☐ | **SX1262 LoRa Module** | Long-range LoRa communication and telemetry experiments. |
| ☐ | **NEO-M8N GPS Module** | GPS positioning, wardriving and RF mapping. |

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

<!-- ![Wiring Diagram](wiring/Ch3rryB0mb.svg?v=2) -->

## NRF24 Hardware Connection

The NRF24L01+ PA/LNA module can be connected either directly to the CYD 3.3V supply or through an optional HW-200 NRF24 adapter board.

The SPI signals are soldered directly to the ESP32-WROOM module pads.

### Power Options

There are two supported power options.

#### Option 1: Direct 3.3V Connection

The NRF24L01+ can be powered directly from the ESP32 3.3V supply.

On the ESP32-WROOM module:

```text
Module pin 1 = GND
Module pin 2 = 3.3V
```

With the ESP32-WROOM antenna facing upward, these are the upper two pads on the left side of the module:

```text
Top left pad        = GND
Second left pad     = 3.3V
```

Connect:

| ESP32-WROOM | NRF24L01+ | Function |
|---|---|---|
| Pin 2 / 3.3V | VCC | 3.3V power |
| Pin 1 / GND | GND | Ground |

For improved power stability, especially when using an NRF24L01+ PA/LNA module for transmission, a **10–47 µF capacitor** may be added close to the NRF24 module between:

```text
ESP32 3.3V -----------+---- NRF VCC
                      |
                    + | 10–47 µF
                      |
ESP32 GND ------------+---- NRF GND
```

The capacitor is optional for initial testing and receive-only spectrum scanning, but can improve stability during higher-current transmit bursts.

> [!WARNING]
> Never connect 5V directly to the VCC pin of a bare NRF24L01+ module.

#### Option 2: HW-200 Adapter Board

The optional HW-200 adapter contains its own voltage regulator and allows the NRF24 module to be powered from the CYD 5V supply.

When using the HW-200 adapter, power can be taken from the CYD P5 connector.

| CYD connection | HW-200 | Function |
|---|---|---|
| P5 5V | VCC / VIN | Adapter power |
| P5 GND | GND | Ground |

> [!WARNING]
> The 5V connection must only be connected to the **HW-200 adapter board**.
>
> Do not connect P5 5V directly to a bare NRF24L01+ module.

### SPI Connections

The SPI signals are soldered directly to the ESP32-WROOM module pads.

These pins use the ESP32 hardware SPI bus shared with the microSD card.

| ESP32 GPIO | NRF24 / HW-200 | Function |
|---|---|---|
| GPIO 23 | MOSI / MO | MOSI |
| GPIO 19 | MISO / MI | MISO |
| GPIO 18 | SCK | SPI clock |
| GPIO 22 | CSN | NRF24 chip select |
| GPIO 27 | CE | NRF24 chip enable |

The NRF24 `IRQ` pin is currently not connected.

### Complete Wiring Overview

#### Direct 3.3V Connection

```text
CYD / ESP32                         NRF24L01+ PA/LNA
──────────────────────────────────────────────────────

3.3V        ──────────────────────> VCC
GND         ──────────────────────> GND

GPIO 23     ──────────────────────> MOSI
GPIO 19     <────────────────────── MISO
GPIO 18     ──────────────────────> SCK
GPIO 22     ──────────────────────> CSN
GPIO 27     ──────────────────────> CE

                                      IRQ   Not connected
```

#### HW-200 Adapter Connection

```text
CYD / ESP32                         HW-200 NRF24 adapter
────────────────────────────────────────────────────────

P5 5V       ──────────────────────> VCC / VIN
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

When using the direct 3.3V configuration, power and ground can also be taken directly from the ESP32-WROOM module.

When using the HW-200 adapter, 5V and ground can instead be taken from the CYD P5 connector.

<!--
Suggested reference photographs:

1. ESP32-WROOM SPI breakout soldering.
2. ESP32-WROOM 3.3V and GND connections.
3. HW-200 P5 power connection.
4. Complete wiring to the NRF24 module.
5. Installed NRF24L01+ PA/LNA module and antenna.
-->
