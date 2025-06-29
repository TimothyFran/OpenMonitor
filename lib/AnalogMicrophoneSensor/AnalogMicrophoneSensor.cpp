#include "AnalogMicrophoneSensor.h"
#include <ArduinoLog.h>
#include <math.h>
#include "../../include/SensorExceptions.h"

// Microphone sensitivity: -44 dBV/Pa = 6.31 mV/Pa
constexpr float MIC_SENSITIVITY_V_PER_PA = 0.00631f;
constexpr float DB_SPL_REF = 94.0f; // Reference SPL for sensitivity
constexpr int ADC_RESOLUTION = 4095; // 12-bit ADC for ESP32

void AnalogMicrophoneSensor::begin() {
    isInitialized = true;
    resetSamplingState();
    Log.notice(F("[AnalogMicrophone] Sensor initialized on pin %d" CR), analogPin);
}

void AnalogMicrophoneSensor::update() {
    if (!isInitialized) {
        throw SensorNotInitializedException();
    }
    
    unsigned long currentTime = millis();
    
    // Start sampling if not already sampling and enough time has passed
    if (!isSampling && (currentTime - lastReadTime >= updateInterval)) {
        samplingStartTime = currentTime;
        isSampling = true;
        resetSamplingState();
        Log.verboseln(F("[AnalogMicrophone][update] Starting sampling period"));
    }
    
    // Continue sampling if within the sampling window
    if (isSampling) {
        if (currentTime - samplingStartTime < updateInterval) {
            processSample();
        } else {
            // Sampling period completed
            computeResults();
            isSampling = false;
            lastReadTime = currentTime;
            Log.verboseln(F("[AnalogMicrophone][update] Sampling completed - mean dB SPL: %F, peak dB SPL: %F"), mean_dBSPL, peak_dBSPL);
        }
    }
}

SensorResult AnalogMicrophoneSensor::readValues(bool force, bool updateReadTime) {
    if (!isInitialized) {
        throw SensorNotInitializedException();
    }
    
    // Return empty result if still sampling and not forced
    if (isSampling && !force) {
        return SensorResult(getSensorName());
    }
    
    SensorResult result(this->getSensorName());
    result.set("mean_dBSPL", mean_dBSPL); // dB SPL medio
    result.set("peak_dBSPL", peak_dBSPL); // dB SPL massimo
    
    Log.verboseln(F("[AnalogMicrophone][readValues] mean dB SPL: %F, peak dB SPL: %F"), mean_dBSPL, peak_dBSPL);
    
    return result;
}

void AnalogMicrophoneSensor::resetSamplingState() {
    signalMax = 0;
    signalMin = ADC_RESOLUTION;
    sumVrms = 0.0f;
    peakVrms = 0.0f;
    sampleCount = 0;
}

void AnalogMicrophoneSensor::processSample() {
    unsigned int sample = analogRead(analogPin);
    if (sample <= ADC_RESOLUTION) {
        if (sample > signalMax) signalMax = sample;
        if (sample < signalMin) signalMin = sample;
        float v = (static_cast<float>(sample) / ADC_RESOLUTION) * VREF_VALUE;
        float vBias = VREF_VALUE / 2.0f;
        float vAc = v - vBias;
        float vrmsSample = fabsf(vAc);
        sumVrms += vrmsSample;
        if (vrmsSample > peakVrms) peakVrms = vrmsSample;
        ++sampleCount;
    }
}

void AnalogMicrophoneSensor::computeResults() {
    peakToPeak = signalMax - signalMin;
    float vref = MIC_SENSITIVITY_V_PER_PA * ANALOG_MIC_GAIN;
    
    // Calculate mean and peak dB SPL
    float meanVrms = (sampleCount > 0) ? (sumVrms / sampleCount) : 0.0f;
    mean_dBSPL = (meanVrms > 0.0f) ? (20.0f * log10f(meanVrms / vref) + DB_SPL_REF) : 0.0f;
    peak_dBSPL = (peakVrms > 0.0f) ? (20.0f * log10f(peakVrms / vref) + DB_SPL_REF) : 0.0f;
    
    Log.verboseln(F("[AnalogMicrophone][computeResults] Samples: %d, peakToPeak: %d, mean dB SPL: %F, peak dB SPL: %F"), 
                  sampleCount, peakToPeak, mean_dBSPL, peak_dBSPL);
}
