🐦 ESP32 Flappy Bird — Ultrasonic Controlled Game

A DIY **Flappy Bird game** built using an **ESP32 DevKit, HC-SR04 ultrasonic sensor and SSD1306 128×64 OLED display**.

The game uses hand distance as the controller. Move your hand above the ultrasonic sensor to control the bird's vertical position and avoid the pipes.

This project is designed for students, embedded-systems learners and makers who want to learn **ESP32 programming, ultrasonic sensing, OLED graphics, game logic and PlatformIO development**.

---

## 📸 Project Hardware

The prototype uses an ESP32 DevKit, SSD1306 OLED display and HC-SR04 ultrasonic sensor.

The HC-SR04 faces upward so that hand distance can be used as the game controller.

![ESP32 Flappy Bird Hardware](images/flappy_bird_hardware.jpg)

---

# 🔍 Project Overview

The ESP32 runs a lightweight Flappy Bird-style game on a **128×64 SSD1306 OLED**.

Instead of using physical buttons, the player's hand controls the bird through the ultrasonic sensor.

### Game Control

- **Hand far from sensor → Bird moves UP**
- **Hand close to sensor → Bird moves DOWN**

The ESP32 continuously measures the hand distance, smooths the sensor readings and converts the distance into the bird's vertical position.

### Game Features

- 🐦 Real-time bird movement
- 🚧 Moving pipes
- 🏆 Score counter
- ⭐ Best-score storage
- 📈 Increasing game speed
- 💾 Best score saved using ESP32 Preferences
- 🎮 Ultrasonic hand control
- 🖥️ 128×64 OLED graphics

---

# ⚙️ Basic Working Principle

```text
        Hand
         ↓
   ┌─────────────┐
   │   HC-SR04   │
   │   Ultrasonic│
   │   Sensor    │
   └──────┬──────┘
          │ Distance
          ▼
   ┌─────────────┐
   │    ESP32    │
   │ ADC / Logic │
   │ Game Engine │
   └──────┬──────┘
          │
          ▼
   ┌─────────────┐
   │  SSD1306    │
   │ OLED 128x64 │
   └─────────────┘
```

The sensor measures the distance to the player's hand.

```text
Far hand
   ↓
Bird moves UP

Near hand
   ↓
Bird moves DOWN
```

---

# ✨ Features

* 🐦 Flappy Bird-style gameplay
* 📏 HC-SR04 ultrasonic hand-distance control
* 🖥️ SSD1306 128×64 OLED display
* 🚧 Randomly generated pipe gaps
* 🏆 Live score display
* ⭐ Persistent best score
* 📈 Game speed increases every 5 points
* 🎮 No joystick or buttons required
* 🔄 Automatic game restart after collision
* 📊 Smoothed ultrasonic readings
* 💻 Built using PlatformIO + VS Code

---

# 🧰 Components Required

| Component | Quantity | Purpose |
| --- | ---: | --- |
| ESP32 DevKit | 1 | Main controller |
| SSD1306 OLED 128×64 | 1 | Game display |
| HC-SR04 Ultrasonic Sensor | 1 | Hand-distance control |
| 1 kΩ Resistor | 1 | Voltage divider |
| 2 kΩ Resistor | 1 | Voltage divider |
| Breadboard | 1 | Prototyping |
| Jumper Wires | As required | Connections |
| USB Cable | 1 | Programming and power |

---

# 🔌 Circuit Diagram

![Circuit Diagram](images/circuit_diagram.svg)

## Text Schematic

### OLED

```text
ESP32             SSD1306 OLED

3V3   ───────────► VCC
GND   ───────────► GND
GPIO21 ──────────► SDA
GPIO22 ──────────► SCL
```

### HC-SR04

```text
ESP32             HC-SR04

VIN/5V ──────────► VCC
GND    ──────────► GND
GPIO5  ──────────► TRIG

ECHO ──[1kΩ]──┬──► GPIO18
              │
             [2kΩ]
              │
             GND
```

The resistor divider reduces the HC-SR04's 5 V ECHO signal to approximately 3.3 V for the ESP32 GPIO.

---

# 📌 Complete Pin Configuration

| Device | Pin | ESP32 |
| --- | --- | --- |
| OLED | VCC | 3.3 V |
| OLED | GND | GND |
| OLED | SDA | GPIO 21 |
| OLED | SCL | GPIO 22 |
| HC-SR04 | VCC | VIN / 5 V |
| HC-SR04 | GND | GND |
| HC-SR04 | TRIG | GPIO 5 |
| HC-SR04 | ECHO | GPIO 18 through divider |

