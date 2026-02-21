#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "credentials.h"


struct einkDataTemplate {
    String temperature;
    String weatherDescription;
    String day;
    String date;
    String humidity;
    String sunrise;
    String sunset;
    String update;
    bool show;
} einkData;

extern void displayOpenWeathermap(const einkDataTemplate &einkData);



// MQTT Broker settings
String clientId = "ESP32-";
const char* mqtt_server = "54.242.182.252";
const char *mqttUser = "pealsen";
const char *mqttPassword = "Pealsen#17";
WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    // Create a random client ID
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
        Serial.println("connected to MQTT");
        // Once connected, publish an announcement...
        
        // OUR specific subscription
        client.subscribe("sensors/data/weatherStation");
        client.subscribe("openweathermap");

    } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
    }
    }
}


void callback(char* topic, byte* payload, unsigned int length) {
    // Serial.print("Message arrived [");

    String messageTemp;
    for (int i = 0; i < length; i++) {
        // Serial.print((char)payload[i]);
        messageTemp += (char)payload[i];
    }
    // Serial.println();

    if (String(topic) == "sensors/data/weatherStation") {
        Serial.print("Message from weatherStation: ");
        Serial.println(messageTemp);

        // Stream& input;

        JsonDocument doc;

        DeserializationError error = deserializeJson(doc, messageTemp);

        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }

        // const char* device = doc["device"]; // "ESP32 is a good MCU"
        // int temperature = doc["temperature"]; // 33
        // int humidity = doc["humidity"]; // 90
        // int lux = doc["lux"]; // 643

        // String output = String("Device: ") + String(device) + String(", Temp: ") + String(temperature) + String(", Humidity: ") + String(humidity) + String(", Lux: ") + String(lux);
        // Serial.println(output);

    }

    if (String(topic) == "openweathermap") {
        Serial.print("Message from OpenWeathermap: ");
        Serial.println(messageTemp);
        // Further processing can be done here
        // Stream& input;

        JsonDocument doc;

        DeserializationError error = deserializeJson(doc, messageTemp);

        if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }

        float temperature = doc["temperature"]; // -2.3
        int humidity = doc["humidity"]; // 89
        String description = doc["description"]; // "overcast clouds"
        String day = doc["day"]; // "MON"
        String date = doc["date"]; // "10/11/25"
        String sunrise = doc["sunrise"]; // "6:55"
        String sunset = doc["sunset"]; // "16:38"
        String update = doc["update"]; // "23:30"
        bool show = doc["show"]; // false

        einkData.temperature = String(temperature, 1);
        einkData.humidity = String(humidity);
        einkData.weatherDescription = description;
        einkData.day = day;
        einkData.date = date;
        einkData.sunrise = sunrise;
        einkData.sunset = sunset;
        einkData.update = update;
        einkData.show = show;


        if (show) {
            Serial.println("Updating e-ink display with new data...");
            // Call your function to update the e-ink display here
            displayOpenWeathermap(einkData);
        } 
        else {
            Serial.println("Display update not required (show=false).");
            // Optionally, clear the screen if 'show' is false
            // lcd.fillScreen(ST7735_BLACK); 
            displayOpenWeathermap(einkData);
        }

    }


}

void connectAP() {
    Serial.println("Connecting to my WiFi...");
    WiFi.begin(ssid, wifiPwd);
    byte count = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
        count++;

        if(count > 30)
            ESP.restart();
    
    }
    // Add success message:
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
}
