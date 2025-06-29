#pragma once
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include "settings.h"
#include <SensorResult.h>

class InfluxLogger {
public:
    InfluxLogger(const char* deviceName, bool simulated = false);
    void begin();
    void logSensorResult(const SensorResult& result);
    void flush();
private:
    InfluxDBClient client;
    const char* deviceName;
    const bool simulated; // If true, does not log to InfluxDB but simulates the logging process
};
