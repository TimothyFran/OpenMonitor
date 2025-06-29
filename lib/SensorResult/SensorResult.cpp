#include "SensorResult.h"
#include <ArduinoLog.h>

void SensorResult::set(const char* key, float value) {
    Log.verboseln(F("SensorResult::set() - Setting value for key: '%s' to %F"), key, value);

    if (key == nullptr) {
        throw std::invalid_argument("Key cannot be null");
    }
    if (strlen(key) == 0) {
        throw std::invalid_argument("Key cannot be empty");
    }
    if (strlen(key) >= SENSORENTRY_MAX_KEY_LEN) {
        throw std::out_of_range("Key too long");
    }

    SensorResultNode* node = head;
    while (node) {
        if (strcmp(node->entry.key, key) == 0) {
            Log.verboseln(F("SensorResult::set() - Key '%s' found, updating value to %F"), key, value);
            node->entry.value = value;
            return;
        }
        node = node->next;
    }

    Log.verboseln(F("SensorResult::set() - Key '%s' not found, creating new node"), key);

    SensorResultNode* newNode = new SensorResultNode(key, value);
    
    newNode->prev = tail;

    if (tail) {
        tail->next = newNode;
    } else {
        head = newNode; // If tail is null, this is the first node
    }
    
    tail = newNode; // Update tail to point to the new node
    count++;
    Log.verboseln(F("SensorResult::set() - New node created with key '%s' and value %F. Updated nodes count: %d"), key, value, count);
}

float SensorResult::getValue(const char* key) const {
    Log.verboseln(F("SensorResult::getValue() - Getting value for key: '%s'"), key);

    if (key == nullptr) {
        throw std::invalid_argument("Key cannot be null");
    }
    if (strlen(key) == 0) {
        throw std::invalid_argument("Key cannot be empty");
    }
    
    SensorResultNode* node = head;
    while (node) {
        if (strcmp(node->entry.key, key) == 0) {
            return node->entry.value;
        }
        node = node->next;
    }

    Log.errorln(F("SensorResult::getValue() - Key '%s' not found"), key);

    throw KeyNotFoundException(key);
}

float SensorResult::getValue(uint8_t idx) const {
    Log.verboseln(F("SensorResult::getValue() - Getting value at index: %d, count: %d"), idx, count);

    if(idx >= count) {
        throw std::out_of_range("Index out of range");
    }
    
    SensorResultNode* node = head;
    for (uint8_t i = 0; i < idx && node; i++) {
        node = node->next;
    }
    if (node) {
        return node->entry.value;
    }

    Log.errorln(F("SensorResult::getValue() - Element at index %d not found (count: %d)"), idx, count);

    throw std::out_of_range("Index out of range");
}

uint8_t SensorResult::countEntries() const {
    return count;
}

bool SensorResult::has(const char* key) const {
    Log.traceln(F("SensorResult::has() - Checking if key '%s' exists"), key);

    if (key == nullptr) {
        throw std::invalid_argument("Key cannot be null");
    }
    if (strlen(key) == 0) {
        throw std::invalid_argument("Key cannot be empty");
    }
    
    SensorResultNode* node = head;
    while (node) {
        if (strcmp(node->entry.key, key) == 0) {
            return true;
        }
        node = node->next;
    }
    return false;
}

void SensorResult::remove(const char* key) {
    Log.traceln(F("SensorResult::remove() - Removing sensor result node with key: '%s'"), key);

    if (key == nullptr) {
        throw std::invalid_argument("Key cannot be null");
    }
    if (strlen(key) == 0) {
        throw std::invalid_argument("Key cannot be empty");
    }
    
    SensorResultNode* node = head;
    
    while (node) {
        if (strcmp(node->entry.key, key) == 0) {
            // Update previous node's next pointer
            if (node->prev) {
                node->prev->next = node->next;
            } else {
                head = node->next; // This was the head node
            }
            
            // Update next node's prev pointer
            if (node->next) {
                node->next->prev = node->prev;
            } else {
                tail = node->prev; // This was the tail node
            }
            
            delete node;
            count--;
            return;
        }
        node = node->next;
    }

    throw KeyNotFoundException(key);
}

void SensorResult::clear() {
    Log.verboseln(F("SensorResult::clear() - Clearing all sensor result nodes for sensor: '%s'"), sensorName);

    SensorResultNode* node = head;
    while (node) {
        SensorResultNode* nextNode = node->next;
        delete node;
        node = nextNode;
    }

    head = nullptr;
    tail = nullptr;
    count = 0;
    Log.verboseln(F("SensorResult::clear() - All nodes cleared"));
}

const char* SensorResult::getKey(uint8_t idx) const {
    Log.verboseln(F("SensorResult::getKey() - Getting key at index: %d, count: %d"), idx, count);

    if (idx >= count) {
        throw std::out_of_range("Index out of range");
    }
    
    SensorResultNode* node = head;
    for (uint8_t i = 0; i < idx && node; i++) {
        node = node->next;
    }
    if (node) {
        Log.verboseln(F("SensorResult::getKey() - Found key: '%s' at index: %d"), node->entry.key, idx);
        return node->entry.key;
    }

    Log.errorln(F("SensorResult::getKey() - Element at index %d not found (count: %d)"), idx, count);

    throw std::out_of_range("Index out of range");
}