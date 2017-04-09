#include <TimeAlarms.h>
#include "garden_cell.h"
#include "components/valve.h"
#include "components/lights.h"

GardenCell::GardenCell(uint8_t cell_num, ValveArray *pvalve_array, Lights *plights){
  _capacity = pvalve_array->get_size();
  _availability = _capacity;
  _cell_num = cell_num;
  // Assign Components
  _pvalve_array = pvalve_array;
  _plights = plights;
  _plants = new Plant[_capacity];
  _water_stop_times = new time_t[_capacity];
  _num_plants = 0;
}

GardenCell::~GardenCell(){
  delete [] _plants;
}

//======================== I N F O R M A T I O N A L =========================//

int GardenCell::get_capacity(){
  return _capacity;
}

int GardenCell::get_availability(){
  return _availability;
}

bool GardenCell::is_lighting(){
  return _plights->is_on();
}

bool GardenCell::is_watering(){
  return _watering;
}

//==================== C R I T I C A L  F U N C T I O N S ====================//

void GardenCell::water_plants(){
  //float flow_rate = _pvalve_array->get_flow_rate(i);
  //int duration = _plants[i].gal_per_period / flow_rate;
}

void GardenCell::activate_lights(){
  _plights->activate();
}

void GardenCell::deactivate_lights(){
  _plights->deactivate();
}
