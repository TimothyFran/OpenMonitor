#include <Arduino.h>
#include "ResultCode.h"

// Variabile globale per il messaggio dell'ultimo errore
char g_lastErrorMessage[MAX_ERROR_MESSAGE_LEN] = {0};

void setLastError(ResultCode code, const char* message) {
    if (message != nullptr) {
        strncpy(g_lastErrorMessage, message, MAX_ERROR_MESSAGE_LEN - 1);
        g_lastErrorMessage[MAX_ERROR_MESSAGE_LEN - 1] = '\0';
    } else {
        strncpy(g_lastErrorMessage, resultCodeToString(code), MAX_ERROR_MESSAGE_LEN - 1);
        g_lastErrorMessage[MAX_ERROR_MESSAGE_LEN - 1] = '\0';
    }
}

const char* getLastErrorMessage() {
    return g_lastErrorMessage;
}

const char* resultCodeToString(ResultCode code) {
    switch (code) {
        case ResultCode::OK:
            return "OK";
        case ResultCode::NULL_ARGUMENT_EXCEPTION:
            return "NULL_ARGUMENT_EXCEPTION";
        case ResultCode::INVALID_ARGUMENT_EXCEPTION:
            return "INVALID_ARGUMENT_EXCEPTION";
        case ResultCode::ARGUMENT_OUT_OF_RANGE_EXCEPTION:
            return "ARGUMENT_OUT_OF_RANGE_EXCEPTION";
        case ResultCode::OUT_OF_MEMORY_EXCEPTION:
            return "OUT_OF_MEMORY_EXCEPTION";
        case ResultCode::INDEX_OUT_OF_RANGE_EXCEPTION:
            return "INDEX_OUT_OF_RANGE_EXCEPTION";
        case ResultCode::INVALID_OPERATION_EXCEPTION:
            return "INVALID_OPERATION_EXCEPTION";
        case ResultCode::NOT_SUPPORTED_EXCEPTION:
            return "NOT_SUPPORTED_EXCEPTION";
        case ResultCode::KEY_NOT_FOUND_EXCEPTION:
            return "KEY_NOT_FOUND_EXCEPTION";
        case ResultCode::DUPLICATE_KEY_EXCEPTION:
            return "DUPLICATE_KEY_EXCEPTION";
        case ResultCode::OVERFLOW_EXCEPTION:
            return "OVERFLOW_EXCEPTION";
        case ResultCode::UNDERFLOW_EXCEPTION:
            return "UNDERFLOW_EXCEPTION";
        case ResultCode::DIVISION_BY_ZERO_EXCEPTION:
            return "DIVISION_BY_ZERO_EXCEPTION";
        case ResultCode::FORMAT_EXCEPTION:
            return "FORMAT_EXCEPTION";
        case ResultCode::TIMEOUT_EXCEPTION:
            return "TIMEOUT_EXCEPTION";
        case ResultCode::UNKNOWN_EXCEPTION:
        default:
            return "UNKNOWN_EXCEPTION";
    }
}

void clearLastError() {
    g_lastErrorMessage[0] = '\0';
}
