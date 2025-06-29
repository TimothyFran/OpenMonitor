#if false

#include <Arduino.h>
#include "SensorManager.h"

// --- Instructions ---
// 1. Copy this template to 'config.h'.
// 2. Remove the 'false' at the top to enable this configuration.
// 3. Uncomment and configure the sensors you want to use.
// 4. Add each sensor to the manager in 'addSensorsToManager()'.
// 5. Do not modify this template directly; always copy it to create your configuration.

// Declare the global SensorManager instance.
// This is required for managing all sensors in the system.
extern SensorManager sensorManager;

// --- Sensor Includes and Instantiations ---
// Include and instantiate only the sensors you want to use in your project.
// Each sensor should have a unique name and, if needed, a configuration.

// Example: Including and instantiating an MPU6050 sensor
// #include <MPU6050Sensor.h>
// MPU6050Sensor mpu6050_1 = MPU6050Sensor("MPU6050_1");

// Example: Including and instantiating an MQ-135 gas sensor
// #include <MQ135Sensor.h>
// MQ135Sensor mq135Sensor = MQ135Sensor("MQ-135", 35);

// Add additional sensors below as needed, following the examples above.

// --- Add Sensors to the Manager ---
// Register each sensor with the SensorManager inside this function.
// The parameters 'throwOnInitializationError' and 'throwOnUpdateError' control
// whether exceptions are thrown on initialization or update failures.
// Adjust these flags based on the criticality of each sensor.

void addSensorsToManager() {
    // Example of adding sensors to the manager:
    // sensorManager.addSensor(&mpu6050_1, true, true);
    // sensorManager.addSensor(&mq135Sensor, true, true);

    // Add more sensors here, customizing error handling as needed.
    // This function is called during system setup to register all active sensors.
}

#endif