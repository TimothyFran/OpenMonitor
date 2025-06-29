#include "GenericAnalogInputSensor.h"
#include <ArduinoLog.h>
#include "../../include/SensorExceptions.h"

void GenericAnalogInputSensor::begin() {
    // On most Arduino boards, analog pins do not require explicit initialization.
    // This method is provided for interface consistency and future extensibility.
    isInitialized = true;
    Log.notice(F("[GenericAnalogInput] Sensor initialized on pin %d" CR), analogPin);
}

void GenericAnalogInputSensor::update() {
    if (!isInitialized) {
        throw SensorNotInitializedException();
    }
    // No periodic update required for simple analog read.
}

SensorResult GenericAnalogInputSensor::readValues(bool force, bool updateReadTime) {
    if (!isInitialized) {
        throw SensorNotInitializedException();
    }
    if (millis() - lastReadTime < updateInterval && !force) {
        return SensorResult(getSensorName());
    }
    if (updateReadTime) {
        lastReadTime = millis();
    }
    int rawValue = analogRead(analogPin);
    float voltage = (static_cast<float>(rawValue) / 4095.0f) * referenceVoltage;
    SensorResult result(this->getSensorName());
    result.set("voltage", voltage); // Voltage in V (SI unit)
    result.set("raw", rawValue);    // Raw ADC value
    Log.verboseln(F("[GenericAnalogInput] Read voltage: %F V (raw: %d) on pin %d"), voltage, rawValue, analogPin);
    return result;
}
