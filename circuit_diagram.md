# ESP-NOW Master-Slave Doorbell Intercom System - Simple Wiring Guide

## Overview
This simplified system uses basic components for reliable wireless communication. TX (Master) sends voice and receives chime commands. RX (Slave) plays voice and sends chime triggers.

## Hardware Components Required

### TX Unit (Master) - Voice Transmitter
- ESP32 DevKitC or compatible board
- Simple analog electret microphone (3-pin)
- 8Ω speaker (for chime tones)
- 1x Push button (momentary)
- 1x LED with 220Ω resistor
- Breadboard and jumper wires

### RX Unit (Slave) - Voice Receiver
- ESP32 DevKitC or compatible board
- 8Ω speaker (for voice playback)
- 1x Push button (momentary)
- 1x LED with 220Ω resistor
- Breadboard and jumper wires

### Power Supply (Both Units)
- 5V USB power adapter (1A minimum)
- USB cable for programming/power

## TX Unit (Master) - Simple Wiring

### Components & Connections
```
ESP32 GPIO 5 ───┬── Push Button ─── GND
                └── (TALK_BUTTON - hold to send voice)

ESP32 GPIO 34 ───┬── Microphone OUT
ESP32 3.3V ──────┼── Microphone VCC
GND ─────────────┼── Microphone GND

ESP32 GPIO 14 ───┬── Speaker +
GND ─────────────┴── Speaker -  (for chime tones)

ESP32 GPIO 2 ───┬── 220Ω Resistor ─── LED +
GND ─────────────┴── LED -  (status indicator)
```

### LED States
- **OFF**: Idle
- **ON**: Voice transmission active
- **ON**: Chime playing (takes priority)

## RX Unit (Slave) - Simple Wiring

### Components & Connections
```
ESP32 GPIO 4 ───┬── Push Button ─── GND
                └── (DOORBELL_BUTTON - press to trigger chime on TX)

ESP32 GPIO 14 ───┬── Speaker +
GND ─────────────┴── Speaker -  (for voice playback)

ESP32 GPIO 12 ─── I2S BCK (Bit Clock)
ESP32 GPIO 13 ─── I2S WS (Word Select)

ESP32 GPIO 2 ───┬── 220Ω Resistor ─── LED +
GND ─────────────┴── LED -  (status indicator)
```

### LED States
- **OFF**: Idle
- **ON**: Voice playback active
- **ON**: Chime send active (brief)

## Testing Your Setup

1. **Power Test**: Connect power - LED should light up briefly then turn off (ready state)
2. **TX Test**: Hold talk button - LED should turn solid, release - LED off
3. **RX Test**: Press doorbell button - LED should flash briefly
4. **Communication Test**: With both units powered, hold TX talk button - RX should play audio and LED on
5. **Chime Test**: Press RX doorbell button - TX should play chime tones and LED on

## Troubleshooting

- **No LED response**: Check button wiring and resistor
- **No audio on RX**: Verify I2S pins (12,13,14) and speaker connection
- **No chime on TX**: Check speaker wiring to GPIO 14
- **Communication fails**: Verify MAC addresses in code match your ESP32 boards

The system uses basic analog microphone and direct speaker drive - no fancy modules needed!
