
#include "InfluxLogger.h"
#include <WiFi.h>
#include "SensorResult.h"

InfluxLogger::InfluxLogger(const char* deviceName, bool simulated) : deviceName(deviceName), simulated(simulated) {
    if (simulated) {
        Log.notice(F("InfluxLogger initialized in simulated mode. No data will be sent to InfluxDB."));
    } else {
        Log.notice(F("InfluxLogger initialized for device: %s"), deviceName);
    }
}

void InfluxLogger::begin() {
    if (simulated) {
        Log.noticeln(F("InfluxLogger is in simulated mode, skipping InfluxDB connection."));
        return;
    }

    //client = new InfluxDBClient(SECRET_INFLUXDB_HOST, SECRET_INFLUXDB_ORG, SECRET_INFLUXDB_BUCKET, SECRET_INFLUXDB_TOKEN, InfluxDbCloud2CACert);
    client.setConnectionParams(SECRET_INFLUXDB_HOST, SECRET_INFLUXDB_ORG, SECRET_INFLUXDB_BUCKET, SECRET_INFLUXDB_TOKEN, InfluxDbCloud2CACert);
    client.setInsecure();

    // Time sync for certificate validation
    //timeSync(INFLUXDB_TZ_INFO, "pool.ntp.org", "time.nis.gov");
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print(F("Waiting for NTP time sync: "));
    time_t nowSecs = time(nullptr);
    while (nowSecs < 8 * 3600 * 2) {
        delay(500);
        Serial.print(F("."));
        yield();
        nowSecs = time(nullptr);
    }

    Serial.println();
    struct tm timeinfo;
    gmtime_r(&nowSecs, &timeinfo);
    Serial.print(F("Current time: "));
    Serial.print(asctime(&timeinfo));

    Log.noticeln(F("Connecting to InfluxDB at %s..."), client.getServerUrl().c_str());
    if (client.validateConnection()) {
        Log.noticeln(F("Connected to InfluxDB: %s"), client.getServerUrl().c_str());
    } else {
        Log.error(F("InfluxDB connection failed: %s"), client.getLastErrorMessage().c_str());
        while(1) {}
    }
    client.setWriteOptions(WriteOptions().writePrecision(WritePrecision::MS).batchSize(500).bufferSize(100));
}

void InfluxLogger::logSensorResult(const SensorResult& result) {
    if (simulated) {
        Log.noticeln(F("Simulated logging\tDevice: %s, Sensor: %s"), deviceName, result.getSensorName());
        return;
    }

    Point point(result.getSensorName());
    point.addTag("device", deviceName);
    for (uint8_t i = 0; i < result.countEntries(); i++) {
        point.addField(result.getKey(i), result.getValue(i));
    }
    //point.setTime(time(nullptr));

    uint16_t pointLen = client.pointToLineProtocol(point).length();
    Log.verboseln(F("Logging point to InfluxDB, Length: %d bytes"), pointLen);
    if(pointLen >= 141000)
        Log.warningln(F("Point length exceeds warning threshold, this may cause issues."));

    client.writePoint(point);
}

void InfluxLogger::flush() {
    if (simulated) {
        Log.notice(F("Simulated flush, no data sent to InfluxDB."));
        return;
    }

    if (!client.flushBuffer()) {
        Serial.print("InfluxDB flush failed: ");
        Serial.println(client.getLastErrorMessage());
        Serial.print("Full buffer: ");
        Serial.println(client.isBufferFull() ? "Yes" : "No");
    }
}
