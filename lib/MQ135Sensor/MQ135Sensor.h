#pragma once

#include <Arduino.h>
#include <MQUnifiedsensor.h>

#include "../../include/ISensor.h"
#include "../../include/ResultCode.h"
#include "SensorResult.h"

class MQ135Sensor : public ISensor {

    public:
        
    void begin() override;
    void update() override;
    SensorResult readValues(bool force = false, bool updateReadTime = true) override;
    MQ135Sensor(const char* sensorName, uint8_t pin, unsigned long interval = 60000) : ISensor(sensorName, interval), mqSensor("ESP-32", 3.3, 12, pin, "MQ-135") {}
    ~MQ135Sensor() override = default;

    private:
        MQUnifiedsensor mqSensor;
        bool isInitialized = false;
        
};