---

# 🧠 How It Works

## 1. Ultrasonic Distance Measurement

The HC-SR04 sends an ultrasonic pulse and measures the returning echo.

The ESP32 calculates the distance using:

```text
Distance = Time × Speed of Sound / 2
```

The code converts the echo time into centimeters.

---

## 2. Hand Distance to Bird Position

The usable sensor range is:

```cpp
const float D_MIN = 5.0;
const float D_MAX = 30.0;
```

The measured distance is mapped to the OLED screen.

```text
5 cm
 ↓
Bird DOWN

30 cm
 ↓
Bird UP
```

---

## 3. Sensor Smoothing

To reduce sudden movement caused by ultrasonic noise, the program uses smoothing:

```cpp
dist = 0.6f * dist + 0.4f * c;
```

This produces more stable bird movement.

---

## 4. Pipe Generation

Two pipes are displayed at a time.

Each pipe contains:

```text
Top pipe
   │
   │
   ├──── Gap ────┐
   │             │
   │
Bottom pipe
```

The gap is randomly positioned so every game is slightly different.

---

## 5. Collision Detection

The ESP32 checks whether the bird overlaps a pipe.

If the bird touches a pipe:

```text
Collision
   ↓
GAME OVER
   ↓
Score displayed
   ↓
Game resets
```

---

## 6. Score System

Whenever the bird successfully passes a pipe:

```text
Score +1
```

Every 5 points the game becomes faster.

```cpp
if (score % 5 == 0 && speedPx < 4)
    speedPx++;
```

---

## 7. Best Score

The best score is stored using the ESP32 `Preferences` library.

Therefore the high score remains stored even after restarting the ESP32.

---

# 💻 Software & Libraries

## Development Environment

* VS Code
* PlatformIO
* ESP32 Arduino Framework
* C++

## Libraries

PlatformIO installs these libraries automatically:

* Adafruit SSD1306
* Adafruit GFX Library

The ESP32 `Preferences` library is part of the ESP32 Arduino framework.

---

# 🛠️ Installation

## 1. Clone the project

```bash
git clone https://github.com/solo-prince/ESP32_FlappyBird.git
cd ESP32_FlappyBird
```

---

## 2. Open in VS Code

Open the project folder in **VS Code** with the PlatformIO extension installed.

```bash
code .
```

---

## 3. Build the project

```bash
pio run
```

---

## 4. Upload to ESP32

Connect the ESP32 through USB and run:

```bash
pio run -t upload
```

---

## 5. Open Serial Monitor

```bash
pio device monitor
```

The serial monitor can be used to observe the ultrasonic distance readings.

---

# 🎮 Usage

1. Power the ESP32.
2. The Flappy Bird game starts automatically.
3. Place your hand above the HC-SR04.
4. Move your hand farther away to move the bird upward.
5. Move your hand closer to move the bird downward.
6. Pass through the pipe gaps to increase the score.
7. Avoid hitting the pipes.

### Control Summary

| Hand Position | Bird Action |
| --- | --- |
| Far from sensor | 🐦 UP |
| Near sensor | 🐦 DOWN |
| Pipe collision | 💀 GAME OVER |

---

# 🧪 Testing Procedure

## Test 1 — OLED

After uploading the program, the OLED should display:

```text
Score: 0

BEST 0
```

along with the game graphics.

---

## Test 2 — Ultrasonic Sensor

Open the Serial Monitor:

```bash
pio device monitor
```

You should see distance values similar to:

```text
Distance: 14.8
Distance: 15.2
Distance: 13.9
```

---

## Test 3 — Bird Movement

Move your hand slowly above the sensor.

```text
Hand farther
     ↓
Bird moves upward
```

and:

```text
Hand closer
     ↓
Bird moves downward
```

---

## Test 4 — Collision

Allow the bird to hit one of the pipes.

The OLED should display:

```text
GAME OVER

Score: X
Best : X
```

After a short delay the game automatically restarts.

---

# 🛠️ Troubleshooting

## OLED blank

Check:

```text
VCC  → 3.3V
GND  → GND
SDA  → GPIO21
SCL  → GPIO22
```

The default I2C address is:

```cpp
0x3C
```

---

## Ultrasonic sensor not responding

Check:

```text
TRIG → GPIO5
ECHO → GPIO18
```

