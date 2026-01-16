# ESP-NOW Doorbell Intercom System - Detailed Wiring Diagram

## Overview
This document provides comprehensive pin-by-pin wiring guides for both RX (Receiver) and TX (Transmitter) units. While both units use identical hardware and wiring, separate sections are provided for clarity. Follow the appropriate section based on which unit you're assembling.

## Hardware Components Required (per unit)

### Microcontroller
- ESP32 DevKitC V4 or compatible board (30-pin version recommended)
- Ensure your ESP32 board has exposed GPIO pins 12, 13, 14, 34

### Audio Components
- **Microphone:** MAX9814 Electret Microphone Amplifier Module (or similar with AGC)
  - Supply Voltage: 3.3V-5V
  - Output: Analog signal (0-3.3V range)
- **Speaker:** 8Ω, 0.5W minimum speaker
- **Speaker Amplifier:** PAM8403 3W Stereo Audio Amplifier Module (recommended for better volume)
  - Supply Voltage: 5V
  - Input: I2S or analog signal

### Input Components
- **Push Buttons:** 2x Momentary tactile switches (6mm x 6mm recommended)
  - Type: Normally Open (NO)
  - Rating: 12VDC, 50mA minimum
- **Status LED:** 5mm diameter LED (any color, 20mA forward current)
- **Current Limiting Resistor:** 220Ω, 0.25W for LED

### Power Supply
- 5V, 2A USB power adapter (USB-C or Micro-USB depending on your ESP32 board)
- Alternatively: 3.7V LiPo battery with 5V boost converter

### Additional Materials
- Breadboard (830 tie-points minimum)
- Male-to-male jumper wires (various lengths)
- Male-to-female jumper wires (for ESP32 connections)
- Wire strippers and electrical tape (optional)

## General Wiring Notes

- **Power Distribution:** Always connect power supplies to breadboard rails first
- **Ground Connections:** Use multiple ground connections to reduce noise
- **Wire Colors:** Use red for +5V/+3.3V, black for GND, other colors for signals
- **ESP32 Pin Constraints:**
  - GPIO 34 is input-only (no internal pull-up)
  - GPIO 12/13/14 are safe for I2S
  - Avoid using GPIO 6-11 (SPI flash pins)
- **Testing:** Test each connection before powering on
- **Heat Dissipation:** Speaker amplifier may get warm during operation

## RX Unit (Receiver) - Detailed Wiring Guide

### Purpose
The RX unit receives doorbell notifications and audio from the TX unit. It can also initiate intercom calls.

### Pin Assignments (RX Unit)

| ESP32 Pin | GPIO Number | Function | Connected To |
|-----------|-------------|----------|--------------|
| GPIO 4 | 4 | DOORBELL_BUTTON | Push Button 1 (Doorbell) |
| GPIO 5 | 5 | TALK_BUTTON | Push Button 2 (Talk/Intercom) |
| GPIO 34 | 34 | MIC_PIN | Microphone Amplifier Output |
| GPIO 14 | 14 | SPEAKER_PIN | Speaker Amplifier Input |
| GPIO 12 | 12 | I2S BCK | Speaker Amplifier BCK |
| GPIO 13 | 13 | I2S WS | Speaker Amplifier WS |
| GPIO 2 | 2 | STATUS_LED | LED (via 220Ω resistor) |
| GND | - | Ground | Common Ground |
| 5V/VIN | - | Power | 5V Supply |
| 3.3V | - | Power | Microphone/LED Power (if needed) |

## Detailed Wiring Instructions

### Step 1: Power Supply Connections
```
ESP32 5V Pin → Breadboard +5V Rail
ESP32 GND Pin → Breadboard GND Rail
```

### Step 2: Push Buttons (Doorbell and Talk)

#### Doorbell Button (GPIO 4)
```
ESP32 GPIO 4 → Button Terminal 1
Button Terminal 2 → Breadboard GND Rail
```
*Note: Uses internal INPUT_PULLUP, no external resistor needed*

#### Talk Button (GPIO 5)
```
ESP32 GPIO 5 → Button Terminal 1
Button Terminal 2 → Breadboard GND Rail
```
*Note: Uses internal INPUT_PULLUP, no external resistor needed*

