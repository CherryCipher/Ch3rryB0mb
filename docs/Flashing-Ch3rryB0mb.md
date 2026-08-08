# 🍒 Flashing Ch3rryB0mb

This guide describes how to build and flash the **Ch3rryB0mb** firmware to a supported ESP32 CYD board using PlatformIO.

> [!WARNING]
> Ch3rryB0mb is currently under development.
> Hardware revisions of CYD boards may differ.
>
> Always verify your exact board revision before connecting external hardware.

---

## Requirements

### Hardware

- ESP32 CYD (`ESP32-2432S028R`)
- USB data cable
- Computer running Windows, Linux, or macOS
- FAT32 formatted MicroSD card (OPTIONAL)
- MicroSD card reader for your computer (OPTIONAL)

For the first firmware flash, **do not connect any external Ch3rryB0mb modules yet**.
Test the CYD by itself first.

### Software

Install:

- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO](https://platformio.org/)
- [Git](https://git-scm.com/)

---

## 1. Clone Ch3rryB0mb

Clone the repository:

```bash
git clone https://github.com/CherryCipher/Ch3rryB0mb.git
```

Open the cloned `Ch3rryB0mb` directory in Visual Studio Code.

---

## 2. Connect the CYD

Connect the CYD directly to your computer using a USB cable.

> [!NOTE]
> Make sure the USB cable supports **data**.
>
> Some USB cables are charge-only cables. If the CYD powers on but is not detected by your computer, try another cable before troubleshooting anything else.

For the initial test, the setup should be as simple as possible:

```text
Computer
   │
   │ USB
   ▼
┌─────────────┐
│     CYD     │
└─────────────┘
```

Do not connect the NRF24, CC1101, GPS, or other external modules yet.

---

## 3. Find the Serial Port

### Windows

Open:

**Device Manager → Ports (COM & LPT)**

Connect and disconnect the CYD and check which COM port appears.

For example:

```text
USB Serial Device (COM5)
```

Your COM port may be different.

### PlatformIO

You can also list available serial devices using:

```bash
pio device list
```

---

## 4. Build the Firmware

Before flashing anything, verify that the firmware compiles successfully.

From the Ch3rryB0mb repository:

```bash
pio run
```

A successful build should end with something similar to:

```text
========================= [SUCCESS] =========================
```

> [!TIP]
> If PowerShell reports that `pio` is not recognized after installing PlatformIO, restart Visual Studio Code and open a new terminal after verifying the plugin is installed.

---

## 5. Prepare the MicroSD Card (OPTIONAL)

Ch3rryB0mb can serve its web interface from the built-in MicroSD card.

This step is **optional**. If no MicroSD card is present, or the required
web files cannot be loaded, Ch3rryB0mb will use its built-in fallback
page instead.

To use the full web interface:

1. Format a MicroSD card as FAT32.
2. Copy the `web` folder from the `data/web` directory from the Ch3rryB0mb repository to the root
   of the MicroSD card.

The resulting structure should look like:

```text
MicroSD/
└── web/
    ├── index.html
    └── css/
        └── style.css
```

3. Safely eject the MicroSD card from your computer and insert it into the CYD.

> [!IMPORTANT]
> Copy the only, and the full `web` directory itself to the MicroSD card. Not the `data` folder.


---

## 6. Flash Ch3rryB0mb

With the CYD connected:

```bash
pio run -t upload
```

PlatformIO should detect the serial port automatically.

If multiple serial devices are connected, specify the port manually:

```bash
pio run -t upload --upload-port COM5
```

Replace `COM5` with the port assigned to your CYD (see step 3).

---

## 7. Bootloader Mode

Normally the ESP32 should automatically enter the correct mode when uploading firmware.

If the upload fails, manual bootloader mode may be required.

A typical ESP32 procedure is:

1. Hold the **BOOT** button.
2. Press and release **RESET**.
3. Release **BOOT**.
4. Start the upload again.

> [!IMPORTANT]
> The exact procedure for the supported CYD hardware will be verified using physical hardware.
>
> **Status: TBD**

---

## 8. Monitor the First Boot

After flashing, open the serial monitor:

```bash
pio device monitor
```

The baud rate is configured in `platformio.ini`.

The firmware should produce startup information over the serial connection.

Example:

```text
=========== ENHANCE ============
Application started successfully.
Welcome to Ch3rryB0mb!
=================================
```

The exact startup sequence will be documented after testing on physical hardware.

Press:

```text
Ctrl + C
```

to exit the serial monitor.

---

# First Hardware Test

Do not connect all Ch3rryB0mb hardware immediately.

Test the system one component at a time.

Recommended order:

1. ESP32 boots
2. Serial communication works
3. Display works
4. Touch input works
5. Built-in MicroSD reader works
6. Wi-Fi works
7. Bluetooth works
8. External modules

This makes hardware and firmware problems much easier to isolate.

---

# Troubleshooting

## `pio` is not recognized

If PowerShell displays:

```text
pio : The term 'pio' is not recognized...
```

Restart Visual Studio Code after installing PlatformIO.

Then open a new terminal and try:

```bash
pio run
```

---

## CYD does not appear as a serial device

Check the following:

- Make sure the USB cable supports data.
- Try another USB cable.
- Try another USB port.
- Check Device Manager.
- Disconnect other USB serial devices.
- Check whether your CYD requires a USB-to-serial driver.

---

## Firmware builds but upload fails

Check:

- The correct COM port is selected (otherwise set it manualy see step 3 & 6).
- No other program is using the serial port.
- The USB cable you are using supports data.
- The CYD is receiving power (check possible status LEDs).
- Manual bootloader mode is required (see step 6).

You can explicitly select the port:

```bash
pio run -t upload --upload-port COM5
```

---

## Firmware uploads but does not start

Open the serial monitor:

```bash
pio device monitor
```
Check the ESP32 boot messages for errors.

If you see:

```text
========== FATAL ERROR ==========
Application failed to start.
System halted.
=================================

System halted. FATAL ERROR: Application failed to start.
System halted. FATAL ERROR: Application failed to start.
System halted. FATAL ERROR: Application failed to start.
```
Verify no external modules are shorting.
Possibly remove any or all external modules and reflash the CYD.
Keep any external modules disconnected until the CYD itself boots correctly.

---

## Next Step

After the first successful flash, the c3B0 should boot in AP Mode and any wireless device should be able to connect
to the device using:

ssid = "CherryLab";
password = "ch3rryb0mb";

And load the captive portal at http://192.168.4.1

Then continue with the Ch3rryB0mb hardware setup and test each subsystem individually.

**Do not assume it works. Test it. Document it. Then build on it. 🍒**