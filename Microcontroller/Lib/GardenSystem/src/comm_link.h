#ifndef comm_link_h
#define comm_link_h

#include <Arduino.h>
// Include Arduino Json Interpreter
#include <ArduinoJson.h>

#include "garden_cell.h"
#include "garden_system.h"

class CommLink {
public:
  static void Begin(int baud, GardenSystem *pg_s);
  static void Update();

  // Data outgoing
  static void Error(String err_mes);
  static void SendCellSensorInfo(const GardenCell &rg_c);
  // Data incoming
  static void Interpret(JsonObject &root);

private:
  static const uint16_t buf_size = 120;
  static GardenSystem *pg_s_;
  static void PlantOperation(JsonObject &root);
  static void CellOperation(JsonObject &root);
  static void SystemOperation(JsonObject &root);

};

#endif
//{"kind":"cell","op":"remove","cell":4}{"kind":"plant","op":"remove","cell":3,"position":2}
//{"kind":"cell","op":"add","num_valves":6,"spins":[2,3,4,5,6,7],"flowrate":[1.0,0.5,0.5,1.0,0.5,0.5],"lpin":8,"cell":3}
