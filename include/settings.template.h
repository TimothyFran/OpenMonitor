#if false

// --- Instructions ---
// 1. Copy this template to 'settings.h'.
// 2. Remove the 'false' at the top to enable this configuration.
// 3. Edit the settings below as needed for your deployment.
// 4. Do not modify this template directly; always copy it to create your configuration.

#pragma once

// --- Timezone Configuration ---
// Set the timezone info for InfluxDB logging.
// Example: "UTC2" or "CET-1CEST,M3.5.0,M10.5.0/3"
#define INFLUXDB_TZ_INFO "UTC2"

// --- Sensor Entry Settings ---
// Maximum key length for sensor entries.
#define SENSORENTRY_MAX_KEY_LEN 8

// --- Error Message Settings ---
// Maximum length for error messages.
#define MAX_ERROR_MESSAGE_LEN 128

// --- Logging Interval ---
// Interval for logging data in milliseconds.
#define LOG_INTERVAL 5000 // 5 seconds

// --- Analog Microphone Sensor Settings ---
#define VREF_VALUE 3.3f
#define ANALOG_MIC_GAIN 75.0f
#define ANALOG_MIC_SAMPLING_DURATION 50UL // ms

#endif