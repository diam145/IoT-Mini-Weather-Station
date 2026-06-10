# IoT Mini Weather Station

An ESP32-based mini weather station that reads environmental data from multiple sensors and publishes it to an MQTT broker in real time. It also displays live readings on an LCD screen. Built with PlatformIO and the Arduino framework.

---

## Overview

This project turns an ESP32 into a self-contained weather station that:

1. Reads temperature and humidity from a **DHT11** sensor
2. Measures ambient light level from a **photoresistor** (GPIO36)
3. Reads temperature from a **thermistor** using the Steinhart-Hart equation
4. Displays all readings on an **LCD display**
5. Publishes a JSON payload to the MQTT topic `sensors/data/weatherStation` every loop cycle

---

## Features

- Multi-sensor data acquisition (DHT11, light sensor, thermistor)
- Light intensity classification (Dark, Dim, Bright, Very Bright)
- Temperature in both Celsius and Fahrenheit
- Real-time display on an LCD screen
- MQTT publish over Wi-Fi with auto-reconnect
- JSON payload serialization with ArduinoJson
- Credentials stored separately in `credentials.h`

---

## Hardware

| Component | Pin |
|---|---|
| DHT11 (Temp + Humidity) | GPIO26 |
| Photoresistor (Light) | GPIO36 (ADC0) |
| Thermistor (10kΩ + Beta 3950) | GPIO34 (ADC) |
| LCD Display | I2C / SPI (via `display.h`) |

---

## Tech Stack

| Layer | Technology |
|---|---|
| Microcontroller | ESP32 |
| Framework | Arduino (via PlatformIO) |
| Build system | PlatformIO + CMake + ESP-IDF |
| Connectivity | Wi-Fi + MQTT (port 1883) |
| MQTT Library | PubSubClient |
| Serialization | ArduinoJson |
| Sensor Library | DHT sensor library |
| Language | C / C++ |

---

## Project Structure

```
IoT-Mini-Weather-Station/
├── src/
│   ├── main.cpp           # Core logic: sensor reads, MQTT publish, display
│   ├── wifimqtt.h         # Wi-Fi + MQTT connection, reconnect, callback
│   ├── display.h          # LCD display initialization and rendering
│   └── credentials.h      # Wi-Fi SSID/password (not committed)
├── include/               # Additional headers
├── lib/                   # External libraries
├── test/                  # Unit tests
├── platformio.ini         # PlatformIO project configuration
├── sdkconfig.defaults     # ESP-IDF default config
└── CMakeLists.txt
```

---

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) (VS Code extension or CLI)
- ESP32 development board
- DHT11 sensor, photoresistor, thermistor (10kΩ), LCD display
- A running MQTT broker (e.g., [Mosquitto](https://mosquitto.org/))

### Setup

1. Clone the repository:

```bash
git clone https://github.com/diam145/IoT-Mini-Weather-Station.git
cd IoT-Mini-Weather-Station
```

2. Fill in your credentials in `src/credentials.h`:

```cpp
#define WIFI_SSID     "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
```

3. Update the MQTT broker address in `src/wifimqtt.h`:

```cpp
const char *mqtt_server = "your_broker_ip";
```

4. Wire up the components according to the pin table above.

5. Build and flash:

```bash
pio run --target upload
```

---

## MQTT Output

**Topic:** `sensors/data/weatherStation`

**Example Payload:**

```json
{
  "temperature_c": 22.5,
  "temperature_f": 72.5,
  "humidity": 48,
  "light": 620,
  "light_label": "Bright",
  "thermistor_temp_c": 21.8
}
```

---

## Author

**[@diam145](https://github.com/diam145)**

---

## License

This project does not currently have a license. All rights reserved by the author.
