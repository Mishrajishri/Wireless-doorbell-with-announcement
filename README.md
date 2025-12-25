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
    *   `ESP32-AudioI2S` by Earle F. Philhower (for MP3 playback) - **NOT the generic "Audio" library**
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
    git clone https://github.com/Mishrajishri/Wireless-doorbell-with-announcement.git
    cd Wireless-doorbell-with-announcement
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

## Code Upload Instructions

### Using Arduino IDE:

1.  **Install ESP32 Board Support:**
    *   Open Arduino IDE
    *   Go to `File > Preferences`
    *   Add `https://dl.espressif.com/dl/package_esp32_index.json` to Additional Board Manager URLs
    *   Go to `Tools > Board > Boards Manager`
    *   Search for "esp32" and install "esp32 by Espressif Systems"

2.  **Install Required Libraries:**
    *   Go to `Sketch > Include Library > Manage Libraries`
    *   Search and install:
        - `ESP32-AudioI2S` by Earle F. Philhower (for Audio.h)
        - `ESP32 SPIFFS` (usually included with ESP32 BSP)

3.  **Configure Board Settings:**
    *   Go to `Tools > Board` and select your ESP32 board (e.g., "ESP32 Dev Module")
    *   Set the following:
        - **Upload Speed:** 921600
        - **Flash Frequency:** 80MHz
        - **Flash Mode:** QIO
        - **Partition Scheme:** Default 4MB with spiffs
        - **Core Debug Level:** None

4.  **Upload SPIFFS File:**
    *   Install the ESP32 Filesystem Uploader tool
    *   Place `doorbellsound.mp3` in a folder named `data` in your sketch directory
    *   Go to `Tools > ESP32 Sketch Data Upload`

5.  **Upload Code:**
    *   Open `rx.ino` in Arduino IDE
    *   Connect RX ESP32 to your computer via USB
    *   Select the correct port under `Tools > Port`
    *   Click the upload button (right arrow)
    *   Repeat for `tx.ino` with the TX ESP32

### Using PlatformIO:

1.  **Create Project:**
    ```bash
    mkdir esp-now-doorbell
    cd esp-now-doorbell
    platformio init --board esp32dev
    ```

2.  **Configure platformio.ini:**
    ```ini
    [env:esp32dev]
    platform = espressif32
    board = esp32dev
    framework = arduino
    lib_deps = 
        earlephilhower/ESP32-AudioI2S
    monitor_speed = 115200
    ```

3.  **Create Data Directory:**
    ```bash
    mkdir data
    cp doorbellsound.mp3 data/
    ```

4.  **Upload:**
    ```bash
    # Upload SPIFFS
    platformio run --target uploadfs
    
    # Upload code (for RX)
    platformio run --target upload --upload-port /dev/ttyUSB0
    
    # Upload code (for TX) - change port if needed
    platformio run --target upload --upload-port /dev/ttyUSB1
    ```

### Troubleshooting Upload Issues:

*   **Port Not Found:** Ensure USB cable supports data transfer, not just charging
*   **Upload Failed:** Try pressing the BOOT button while uploading
*   **Library Errors:**
    - If you get "Audio does not name a type" error, you have the wrong Audio library installed
    - Remove any generic "Audio" library (the SAM/Arduino Due one)
    - Install "ESP32-AudioI2S" by Earle F. Philhower from Library Manager
    - Restart Arduino IDE after library installation
*   **SPIFFS Upload Failed:** Check that `doorbellsound.mp3` is in the correct `data` folder
*   **Board Not Recognized:** Install correct USB drivers for your ESP32 board
*   **ESP-NOW Callback Errors:** The code has been updated for ESP32 Arduino library v3.x - if you still get callback errors, check your ESP32 Arduino library version

### MAC Address Configuration:

Before uploading, update the MAC addresses in the code:
*   In `rx.ino`: Replace `txMAC[]` with your TX ESP32's MAC address
*   In `tx.ino`: Replace `rxMAC[]` with your RX ESP32's MAC address

To find MAC addresses, upload a simple WiFi scan sketch or check the device label.

6.  **Power On:**
    *   Power both units. The STATUS_LED should light up indicating successful initialization.
    *   Press the DOORBELL_BUTTON on TX to test the chime.
    *   Hold TALK_BUTTON on either unit to test voice transmission.
