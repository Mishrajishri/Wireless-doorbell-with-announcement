# ESP-NOW Doorbell Intercom System

This project implements a wireless doorbell and two-way audio intercom system using two ESP32 development boards communicating via ESP-NOW. One ESP32 acts as the receiver (RX) and the other as the transmitter (TX).

## Features

*   **Wireless Doorbell:** Pressing a button on the TX unit triggers a chime on both TX and RX units, and sends a notification to the RX unit.
*   **Two-Way Audio Intercom:** Hold a button on either unit to transmit voice to the other unit, enabling two-way communication.
*   **16-bit Audio Quality:** Improved audio sampling and playback using 16-bit I2S.
*   **Button Debouncing:** Software debouncing implemented for reliable button presses.
*   **Status LED:** Visual feedback for initialization and critical errors.

## Hardware Requirements

For each ESP32 unit (one RX, one TX):

*   ESP32 Development Board (e.g., ESP32 DevKitC)
*   Electret Microphone Amplifier Module (e.g., MAX9814 or similar)
*   Small Speaker (8 Ohm, 0.5W or similar)
*   Two Push Buttons
*   Resistors for buttons (if not using internal pull-ups)
*   LED (for status indication, optional)
*   Breadboard and Jumper Wires
*   USB cables for programming

## Software Requirements

*   Arduino IDE or PlatformIO
*   ESP32 Board Support Package installed in your IDE.
*   Libraries:
    *   `WiFi.h` (comes with ESP32 BSP)
    *   `esp_now.h` (comes with ESP32 BSP)
    *   `SPIFFS.h` (comes with ESP32 BSP)
    *   `Audio.h` (e.g., by Earle F. Philhower, needed for MP3 playback)
    *   `driver/i2s.h` (comes with ESP32 BSP)

## Circuit Diagram (Conceptual)

**Note:** A detailed Fritzing or similar diagram will be provided in a separate file (e.g., `circuit_diagram.fzz` or `circuit_diagram.png`).

**Common Connections for both RX and TX:**

*   **ESP32 Power:** Connect 5V to VIN, GND to GND.
*   **Speaker:**
    *   Connect Speaker positive to `SPEAKER_PIN` (GPIO 14 in code, via I2S).
    *   Connect Speaker negative to GND.
*   **Microphone:**
    *   Connect Microphone audio output to `MIC_PIN` (GPIO 34 in code).
    *   Connect Microphone VCC to 3.3V.
    *   Connect Microphone GND to GND.
*   **DOORBELL_BUTTON:**
    *   One terminal to `DOORBELL_BUTTON` pin (GPIO 4 in code).
    *   Other terminal to GND (using internal `INPUT_PULLUP`).
*   **TALK_BUTTON:**
    *   One terminal to `TALK_BUTTON` pin (GPIO 5 in code).
    *   Other terminal to GND (using internal `INPUT_PULLUP`).
*   **STATUS_LED (Optional):**
    *   Longer leg (Anode) to `STATUS_LED` pin (GPIO 2 in code, with a current limiting resistor).
    *   Shorter leg (Cathode) to GND.

**I2S Connections:**

*   `bck_io_num`: GPIO 12
*   `ws_io_num`: GPIO 13
*   `data_out_num`: GPIO 14 (for speaker output)
*   `data_in_num`: GPIO 34 (`MIC_PIN` for microphone input)

## Setup and Usage

1.  **Clone the Repository:**
    ```bash
    git clone <your-repository-url>
    cd <your-repository-name>
    ```

2.  **Install Libraries:** Ensure all required libraries (especially `Audio.h`) are installed in your Arduino IDE or PlatformIO project.

3.  **Upload `doorbellsound.mp3` to SPIFFS:**
    *   The `doorbellsound.mp3` file needs to be uploaded to the SPIFFS filesystem of both ESP32 boards.
    *   Use the Arduino IDE ESP32 filesystem uploader tool or PlatformIO to upload the file to SPIFFS partition.

4.  **Update MAC Addresses:**
    *   Replace the hardcoded MAC addresses in `rx.ino` and `tx.ino` with the actual MAC addresses of your ESP32 boards.
    *   You can find the MAC address by running a simple WiFi scan sketch or checking the device label.

5.  **Upload Code:**
    *   Upload `rx.ino` to the RX ESP32 board.
    *   Upload `tx.ino` to the TX ESP32 board.

6.  **Power On:**
    *   Power both units. The STATUS_LED should light up indicating successful initialization.
    *   Press the DOORBELL_BUTTON on TX to test the chime.
    *   Hold TALK_BUTTON on either unit to test voice transmission.
