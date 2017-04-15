#ifndef comm_link_h
#define comm_link_h

#include <Arduino.h>
// Include Arduino Json Interpreter
#include <ArduinoJson>

class CommLink {
public:
  CommLink()
    { Serial.begin(9600); }

  void Update();
  // Data outgoing
  void SendSensorData(Sensor s);
  // Data incoming

private:

};

extern CommLink Comm;
#endif
