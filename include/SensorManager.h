#pragma once
#include <Arduino.h>
#include <ArduinoLog.h>
#include <vector>
#include "ISensor.h"
#include "ResultCode.h"
#include "SensorResult.h"
#include "SensorExceptions.h"

class SensorManager {
public:
    struct SensorInstance {
        ISensor* sensor;
        bool throwOnInitializationError;
        bool throwOnUpdateError;
    };

    void addSensor(ISensor* sensor, bool throwOnInitializationError = true, bool throwOnUpdateError = true) {
        sensors.push_back({sensor, throwOnInitializationError, throwOnUpdateError});
    }

    void beginAll() {
        for (const SensorInstance& entry : sensors) {
            Log.verboseln(F("Initializing sensor: %s"), entry.sensor->getSensorName());
            try {
                entry.sensor->begin();
            } catch (const SensorInitializationException& e) {
                Log.error(F("Sensor init error: %s\n"), e.what());
                if (entry.throwOnInitializationError) {
                    Log.fatal(F("Critical sensor initialization error, propagating exception.\n"));
                    throw;
                }
            }
        }
    }

    void updateAll() {
        for (const SensorInstance& entry : sensors) {
            try {
                entry.sensor->update();
            } catch (const SensorNotInitializedException& e) {
                Log.error(F("Sensor update error: %s\n"), e.what());
                if (entry.throwOnUpdateError) {
                    Log.fatal(F("Critical sensor update error, propagating exception.\n"));
                    throw;
                }
            }
        }
    }

    void logAllValues() {
        for (const SensorInstance& entry : sensors) {
            Log.notice(F("Logging values from sensor: %s\n"), entry.sensor->getSensorName());
            try {
                SensorResult result = entry.sensor->readValues();
                Log.notice(F("--- Sensor values ---\n"));
                logSensorResult(result);
            } catch (const SensorNotInitializedException& e) {
                Log.error(F("Sensor not initialized: %s\n"), e.what());
                if (entry.throwOnUpdateError) {
                    Log.fatal(F("Critical sensor not initialized error, propagating exception.\n"));
                    throw;
                }
            } catch (const SensorReadException& e) {
                Log.error(F("Sensor read error: %s\n"), e.what());
                if (entry.throwOnUpdateError) {
                    Log.fatal(F("Critical sensor read error, propagating exception.\n"));
                    throw;
                }
            }
        }
    }

    /**
     * Reads all sensor values by calling readValues() on each sensor.
     * Returns a vector of SensorResult, one for each sensor.
     * Throws if an exception is not handled internally.
     */
    std::vector<SensorResult> readAll(bool forceRead = false, bool updateReadTime = true) {
        std::vector<SensorResult> results;
        for (const SensorInstance& entry : sensors) {
            if (entry.sensor == nullptr) {
                Log.error(F("Null sensor pointer detected in SensorManager::readAll(). Skipping.\n"));
                continue;
            }
            try {
                SensorResult result = entry.sensor->readValues(forceRead, updateReadTime);
                if(result.isEmpty()) continue; // Not time to read or no data available
                results.push_back(result);
                Log.verboseln(F("Sensor %s read successfully."), entry.sensor->getSensorName());
            } catch (const SensorReadException& e) {
                Log.error(F("Sensor read error: %s\n"), e.what());
                if (entry.throwOnUpdateError) {
                    Log.fatal(F("Critical sensor read error, propagating exception.\n"));
                    throw;
                }
            }
        }
        return results;
    }

    /**
     * Reads all sensor values and logs them to the console.
     * This is a convenience method that combines readAll() and logging.
     * Throws if an exception is not handled internally.
     */
    void readAndLogAllValues() {
        Log.notice(F("\n--- START SENSOR LOG ---\n"));
        std::vector<SensorResult> results = readAll(true, false);
        for (const SensorResult& result : results) {
            logSensorResult(result);
        }
        Log.notice(F("--- END SENSOR LOG ---\n"));
    }

private:
    std::vector<SensorInstance> sensors;

    void logSensorResult(const SensorResult& result) {
        uint8_t keysCount = result.countEntries();
        for (uint8_t i = 0; i < keysCount; i++) {
            const char* key = result.getKey(i);
            float value = result.getValue(i);
            Log.notice(F("%s: %F\n"), key, value);
        }
    }
};