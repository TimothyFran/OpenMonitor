#pragma once

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "../../include/ISensor.h"
#include "../../include/ResultCode.h"
#include "SensorResult.h"

class MPU6050Sensor : public ISensor {

    public:
        
    void begin() override;
    void update() override;
    SensorResult readValues(bool force = false, bool updateReadTime = true) override;
    MPU6050Sensor(const char* sensorName, unsigned long interval = 200) : ISensor(sensorName, interval) {}
    ~MPU6050Sensor() override = default;

    private:
        Adafruit_MPU6050 mpu;
        bool isInitialized = false;

        float ax_offset = 0.0f, ay_offset = 0.0f, az_offset = 0.0f;
        float gx_offset = 0.0f, gy_offset = 0.0f, gz_offset = 0.0f;

        void calibrate();
        
};