#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <Arduino.h>

// ===== REQUIRED DEFINITIONS (must come BEFORE usage) =====
const int MAX_LOG_ENTRIES = 50;

struct AuthenticationAttempt {
    bool outcome;
    String method;
    unsigned long timestamp;
};

// ===== CLASS =====
class LogManager {
  private:
    String eventLog[MAX_LOG_ENTRIES];
    int    eventCount;
    int    previousEventID;

    AuthenticationAttempt authLog[MAX_LOG_ENTRIES];
    int                   authCount;

  public:
    LogManager();

    void record(String event);
    void recordAuth(bool outcome, String method);
    String recall(int eventID);
    void printLog();
    int getEventCount();
};

#endif