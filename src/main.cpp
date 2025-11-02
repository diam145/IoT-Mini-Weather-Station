#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "wifimqtt.h"


#define LIGHT_SENSOR_PIN 36 // GIOP36 (ADC0)

#define DHT11_PIN  26 // ESP32 pin GPIO26 connected to DHT11 sensor
DHT dht11(DHT11_PIN, DHT11);

// Thermistor setup
const int thermistorPin = 34;  // ADC Pin connected to the NTC Thermistor
const float referenceVoltage = 3.3;  // Reference voltage (3.3V)
const int seriesResistor = 10000;  // Series resistor value (10kΩ)
const float beta = 3950;  // Beta value for the thermistor (depends on the thermistor used)
const float tempNominal = 25.0;  // Nominal temperature at 25°C
const int resistanceNominal = 10000;  // Nominal resistance at 25°C (10kΩ)



void sendSensorsDataViaMQTT (float light, float themistorTEmp,float h, float tC, float tF) {

    Serial.print("Sending data via MQTT -> ");
    JsonDocument doc;
    doc["light"] = light;
    doc["thermistorTemp"] = themistorTEmp;
    doc["humidity"] = h;
    doc["tempC"] = tC;
    doc["tempF"] = tF;
    
    char buffer[256];
    serializeJson(doc, buffer);
    client.publish("sensors/data/weatherStation", buffer);
    
    return;
}

void setup() {
    // Serial communication at 115200 bits per second:
    Serial.begin(115200);

    // set the ADC attenuation to 11 dB (up to ~3.3V input)
    analogSetAttenuation(ADC_11db);

    dht11.begin(); // initialize the DHT11 sensor

    // Thermistor pin mode
    pinMode(thermistorPin, INPUT);

    connectAP();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    if (!client.loop()) { // client not looping anymore with mqtt
        client.connect("ESP32-");
    }

    // reads the input on analog pin (value between 0 and 4095)
    int analogValue = analogRead(LIGHT_SENSOR_PIN);

    Serial.print("Analog Value = ");
    Serial.print(analogValue);

    // We'll have a few threshholds, qualitatively determined
    if (analogValue < 50) {
        Serial.println(" => Dark");
    } else if (analogValue < 500) {
        Serial.println(" => Dim");
    } else if (analogValue < 1500) {
        Serial.println(" => Light");
    } else if (analogValue < 2500) {
        Serial.println(" => Bright");
    } else {
        Serial.println(" => Very bright");
    }


    // Thermistor temperature calculation
    int adcValue = analogRead(thermistorPin);  // Read the ADC value (0-4095)
    float voltage = adcValue * (referenceVoltage / 4095.0);  // Convert ADC value to voltage
    float resistance = (seriesResistor * (referenceVoltage / voltage - 1));  // Calculate the resistance of the thermistor

    // Calculate temperature using the Steinhart-Hart equation or an approximation
    float thermistorTemperature = 1.0 / (log(resistance / resistanceNominal) / beta + 1.0 / (tempNominal + 273.15)) - 273.15;

    // Print the temperature in Celsius
    Serial.print("Temperature: ");
    Serial.print(thermistorTemperature);
    Serial.println(" C");



    // DHT11 sensor readings
    // read humidity
    float humi  = dht11.readHumidity();
    // read temperature in Celsius
    float tempC = dht11.readTemperature();
    // read temperature in Fahrenheit
    float tempF = dht11.readTemperature(true);

    // check whether the reading is successful or not
    if ( isnan(tempC) || isnan(tempF) || isnan(humi)) {
        Serial.println("Failed to read from DHT11 sensor!");
        
        sendSensorsDataViaMQTT(analogValue, thermistorTemperature,-1, -1, -1); // indicate error with -1 values
    } else {
        Serial.print("Humidity: ");
        Serial.print(humi);
        Serial.print("%");

        Serial.print("  |  ");

        Serial.print("Temperature: ");
        Serial.print(tempC);
        Serial.print("°C  ~  ");
        Serial.print(tempF);
        Serial.println("°F");

        // SendSensorsDataViaMQTT
        sendSensorsDataViaMQTT(analogValue, thermistorTemperature, humi, tempC, tempF);
    }

    delay(3000);
}



