#include <Arduino.h>
#include <ArduinoJson.h>

#include "components/valve.h"
#include "components/lights.h"

#include "comm_link.h"
#include "garden_cell.h"
#include "garden_system.h"

GardenSystem *CommLink::pg_s_;

void CommLink::Begin(int baud, GardenSystem *pg_s){
    Serial.begin(baud);
    pg_s_ = pg_s;
}

void CommLink::Update(){
  // Check serial buffer
  if(Serial.available()){
    StaticJsonBuffer<buf_size> jsonBuffer;
    JsonObject &root = jsonBuffer.parse(Serial);
    if (!root.success()) {
      Serial.println("parseObject() failed");
      return;
    }
    Interpret(root);
    jsonBuffer.~StaticJsonBuffer();
  }
}

void CommLink::Interpret(JsonObject &root){
  String kind = root["kind"];
  if(kind.equals("plant")){
    PlantOperation(root);
  }
  else if(kind.equals("cell")){
    CellOperation(root);
  }
  else if(kind.equals("system")){
    SystemOperation(root);
  }
  else if(kind.equals("handshake")){
    StaticJsonBuffer<buf_size> jsonBuffer;
    JsonObject& response = jsonBuffer.createObject();
    response["kind"] = "handshake";
    response.printTo(Serial);
    jsonBuffer.~StaticJsonBuffer();
  }
}

/* Further unpacks the JSON message recieved and either adds or removes the
 * plant sent.
 */
void CommLink::PlantOperation(JsonObject &root){
  String op = root["op"];
  uint8_t cell = root["cell"];
  uint8_t position = root["position"];

  if(op.equals("add")){
    String owner = root["owner"];
    float gal_per_period = root["gal"];
    uint8_t water_period = root["period"];
    // Construct new plant
    Plant p(owner, cell, position, gal_per_period, water_period);
    // Copy plant into requested cell and position
    pg_s_->get_cell(cell) += p;
  }
  else if(op.equals("remove")){
    // Remove the plant from the requested cell and position
    pg_s_->get_cell(cell) -= pg_s_->get_cell(cell).get_plant(position);
  }
}

/* Further unpacks the JSON message recieved and runs the requested command
 * on the requested cell.
 */
void CommLink::CellOperation(JsonObject &root){
  String op = root["op"];
  uint8_t cell = root["cell"];

  // Report status
  if(op.equals("status")){
    //TODO: add functionality
  }
  // Report all plants
  else if(op.equals("plants")){
    //TODO: add functionality
  }
  // Send sensor information
  else if(op.equals("sensors")){
    SendCellSensorInfo(pg_s_->get_cell(cell));
  }
  // Add a cell
  else if(op.equals("add")){
    uint8_t lights_pin = root["lpin"];
    uint8_t num_valves = root["num_valves"];
    uint8_t pins[num_valves];
    float flow_rate[num_valves];
    for(int i = 0; i < num_valves; i++){
      pins[i] = root["spins"][i];
      flow_rate[i] = root["flowrate"][i];
    }
    // Make dynamic lights
    Lights *plights = new Lights(lights_pin);
    // Make dynamic solenoid arrays
    ValveArray *pv_array = new ValveArray(num_valves, pins, flow_rate);
    // Make dynamic cell
    GardenCell *pg_c = new GardenCell(cell, pv_array, plights);
    (*pg_s_) += pg_c;
  }
  // Remove a cell
  else if(op.equals("remove")){
    (*pg_s_) -= pg_s_->get_cell_ptr(cell);
  }
  // Set the time that lights turn on in a cell
  else if(op.equals("setlighttime")){
    uint8_t hr = root["hr"];
    uint8_t mn = root["min"];
    pg_s_->get_cell(cell).set_lights_on_time(hr,mn);
  }
  // Set the time that lights shut off in a cell
  else if(op.equals("setdarktime")){
    uint8_t hr = root["hr"];
    uint8_t mn = root["min"];
    pg_s_->get_cell(cell).set_lights_off_time(hr,mn);
  }
  // Deactivate the cell
  else if(op.equals("deactivate")){
    pg_s_->get_cell(cell).Deactivate();
  }
  // Activate the cell
  else if(op.equals("activate")){
    pg_s_->get_cell(cell).Activate();
  }
}

void CommLink::SystemOperation(JsonObject &root){
  String op = root["op"];
}

/* Send the temperature and light sensor data from the GardenCell reference
 */
void CommLink::SendCellSensorInfo(const GardenCell &rg_c){
  StaticJsonBuffer<buf_size> jsonBuffer;
  JsonObject& response = jsonBuffer.createObject();
  response["kind"] = "cell";
  response["op"] = "sensors";
  response["cell"] = rg_c.get_cell_num();
  response["temp"] = rg_c.get_temp_val();
  response["light"] = rg_c.get_light_val();
  response.printTo(Serial);
  jsonBuffer.~StaticJsonBuffer();
}
