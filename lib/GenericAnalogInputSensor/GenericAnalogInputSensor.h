#pragma once

#include <Arduino.h>
#include "../../include/ISensor.h"
#include "../../include/ResultCode.h"
#include "SensorResult.h"

/**
 * @brief Generic analog input sensor for reading voltage from an analog pin.
 * Reads the analog value and converts it to voltage (V) using the reference voltage.
 * Designed for simple analog sensors or voltage monitoring.
 */
class GenericAnalogInputSensor : public ISensor {
public:
    /**
     * @param sensorName Name of the sensor.
     * @param pin Analog pin to read from.
     * @param vRef Reference voltage for ADC conversion (default: 3.3V).
     * @param interval Update interval in ms (default: 60000 ms).
     */
    GenericAnalogInputSensor(const char* sensorName, uint8_t pin, float vRef = 3.3f, unsigned long interval = 60000)
        : ISensor(sensorName, interval), analogPin(pin), referenceVoltage(vRef) {}
    ~GenericAnalogInputSensor() override = default;

    void begin() override;
    void update() override;
    SensorResult readValues(bool force = false, bool updateReadTime = true) override;

private:
    uint8_t analogPin;
    float referenceVoltage;
    bool isInitialized = false;
};
