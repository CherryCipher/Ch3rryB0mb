<div align="center">

# 🍒💣 Ch3rryB0mb

### A modular firmware platform for the ESP32 CYD (Cheap Yellow Display)

My first C++ and embedded-development project, built with PlatformIO.

[![Build](https://img.shields.io/badge/build-passing-brightgreen)](#)
[![Platform](https://img.shields.io/badge/platform-ESP32-E7352C)](https://www.espressif.com/)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979D)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/language-C++-00599C)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/status-early_development-orange)](#project-status)

</div>

---

## About Ch3rryB0mb

Ch3rryB0mb is a modular firmware platform for the ESP32-based
CYD (Cheap Yellow Display).

It is my first C++ and embedded-development project and exists primarily
as a learning platform.

(most) Parts of the project are vibe-coded with the assistance of ChatGPT.
However, all code is read, reviewed and understood before it becomes
part of the project.

> In it to learn. Building it to understand it.

## Disclaimer

Ch3rryB0mb is educational firmware intended for learning about embedded
development, networking and the security of systems you own or are
explicitly authorized to test.

Do not use this software against networks, devices or systems without
permission. The author does not encourage or condone misuse.

Be curious. Be responsible.

## Features

Currently implemented:

- Modular input-event system
- Hardware-independent input interfaces
- Support for multiple input devices
- Native unit tests using Unity
- Separation between application logic and hardware

## Hardware

Hardware support has not yet been tested.
The initial target will be an ESP32 Cheap Yellow Display.

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
