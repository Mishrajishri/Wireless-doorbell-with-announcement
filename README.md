# ESP-NOW Master-Slave Doorbell Intercom System

This project implements a simplified wireless master-slave intercom system using two ESP32 boards. TX (Master) controls and sends voice. RX (Slave) receives voice and triggers chimes on the master.

## Features

*   **Master-Slave Architecture:** TX sends voice, RX triggers chimes and plays received audio
*   **Priority System:** Chime interrupts voice transmission for clear alerts
*   **LED Status Indicators:** Visual feedback for active states
*   **Simple Components:** Uses basic analog microphone and speakers (no fancy modules)
*   **Optimized Size:** Reduced from 1.7MB to ~600KB flash usage
*   **Production-Ready Reliability:** Non-blocking ISRs, mutex protection, error handling
*   **FreeRTOS Implementation:** Dedicated tasks for chime playing and audio playback

## Hardware Requirements

### TX Unit (Master):
- ESP32 DevKitC board
- Simple 3-pin electret microphone
- 8Ω speaker (for chime tones)
- 1x momentary push button
- 1x LED with 220Ω resistor

### RX Unit (Slave):
- ESP32 DevKitC board
- 8Ω speaker (for voice playback)
- 1x momentary push button
- 1x LED with 220Ω resistor

### Both Units:
- Breadboard and jumper wires
- 5V USB power supply
- USB cable for programming

## Software Requirements

*   Arduino IDE with ESP32 Board Support Package
*   Libraries:
    *   `WiFi.h` (comes with ESP32 BSP)
    *   `esp_now.h` (comes with ESP32 BSP)
    *   `driver/i2s.h` (comes with ESP32 BSP)

## Circuit Diagram

**Note:** Simple wiring diagrams are provided in `circuit_diagram.md`. The system uses basic analog microphone and direct speaker drive - no fancy modules needed!

## Setup Instructions

1. **Install Arduino IDE** with ESP32 board support

2. **Wire the circuits** according to `circuit_diagram.md`

3. **Update MAC addresses** in both `rx.ino` and `tx.ino`:
   - Find your ESP32 MAC addresses using a simple WiFi scan sketch
   - Replace the hardcoded addresses in the code

4. **Upload code**:
   - Upload `tx.ino` to the TX (Master) ESP32
   - Upload `rx.ino` to the RX (Slave) ESP32

## Usage

- **TX Unit:** Hold the button to transmit voice to RX
- **RX Unit:** Press the button to trigger chime on TX
- **LED Indicators:**
  - TX: ON during voice transmission or chime playback
  - RX: ON during audio playback or chime sending

## Operation

1. Power on both units (LEDs light briefly during startup)
2. TX button held = voice transmits to RX speaker
3. RX button pressed = chime plays on TX speaker (interrupts voice if active)
4. System uses ESP-NOW for reliable wireless communication

The system is now optimized and simplified - no SPIFFS, no MP3 files, no fancy audio libraries!
