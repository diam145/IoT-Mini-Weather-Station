#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "credentials.h"

// MQTT Broker settings
String clientId = "ESP32-";
const char* mqtt_server = "127.0.0.1";
const char *mqttUser = "mqttUser";
const char *mqttPassword = "mqttPassword";
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
