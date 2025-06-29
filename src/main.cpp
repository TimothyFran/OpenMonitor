#include <Arduino.h>
#include <ArduinoLog.h>
#include <stdexcept>

#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"

#include "SensorManager.h"

SensorManager sensorManager;

#include "config.h"
#include "secrets.h"
#include "InfluxLogger.h"

InfluxLogger influxLogger("D0", false);

void printPrefix(Print* _logOutput, int logLevel) {
    _logOutput->print("[");
    _logOutput->print(millis() / 1000);
    _logOutput->print("] ");
}

void setup() {
    Serial.begin(115200);

    Log.setPrefix(printPrefix);
    Log.begin(LOG_LEVEL_TRACE, &Serial, false, true);

    WiFi.mode(WIFI_STA);
    /*WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Log.notice(F("Connecting to WiFi...\n"));
    }*/

    Log.notice(F("Connecting to WiFi...\n"));
    wifiMulti.addAP(SECRET_WIFI_SSID, SECRET_WIFI_PASSWORD);
    while (wifiMulti.run() != WL_CONNECTED) {
        delay(1000);
        Log.notice(F("Connecting to WiFi...\n"));
    }
    Log.notice(F("Connected to WiFi: %s\n"), SECRET_WIFI_SSID);

    influxLogger.begin();

    addSensorsToManager();
    sensorManager.beginAll();
    std::vector<SensorResult> results = sensorManager.readAll(true); // Force read all sensors to initialize them
    for (const SensorResult& result : results) {
        if (result.isEmpty()) continue; // Skip empty results
        try {
            influxLogger.logSensorResult(result);
        } catch (const SensorReadException& e) {
            Log.error(F("Error reading sensor '%s': %s\n"), result.getSensorName(), e.what());
        }
    }

}

void loop() {

    static unsigned long lastLogTime = 0;

    unsigned long now = millis();

    sensorManager.updateAll();
    std::vector<SensorResult> results = sensorManager.readAll();
    for (const SensorResult& result : results) {
        if (result.isEmpty()) continue; // Skip empty results
        try {
            influxLogger.logSensorResult(result);
        } catch (const SensorReadException& e) {
            Log.error(F("Error reading sensor '%s': %s\n"), result.getSensorName(), e.what());
        }
    }

    if (now - lastLogTime >= LOG_INTERVAL) {
        lastLogTime = now;
        try {
            sensorManager.readAndLogAllValues();
        } catch (const std::exception& e) {
            Log.error(F("Error occurred while logging sensor values: %s\n"), e.what());
        }
        influxLogger.flush();
    }

}