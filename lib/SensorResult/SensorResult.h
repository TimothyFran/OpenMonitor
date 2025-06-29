#pragma once

#include <Arduino.h>
#include <stdexcept>
#include "../../include/ResultCode.h"
#include "../../include/SensorExceptions.h"
#include "SensorEntry.h"
#include "SensorResultNode.h"

class SensorResult {

private:
    SensorResultNode* head;
    SensorResultNode* tail;

    uint8_t count;
    const char* sensorName;

public:

    SensorResult(const char* sensorName = "Unknown") : head(nullptr), tail(nullptr), count(0), sensorName(sensorName) {}

    // Copy constructor
    SensorResult(const SensorResult& other) : head(nullptr), tail(nullptr), count(0), sensorName(other.sensorName) {
        copyFrom(other);
    }

    // Move constructor
    SensorResult(SensorResult&& other) noexcept : head(other.head), tail(other.tail), count(other.count), sensorName(other.sensorName) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
        other.sensorName = "Unknown";
    }

    // Copy assignment operator
    SensorResult& operator=(const SensorResult& other) {
        if (this != &other) {
            clear();
            sensorName = other.sensorName;
            copyFrom(other);
        }
        return *this;
    }

    // Move assignment operator
    SensorResult& operator=(SensorResult&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            count = other.count;
            sensorName = other.sensorName;
            
            other.head = nullptr;
            other.tail = nullptr;
            other.count = 0;
            other.sensorName = "Unknown";
        }
        return *this;
    }

    ~SensorResult() {
        clear();
    }

    void set(const char* key, float value);

    float getValue(const char* key) const;

    float getValue(uint8_t idx) const;

    const char* getKey(uint8_t idx) const;

    uint8_t countEntries() const;

    bool has(const char* key) const;

    void remove(const char* key);

    void clear();

    const char* getSensorName() const {
        return sensorName;
    }

    bool isEmpty() const { 
        return count == 0; 
    }

private:
    void copyFrom(const SensorResult& other) {
        SensorResultNode* node = other.head;
        while (node) {
            set(node->entry.key, node->entry.value);
            node = node->next;
        }
    }

};