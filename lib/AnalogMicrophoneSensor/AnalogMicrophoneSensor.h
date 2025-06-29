#pragma once

#include <Arduino.h>
#include "../../include/ISensor.h"
#include "../../include/ResultCode.h"
#include "../SensorResult/SensorResult.h"

/**
 * @brief Analog microphone sensor (e.g. MAX4466) for sound level measurement in dB SPL.
 * Samples the analog input asynchronously, computes peak-to-peak amplitude, and estimates dB SPL.
 */
class AnalogMicrophoneSensor : public ISensor {
public:
    /**
     * @param sensorName Name of the sensor.
     * @param pin Analog pin to read from.
     * @param vRef Reference voltage for ADC conversion (default: 3.3V).
     * @param gain Amplifier gain (linear, e.g. 75 for MAX4466 at center).
     * @param samplingDuration Sampling duration in ms (default: 50 ms).
     */
    AnalogMicrophoneSensor(const char* sensorName, uint8_t pin)
        : ISensor(sensorName, ANALOG_MIC_SAMPLING_DURATION), analogPin(pin) {}
    ~AnalogMicrophoneSensor() override = default;

    void begin() override;
    void update() override;
    SensorResult readValues(bool force = false, bool updateReadTime = true) override;

private:
    uint8_t analogPin;
    bool isInitialized = false;

    // Asynchronous sampling state
    unsigned long samplingStartTime = 0;
    bool isSampling = false;
    
    // Sampling accumulators
    unsigned int signalMax = 0;
    unsigned int signalMin = 4095; // 12-bit ADC for ESP32
    float sumVrms = 0.0f;
    float peakVrms = 0.0f;
    unsigned int sampleCount = 0;

    // Last computed values
    unsigned int peakToPeak = 0;
    float mean_dBSPL = 0.0f;
    float peak_dBSPL = 0.0f;
    
    void resetSamplingState();
    void processSample();
    void computeResults();
};
