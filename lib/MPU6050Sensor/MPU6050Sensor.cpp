#include "MPU6050Sensor.h"
#include <Arduino.h>
#include "../../include/SensorExceptions.h"

void MPU6050Sensor::begin() {
    const uint8_t max_attempts = 5;
    uint8_t attempts = 0;

    while (attempts < max_attempts) {
        if (!mpu.begin()) {
            attempts++;
            delay(10);
            continue;
        }
        break;
    }

    if (attempts == max_attempts) {
        isInitialized = false;
        throw SensorInitializationException("MPU6050 initialization failed after multiple attempts");
    }

    isInitialized = true;
    calibrate();
    Log.notice(F("[MPU6050] Sensor initialized successfully after %d attempts" CR), attempts);
}

void MPU6050Sensor::update() {
    if (!isInitialized) {
        throw SensorNotInitializedException();
    }

    constexpr float threshold = 0.1f;
    constexpr uint32_t durationMs = 2000;
    static uint32_t thresholdStartTime = 0;
    static bool thresholdExceeded = false;

    sensors_event_t accel, gyro, temp;
    if (!mpu.getEvent(&accel, &gyro, &temp)) {
        Log.warning(F("[MPU6050] Failed to read sensor data in update()" CR));
        return;
    }

    float ax = accel.acceleration.x - ax_offset;
    float ay = accel.acceleration.y - ay_offset;
    // Subtract gravity (9.80665 m/s^2) from Z acceleration to account for Earth's gravity
    float az = accel.acceleration.z - az_offset - 9.80665f;

    float gx = gyro.gyro.x - gx_offset;
    float gy = gyro.gyro.y - gy_offset;
    float gz = gyro.gyro.z - gz_offset;

    // Check if any axis exceeds the threshold
    thresholdExceeded = (fabs(ax) > threshold) || (fabs(ay) > threshold) || (fabs(az) > threshold) ||
                        (fabs(gx) > threshold) || (fabs(gy) > threshold) || (fabs(gz) > threshold);

    if(!thresholdExceeded) {
        thresholdStartTime = 0;
        return;
    }

    if(thresholdStartTime == 0) {
        thresholdStartTime = millis();
        return;
    }

    if(millis() - thresholdStartTime < durationMs)
        return;

    // If we reach here, it means the threshold was exceeded for the specified duration
    Log.notice(F("[MPU6050] Axis exceeded threshold for %d ms, starting auto-calibration" CR), durationMs);
    calibrate();

    thresholdExceeded = false;
    thresholdStartTime = 0;
}

SensorResult MPU6050Sensor::readValues(bool force, bool updateReadTime) {
    if (!isInitialized) {
        throw SensorNotInitializedException();
    }

    if (millis() - lastReadTime < updateInterval && !force) {
        return SensorResult(getSensorName());
    }

    if (updateReadTime)
        lastReadTime = millis();

    sensors_event_t accel, gyro, temp;

    if (!mpu.getEvent(&accel, &gyro, &temp)) {
        throw SensorReadException("Failed to read sensor data");
    }

    SensorResult result(this->getSensorName());
    
    float ax = accel.acceleration.x - ax_offset;
    float ay = accel.acceleration.y - ay_offset;
    float az = accel.acceleration.z - az_offset;
    float gx = gyro.gyro.x - gx_offset;
    float gy = gyro.gyro.y - gy_offset;
    float gz = gyro.gyro.z - gz_offset;
    
    // Popola i valori dell'accelerometro
    result.set("ax", ax);
    result.set("ay", ay);
    result.set("az", az);

    // Popola i valori del giroscopio
    result.set("gx", gx);
    result.set("gy", gy);
    result.set("gz", gz);

    // Popola la temperatura
    result.set("temp", temp.temperature);

    return result;
}

/**
 * Calibrate the MPU6050 sensor.
 * Calculate and apply offsets for accelerometer and gyroscope.
 * This method assumes the sensor is stationary during calibration.
 */
void MPU6050Sensor::calibrate() {
    if (!isInitialized) {
        throw SensorNotInitializedException();
    }

    constexpr size_t samples = 1000;
    constexpr uint16_t delayMs = 2;

    Log.notice(F("[MPU6050] Starting calibration with %d samples" CR), samples);

    for (size_t i = 0; i < samples; ++i) {
        sensors_event_t accel, gyro, temp;
        if (!mpu.getEvent(&accel, &gyro, &temp)) {
            Log.warning(F("[MPU6050] Failed to read sensor data during calibration at sample %d" CR), i);
            continue;
        }
        ax_offset += accel.acceleration.x;
        ay_offset += accel.acceleration.y;
        az_offset += accel.acceleration.z;
        gx_offset += gyro.gyro.x;
        gy_offset += gyro.gyro.y;
        gz_offset += gyro.gyro.z;
        delay(delayMs);
    }

    ax_offset /= samples;
    ay_offset /= samples;
    az_offset = (az_offset / samples) - 9.80665f; // Remove gravity (m/s^2)
    gx_offset /= samples;
    gy_offset /= samples;
    gz_offset /= samples;

    Log.notice(F("[MPU6050] Calibration complete. Offsets: ax=%F ay=%F az=%F gx=%F gy=%F gz=%F" CR),
               ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset);
}