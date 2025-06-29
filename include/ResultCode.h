#pragma once

#include "settings.h"

/**
 * Enum dei codici di risultato basati su C# .NET
 */
enum class ResultCode {
    OK = 0,
    NULL_ARGUMENT_EXCEPTION,
    INVALID_ARGUMENT_EXCEPTION,
    ARGUMENT_OUT_OF_RANGE_EXCEPTION,
    OUT_OF_MEMORY_EXCEPTION,
    INDEX_OUT_OF_RANGE_EXCEPTION,
    INVALID_OPERATION_EXCEPTION,
    NOT_SUPPORTED_EXCEPTION,
    KEY_NOT_FOUND_EXCEPTION,
    DUPLICATE_KEY_EXCEPTION,
    OVERFLOW_EXCEPTION,
    UNDERFLOW_EXCEPTION,
    DIVISION_BY_ZERO_EXCEPTION,
    FORMAT_EXCEPTION,
    TIMEOUT_EXCEPTION,
    UNKNOWN_EXCEPTION
};

/**
 * Struttura per incapsulare il risultato di un'operazione
 */
template<typename T>
struct Result {
    ResultCode code;
    T value;
    
    Result() : code(ResultCode::OK), value{} {}
    Result(ResultCode c) : code(c), value{} {}
    Result(ResultCode c, const T& v) : code(c), value(v) {}
    
    bool isSuccess() const { return code == ResultCode::OK; }
    bool isError() const { return code != ResultCode::OK; }
    
    operator bool() const { return isSuccess(); }
};

/**
 * Specializzazione per operazioni che non restituiscono un valore
 */
template<>
struct Result<void> {
    ResultCode code;
    
    Result() : code(ResultCode::OK) {}
    Result(ResultCode c) : code(c) {}
    
    bool isSuccess() const { return code == ResultCode::OK; }
    bool isError() const { return code != ResultCode::OK; }
    
    operator bool() const { return isSuccess(); }
};

// Variabile globale per il messaggio dell'ultimo errore
extern char g_lastErrorMessage[MAX_ERROR_MESSAGE_LEN];

/**
 * Funzioni di utilità per la gestione degli errori
 */
void setLastError(ResultCode code, const char* message = nullptr);
const char* getLastErrorMessage();
const char* resultCodeToString(ResultCode code);
void clearLastError();
