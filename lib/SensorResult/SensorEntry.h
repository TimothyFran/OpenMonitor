#pragma once

#include <Arduino.h>
#include "settings.h"
#include "../../include/ResultCode.h"

struct SensorEntry {
    char key[SENSORENTRY_MAX_KEY_LEN];
    float value;
};