#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <Arduino.h>

class DisplayManager {
  private:
    int    bufferSize;
    String buffer;
    long   lastActivityTime;
    bool   displayOn;

  public:
    DisplayManager();

    void setbuffer(String inputString);
    String getbuffer();
    int getbuffersize();
    void clearbuffer();
    void update();
};

#endif