### Step 3: Status LED (GPIO 2)
```
ESP32 GPIO 2 → 220Ω Resistor → LED Anode (+)
LED Cathode (-) → Breadboard GND Rail
```

### Step 4: Microphone (GPIO 34)
```
ESP32 GPIO 34 → Microphone Amplifier Output Pin
Microphone Amplifier VCC → Breadboard 3.3V Rail (or 5V if module requires)
Microphone Amplifier GND → Breadboard GND Rail
```

### Step 5: Speaker (GPIO 14, 12, 13)
```
ESP32 GPIO 14 → Speaker Amplifier Audio Input
ESP32 GPIO 12 → Speaker Amplifier BCK (Bit Clock)
ESP32 GPIO 13 → Speaker Amplifier WS (Word Select)
Speaker Amplifier GND → Breadboard GND Rail
Speaker Amplifier VCC → Breadboard 5V Rail
Speaker + Terminal → Speaker Amplifier Output
Speaker - Terminal → Breadboard GND Rail
```

## Breadboard Layout Suggestion

### Left Side (Power and Ground)
- Column 1-2: +5V Rail
- Column 3-4: GND Rail

### ESP32 Placement
- Place ESP32 across rows 10-20, columns 10-20

### Component Placement
- Doorbell Button: Row A, Columns 1-3
- Talk Button: Row C, Columns 1-3
- Status LED: Row E, Columns 1-3 (with resistor)
- Microphone Amp: Row G, Columns 1-5
- Speaker Amp: Row I, Columns 1-5

## Wiring Verification Checklist

- [ ] ESP32 powered (5V and GND connected)
- [ ] Both buttons connected to GPIO 4/5 and GND
- [ ] LED connected to GPIO 2 via 220Ω resistor to GND
- [ ] Microphone connected to GPIO 34, powered, and grounded
- [ ] Speaker amplifier connected to GPIO 14/12/13, powered, speaker attached
- [ ] All components share common GND
- [ ] No short circuits between power rails

## Testing Connections

1. Power on ESP32 - STATUS_LED should light up if initialization successful
2. Press doorbell button - should hear chime (if SPIFFS file uploaded)
3. Hold talk button - should transmit audio (test with paired unit)
4. Check serial output for any initialization errors

## Common Wiring Mistakes to Avoid

1. **Wrong GPIO pins**: Double-check pin numbers against code
2. **Missing grounds**: All components must share common GND
3. **Power polarity**: Ensure correct +5V/+3.3V connections
4. **LED without resistor**: Will burn out the LED immediately
5. **Microphone connections**: Amplifier output goes to GPIO 34, not microphone element directly
6. **Speaker connections**: Speaker + goes to amplifier output, not directly to ESP32

## TX Unit (Transmitter) - Detailed Wiring Guide

### Purpose
The TX unit sends doorbell notifications and audio to the RX unit. It can also receive intercom calls from the RX unit.

### Pin Assignments (TX Unit)

| ESP32 Pin | GPIO Number | Function | Connected To |
|-----------|-------------|----------|--------------|
| GPIO 4 | 4 | DOORBELL_BUTTON | Push Button 1 (Doorbell) |
| GPIO 5 | 5 | TALK_BUTTON | Push Button 2 (Talk/Intercom) |
| GPIO 34 | 34 | MIC_PIN | Microphone Amplifier Output |
| GPIO 14 | 14 | SPEAKER_PIN | Speaker Amplifier Input |
| GPIO 12 | 12 | I2S BCK | Speaker Amplifier BCK |
| GPIO 13 | 13 | I2S WS | Speaker Amplifier WS |
| GPIO 2 | 2 | STATUS_LED | LED (via 220Ω resistor) |
| GND | - | Ground | Common Ground |
| 5V/VIN | - | Power | 5V Supply |
| 3.3V | - | Power | Microphone/LED Power (if needed) |

### Wiring Instructions (TX Unit)
**Note:** TX unit wiring is identical to RX unit. Follow the same steps as above.

