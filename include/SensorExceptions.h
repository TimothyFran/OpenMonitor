#pragma once

#include <stdexcept>
#include <Arduino.h>

/**
 * Eccezione base per errori del sensore
 */
class SensorException : public std::runtime_error {
public:
    explicit SensorException(const char* message) : std::runtime_error(message) {}
};

/**
 * Eccezione per sensore non inizializzato
 */
class SensorNotInitializedException : public SensorException {
public:
    SensorNotInitializedException() : SensorException("Sensor not initialized") {}
};

/**
 * Eccezione per fallimento di inizializzazione del sensore
 */
class SensorInitializationException : public SensorException {
public:
    explicit SensorInitializationException(const char* message) : SensorException(message) {}
};

/**
 * Eccezione per fallimento di lettura dei dati del sensore
 */
class SensorReadException : public SensorException {
public:
    explicit SensorReadException(const char* message) : SensorException(message) {}
};

/**
 * Eccezione per chiave non trovata nel SensorResult
 */
class KeyNotFoundException : public std::out_of_range {
public:
    explicit KeyNotFoundException(const char* key) : std::out_of_range((String("Key not found: ") + String(key)).c_str()) {}
};
