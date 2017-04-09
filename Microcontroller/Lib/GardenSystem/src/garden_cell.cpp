#include <Time.h>
#include <TimeAlarms.h>
#include "garden_cell.h"
#include "components/valve.h"
#include "components/lights.h"

GardenCell::GardenCell(uint8_t cell_num, ValveArray *pvalve_array, Lights *plights){
  _capacity = pvalve_array->get_size();
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
  delete [] _water_stop_times;
}

//======================== I N F O R M A T I O N A L =========================//

int GardenCell::get_capacity(){
  return _capacity;
}

int GardenCell::get_availability(){
  return _capacity - _num_plants;
}

Plant GardenCell::get_plant_at(uint8_t pos){
  return _plants[pos];
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

//===================== O P E R A T O R  O V E R L O A D =====================//

/* Copy plant to GardenCell object if there is room and schedule a watering time
 * for the new plant, also updates state variables.
 * Ex. garden_cell1 += some_plant;
 */
GardenCell &GardenCell::operator+=(const Plant &new_plant){
  uint8_t pos = new_plant.position;
  // If the requested position is empty, insert the new plant there
  if(this->_plants[pos].is_empty){
    this->_plants[pos] = new_plant;
    this->_num_plants ++;
    this->schedule();
  }
  return *this;
}

/* Removes the desired plant from the garden cell and updates state variables
 * Ex. garden_cell1 -= garden_cell1.get_plant_at(some_pos);
 */
GardenCell &GardenCell::operator-=(const Plant &plant){
  uint8_t pos = plant.position;
  this->_plants[pos].is_empty = true;;
  this->_num_plants --;
  return *this;
}