### Component-Specific Wiring Details

#### MAX9814 Microphone Module Pinout
```
Module Pins:
- VCC: 3.3V-5V input
- GND: Ground
- OUT: Analog output to ESP32 GPIO 34
- GAIN: Optional gain adjustment (leave floating for default)
- AR: Attack/Release time (leave floating for default)
```

#### PAM8403 Speaker Amplifier Pinout
```
Module Pins:
- VCC: 5V input
- GND: Ground
- LIN: Left audio input (connect to ESP32 GPIO 14)
- RIN: Right audio input (short to LIN for mono)
- LOUT+: Left speaker positive
- LOUT-: Left speaker negative
- ROUT+: Right speaker positive (short to LOUT+ for mono)
- ROUT-: Right speaker negative (short to LOUT- for mono)
- SD: Shutdown pin (connect to VCC to enable)
```

#### ESP32 DevKitC Pin Layout Reference
```
ESP32 DevKitC V4 Pinout (Top View):
                     USB
                 ┌─────────┐
          GND ───┤ 1     2 ├─── 3.3V
          GPIO 1 ─┤ 3     4 ├─── GPIO 3
          GPIO 5 ─┤ 5     6 ├─── GPIO 6
          GPIO 7 ─┤ 7     8 ├─── GPIO 8
          GPIO 9 ─┤ 9    10 ├─── GPIO 10
         GPIO 11 ─┤11    12 ├─── GPIO 12 ← I2S BCK
         GPIO 13 ─┤13    14 ├─── GPIO 14 ← SPEAKER_PIN
         GPIO 15 ─┤15    16 ├─── GPIO 16
         GPIO 17 ─┤17    18 ├─── GPIO 18
         GPIO 19 ─┤19    20 ├─── GPIO 21
         GPIO 22 ─┤21    22 ├─── GPIO 22
         GPIO 23 ─┤23    24 ├─── GPIO 23
         GPIO 25 ─┤25    26 ├─── GPIO 25
         GPIO 26 ─┤27    28 ├─── GPIO 26
         GPIO 27 ─┤29    30 ├─── GPIO 27
           GND ───┤31    32 ├─── GPIO 32
          GPIO 33 ─┤33    34 ├─── GPIO 33
          GPIO 34 ─┤35    36 ├─── GPIO 35  ← MIC_PIN (GPIO 34)
          GPIO 36 ─┤37    38 ├─── GPIO 39
            EN ───┤39    40 ├─── GPIO 2 ← STATUS_LED
           5V/VIN ┼─────────┼
                 └─────────┘
```

### Advanced Wiring Options

#### Alternative Speaker Connection (Without Amplifier)
If you don't have a speaker amplifier, connect speaker directly to ESP32:
```
ESP32 GPIO 14 → 10μF capacitor → Speaker +
Speaker - → GND
```
*Note: Volume will be very low, PAM8403 recommended*

#### External Antenna (For Better Range)
ESP32 has onboard antenna, but for extended range:
```
ESP32 Antenna Pin → U.FL to RP-SMA adapter → External antenna
```

#### Battery Power Option
For portable use:
```
3.7V LiPo Battery → TP4056 charging module → AMS1117-5.0 regulator → 5V rail
```

## Troubleshooting

- **No LED on startup**: Check power connections and GPIO 2 wiring
- **No chime on doorbell**: Check SPIFFS upload and speaker wiring
- **No audio transmission**: Check microphone wiring and talk button
- **ESP-NOW communication fails**: Verify MAC addresses in code match actual board addresses
- **Poor audio quality**: Check microphone amplifier gain and speaker connections
- **System crashes**: Check for short circuits and adequate power supply current

## Final Testing Procedure

1. **Power Test**: Connect power - LED should light steadily
2. **Button Test**: Press buttons - check serial output shows button presses
3. **Audio Test**: Play chime - speaker should produce sound
4. **Communication Test**: Pair with other unit and test doorbell/audio
5. **Range Test**: Test maximum reliable distance between units

This comprehensive wiring guide ensures successful assembly for both RX and TX units. The detailed specifications and troubleshooting steps help prevent common issues.
