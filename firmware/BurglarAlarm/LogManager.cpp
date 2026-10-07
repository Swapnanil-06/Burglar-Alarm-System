#include "LogManager.h"

LogManager::LogManager() : eventCount(0), previousEventID(0), authCount(0) {
}

void LogManager::record(String event) {
    if (eventCount < MAX_LOG_ENTRIES) {
        // SR 2.3.3 — store timestamp and event type, never the PIN
        eventLog[eventCount] = String(millis()) + "ms | " + event;
        previousEventID = eventCount;
        eventCount++;
    }
    // when full, oldest entries are silently dropped
    // extend to EEPROM or SD card for persistence (SR 2.3.2)
}

void LogManager::recordAuth(bool outcome, String method) {
    if (authCount < MAX_LOG_ENTRIES) {
        authLog[authCount].timestamp = millis();   // <-- FIXED
        authLog[authCount].outcome   = outcome;
        authLog[authCount].method    = method;
        authCount++;
    }

    String entry = "AUTH | method:" + method + " | result:";
    entry += outcome ? "success" : "fail";
    record(entry);
}

String LogManager::recall(int eventID) {
    if (eventID >= 0 && eventID < eventCount) {
        return eventLog[eventID];
    }
    return "no entry";
}

void LogManager::printLog() {
    Serial.println("--- Event Log ---");
    for (int i = 0; i < eventCount; i++) {
        Serial.println(eventLog[i]);
    }
    Serial.println("-----------------");
}

int LogManager::getEventCount() {
    return eventCount;
}