Also verify that the ECHO signal uses the resistor divider before connecting to the ESP32.

---CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/esp32dev.html
PLATFORM: Espressif 32 (7.1.3) > Espressif ESP32 Dev Module
HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
DEBUG: Current (cmsis-dap) External (cmsis-dap, esp-bridge, esp-prog, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa)
PACKAGES: 
 - framework-arduinoespressif32 @ 4.20017.260907+sha.dcc1105b 
 - tool-esptoolpy @ 2.41100.260830 (4.11.0) 
 - toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch5
LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain, Compatibility ~ soft
Found 36 compatible libraries
Scanning dependencies...
Dependency Graph
|-- Adafruit GFX Library @ 1.12.6
|-- Adafruit SSD1306 @ 2.5.17
|-- Preferences @ 2.0.0
|-- Wire @ 2.0.0
Building in release mode
Retrieving maximum program size .pio/build/esp32dev/firmware.elf
Checking size .pio/build/esp32dev/firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [=         ]   6.7% (used 22020 bytes from 327680 bytes)
Flash: [==        ]  22.4% (used 294037 bytes from 1310720 bytes)
======================================== [SUCCESS] Took 7.61 seconds ========================================
pravin@pravin-Lenovo-E41-25:~/Documents/PlatformIO/Projects/ESP32_FlappyBird$ echo "# ESP32_FlappyBird" > README.md
pravin@pravin-Lenovo-E41-25:~/Documents/PlatformIO/Projects/ESP32_FlappyBird$ git add README.md
git commit -m "Update project documentation"
git push
On branch main
Your branch is up to date with 'origin/main'.

nothing to commit, working tree clean
Everything up-to-date
pravin@pravin-Lenovo-E41-25:~/Documents/PlatformIO/Projects/ESP32_FlappyBird$ 

## Bird movement is unstable

The sensor is sensitive to sudden hand movement and reflections.

Try:

* Moving the hand slowly
* Keeping the hand centered above the sensor
* Keeping the sensor clear of nearby objects
* Adjusting the smoothing values

Current smoothing:

```cpp
dist = 0.6f * dist + 0.4f * c;
```

---

## Bird does not move correctly

Check the distance range:

```cpp
const float D_MIN = 5.0;
const float D_MAX = 30.0;
```

Adjust these values according to the physical position of your sensor.

---

## ESP32 does not upload

Check the port:

```bash
pio device list
```

Then upload using the detected port if necessary:

```bash
pio run -t upload --upload-port /dev/ttyUSB0
```

Replace `/dev/ttyUSB0` with your actual ESP32 port.

---

# ⚠️ Limits & Safety

* The HC-SR04 ECHO output is approximately 5 V and must not be connected directly to an ESP32 GPIO.
* Use the resistor divider on the ECHO line.
* Keep the ultrasonic sensor facing the hand clearly.
* Avoid placing objects very close to the sensor.
* This project is intended for educational and experimental use.

---

# 📁 Project Structure

```text
ESP32_FlappyBird/
│
├── src/
│   └── main.cpp
│
├── images/
│   ├── flappy_bird_hardware.jpg
│   └── circuit_diagram.svg
│
├── platformio.ini
├── .gitignore
└── README.md
```

---

# 🚀 Future Improvements

* 🔊 Add a buzzer for collision and scoring sounds
* 🎵 Add background game sounds
* 🏅 Add multiple difficulty levels
* ❤️ Add lives system
* 🎨 Improve bird and pipe graphics
* 🌈 Add a color OLED version
* 📱 Add Bluetooth/Wi-Fi score sharing
* 🎮 Add physical buttons as an alternative controller
* 🕹️ Add multiple game modes
* 🏆 Add an online leaderboard

---

# 📚 Learning Outcomes

This project demonstrates practical concepts in:

* ESP32 programming
* Ultrasonic distance measurement
* GPIO control
* I2C communication
* SSD1306 OLED graphics
* Sensor data smoothing
* Collision detection
* Game-loop programming
* Random number generation
* Non-volatile data storage
* PlatformIO workflow
* VS Code embedded development

---

# 👨‍💻 Author

**solo-prince**

ESP32 Flappy Bird — Ultrasonic Controlled Game

GitHub:

https://github.com/solo-prince

---

# 📄 License

This project is intended for educational and personal development purposes.

You may modify and improve the project for your own electronics, embedded-systems and game-development experiments.

