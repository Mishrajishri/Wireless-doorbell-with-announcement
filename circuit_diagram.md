# ESP-NOW Doorbell Intercom System - Wiring Diagram

## Overview
This document provides a complete pin-by-pin wiring guide for both RX (Receiver) and TX (Transmitter) units. Each unit has identical hardware requirements and wiring.

## Hardware Components Required (per unit)

### Microcontroller
- ESP32 DevKitC or compatible board

### Audio Components
- Electret Microphone Module (with amplifier, e.g., MAX9814)
- 8Ω Speaker (0.5W minimum)
- Speaker Amplifier Module (optional, PAM8403 recommended for louder audio)

### Input Components
- 2x Push Button Switches (momentary, normally open)
- 1x LED (5mm, any color) with 220Ω current limiting resistor

### Power Supply
- 5V USB power adapter or battery pack

## Pin Assignments (Common for both RX and TX)

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

## Troubleshooting

- **No LED on startup**: Check power connections and GPIO 2 wiring
- **No chime on doorbell**: Check SPIFFS upload and speaker wiring
- **No audio transmission**: Check microphone wiring and talk button
- **ESP-NOW communication fails**: Verify MAC addresses in code match actual board addresses

This wiring guide ensures error-free assembly. Follow each step sequentially and use the checklist for verification.
