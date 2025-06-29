#include <Arduino.h>
#include <ArduinoLog.h>

#include "MQ135Sensor.h"
#include "../../include/SensorExceptions.h"

void MQ135Sensor::begin() {

    if (isInitialized) {
        Log.warning(F("MQ135Sensor already initialized."));
        return;
    }
    
    mqSensor.setRegressionMethod(1); // _PPM = a * ratio ^ b
    mqSensor.init();
    mqSensor.serialDebug(true);

    // TODO: Add calibration logic
    mqSensor.setR0(10.0f);

    isInitialized = true;
}

void MQ135Sensor::update() {
    if (!isInitialized) {
        Log.error(F("MQ135Sensor not initialized. Call begin() first."));
        return;
    }
}

SensorResult MQ135Sensor::readValues(bool force, bool updateReadTime) {
    if (!isInitialized) {
        Log.error(F("MQ135Sensor not initialized. Call begin() first."));
        throw SensorNotInitializedException();
    }

    if (!force && millis() - lastReadTime < updateInterval) {
        return SensorResult(getSensorName());
    }

    if(updateReadTime)
        lastReadTime = millis();

    mqSensor.update();

    SensorResult result(getSensorName());

    // Correction factor should be calculated based on temperature/humidity if available
    float correctionFactor = 0.0f; // TODO: Implement environmental correction

    /*
        Exponential regression:
        GAS      | a      | b
        CO       | 605.18 | -3.937  
        Alcohol  | 77.255 | -3.18 
        CO2      | 110.47 | -2.862
        Toluen   | 44.947 | -3.445
        NH4      | 102.2  | -2.473
        Aceton   | 34.668 | -3.369
    */

    // CO
    mqSensor.setA(605.18f);
    mqSensor.setB(-3.937f);
    float co = mqSensor.readSensor(false, correctionFactor);
    result.set("CO", co);

    mqSensor.serialDebug();

    // Alcohol
    mqSensor.setA(77.255f);
    mqSensor.setB(-3.18f);
    float alcohol = mqSensor.readSensor(false, correctionFactor);
    result.set("Alcohol", alcohol);

    // CO2
    mqSensor.setA(110.47f);
    mqSensor.setB(-2.862f);
    float co2 = mqSensor.readSensor(false, correctionFactor);
    result.set("CO2", co2);

    // Toluene
    mqSensor.setA(44.947f);
    mqSensor.setB(-3.445f);
    float toluene = mqSensor.readSensor(false, correctionFactor);
    result.set("Toluen", toluene);

    // NH4
    mqSensor.setA(102.2f);
    mqSensor.setB(-2.473f);
    float nh4 = mqSensor.readSensor(false, correctionFactor);
    result.set("NH4", nh4);

    // Acetone
    mqSensor.setA(34.668f);
    mqSensor.setB(-3.369f);
    float acetone = mqSensor.readSensor(false, correctionFactor);
    result.set("Aceton", acetone);

    Log.verboseln(F("MQ135Sensor::readValues() - Read values for sensor '%s': CO: %F, Alcohol: %F, CO2: %F, Toluene: %F, NH4: %F, Aceton: %F"), getSensorName(), co, alcohol, co2, toluene, nh4, acetone);

    return result;
}