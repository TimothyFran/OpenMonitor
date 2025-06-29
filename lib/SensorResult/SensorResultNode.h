#pragma once
#include "SensorEntry.h"
#include <ArduinoLog.h>

struct SensorResultNode {
    SensorEntry entry;
    SensorResultNode* next;
    SensorResultNode* prev;

    SensorResultNode(const char* key, float value) : next(nullptr), prev(nullptr) {
        if (key != nullptr) {
            strncpy(entry.key, key, SENSORENTRY_MAX_KEY_LEN - 1);
            entry.key[SENSORENTRY_MAX_KEY_LEN - 1] = '\0';
        } else {
            entry.key[0] = '\0';
        }
        entry.value = value;      
    }
};
