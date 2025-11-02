# Mini Weather Station ()

This module runs on an ESP32 and reads three sensors:
- Light sensor (ADC on GPIO36)
- DHT11 (GPIO26) for temperature and humidity
- NTC thermistor (ADC on GPIO34)

Sensor readings are published as JSON over MQTT to topic `sensors/data/weatherStation`. A Node-RED flow subscribes to that topic, optionally transforms and enriches the data, and writes the measurements in real time to InfluxDB. Grafana reads from InfluxDB and provides dashboards with multiple visualizations (time series, gauges, single-stat panels, thresholds and aggregated views).

## Data format
Published JSON contains some fields like:
- `light` — raw ADC value (0–4095)
- `thermistorTemp` — thermistor temperature in °C (computed using beta approximation)
- `humidity` — DHT11 relative humidity (%)
- `tempC` — DHT11 temperature in °C
- `tempF` — DHT11 temperature in °F

Example payload:
```json
{
  "light": 1200,
  "thermistorTemp": 22.4,
  "humidity": 45.2,
  "tempC": 22.7,
  "tempF": 72.9
}
```

## Thermistor calculation
Thermistor temperature is computed using the beta approximation:
$$
T(°C) = \frac{1}{\dfrac{1}{T_0} + \dfrac{1}{\beta}\ln\left(\dfrac{R}{R_0}\right)} - 273.15
$$
where $T_0$ is 298.15 K (25 °C), $R_0$ is the nominal resistance (10 kΩ), $R$ is the measured resistance, and $\beta$ is the thermistor beta value (3950 in this project).

## Node-RED, InfluxDB and Grafana (operational notes)
- Node-RED:
  - Subscribe to MQTT topic `sensors/data/weatherStation`.
  - Use a JSON node (or function node) to parse and map fields to InfluxDB measurement and tags.
  - Write points to InfluxDB with a retention policy appropriate for your use case.
- InfluxDB:
  - Store each sensor as fields on a measurement (e.g., `weather_station`).
  - Use tags for device id/location if multiple devices report.
- Grafana:
  - Connect Grafana to InfluxDB.
  - Build panels:
    - Time-series for temp/humidity/light
    - Gauge or stat for current temperature and humidity
    - Thresholds/alerts for out-of-range values
    - Aggregation panels (min/max/avg over interval)

## Configuration and where to look in this repo
- Wi‑Fi credentials: [`src/credentials.h`](src/credentials.h) (this file is ignored by git)
- MQTT client and broker config: see [`src/wifimqtt.h`](src/wifimqtt.h)
  - Contains functions: [`connectAP`](src/wifimqtt.h), [`reconnect`](src/wifimqtt.h), [`callback`](src/wifimqtt.h)
- JSON payload and publishing: see [`src/main.cpp`](src/main.cpp) — function [`sendSensorsDataViaMQTT`](src/main.cpp)
- Build & environment: [`platformio.ini`](platformio.ini)
- Dev container config (optional): [`.devcontainer/devcontainer.json`](.devcontainer/devcontainer.json)

## Getting started
1. Set your Wi‑Fi SSID and password in [`src/credentials.h`](src/credentials.h).
2. Configure the MQTT broker settings in [`src/wifimqtt.h`](src/wifimqtt.h).
3. Build and upload with PlatformIO:
   - See [`platformio.ini`](platformio.ini)
4. Ensure Node-RED subscribes to `sensors/data/weatherStation` and writes to InfluxDB.
5. Open Grafana and create dashboards querying the InfluxDB measurement.

## Notes
- Credentials are stored in `src/credentials.h` and intentionally omitted from source control via `.gitignore`.
- Topic used by device: `sensors/data/weatherStation`.
- If you change pin assignments or thermistor constants, update the relevant constants in [`src/main.cpp`](src/main.cpp).
