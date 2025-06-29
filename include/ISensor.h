#pragma once

#include <Arduino.h>
#include "SensorExceptions.h"
#include "ResultCode.h"
#include "SensorResult.h"

class ISensor {

    protected:
        const char* sensorName;
        const unsigned long updateInterval;
        unsigned long lastReadTime = 0;

    public:

        /**
         * Constructor for the ISensor interface.
         * @param name Name of the sensor.
         * @param interval Update interval for the sensor.
         */
        ISensor(const char* name, unsigned long interval) : sensorName(name), updateInterval(interval) {}

        /**
         * Virtual destructor for the ISensor interface.
         * Ensures proper cleanup of derived classes.
         */
        virtual ~ISensor() = default;

        /**
         * Used to initialize the sensor.
         * @throws SensorInitializationException if initialization fails.
         */
        virtual void begin() = 0;

        /**
         * Used to update the sensor data. Will be called periodically by the main loop.
         * May not be used by all sensors, depending on their implementation.
         * @throws SensorException if update fails.
         */
        virtual void update() = 0;

        /**
         * Used to read all sensor values into a SensorResult container.
         * @return SensorResult containing all sensor readings.
         * @throws SensorNotInitializedException if sensor is not initialized.
         * @throws SensorReadException if reading fails.
         */
        virtual SensorResult readValues(bool force = false, bool updateReadTime = true) = 0;

        /**
         * Get the name of the sensor.
         * @return Name of the sensor.
         */
        const char* getSensorName() const {
            return sensorName;
        }

};