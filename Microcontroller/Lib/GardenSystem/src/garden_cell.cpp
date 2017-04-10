#include <Time.h>
#include <TimeAlarms.h>
#include "garden_cell.h"
#include "components/valve.h"
#include "components/lights.h"

GardenCell::GardenCell(uint8_t cell_num, ValveArray *pvalve_array, Lights *plights){
  capacity_ = pvalve_array->get_size();
  cell_num_ = cell_num;
  // Assign Components
  pvalve_array_ = pvalve_array;
  plights_ = plights;
  plants_ = new Plant[capacity_];
  water_stop_times_ = new time_t[capacity_];
  numplants_ = 0;
}

GardenCell::~GardenCell(){
  delete [] plants_;
  delete [] water_stop_times_;
}

//============================ A C C E S S O R S =============================//

int GardenCell::get_capacity(){
  return capacity_;
}

int GardenCell::get_availability(){
  return capacity_ - numplants_;
}

Plant GardenCell::get_plant_at(uint8_t pos){
  return plants_[pos];
}

bool GardenCell::is_lighting(){
  return plights_->is_on();
}

bool GardenCell::is_watering(){
  return watering_;
}

void GardenCell::set_lights_on_time(uint8_t hr, uint8_t mn){
  lights_start_hr_ = hr;
  lights_start_mn_ = mn;
}

void GardenCell::set_lights_off_time(uint8_t hr, uint8_t mn){
  lights_stop_hr_ = hr;
  lights_stop_mn_ = mn;
}

//==================== C R I T I C A L  F U N C T I O N S ====================//

/* Updates the state of the lights and the solenoids based on when they are
 * scheduled to be activated/deactivated or opened/closed.
 */
void GardenCell::Update(){
  if(plights_->is_on() && hour() >= lights_stop_hr_ && minute() >= lights_stop_mn_)
    DeactivateLights();
  else if(!plights_->is_on() && hour() >= lights_start_hr_ && minute() >= lights_start_mn_)
    ActivateLights();
  // TODO: add solenoid valve update functionality
}

void GardenCell::WaterPlants(){
  //float flow_rate = pvalve_array_->get_flow_rate(i);
  //int duration = plants_[i].gal_per_period / flow_rate;
}

void GardenCell::ActivateLights(){
  plights_->Activate();
}

void GardenCell::DeactivateLights(){
  plights_->Deactivate();
}

void GardenCell::Schedule(){

}

//===================== O P E R A T O R  O V E R L O A D =====================//

/* Copy plant to GardenCell object if there is room and schedule a watering time
 * for the new plant, also updates state variables.
 * Ex. garden_cell1 += some_plant;
 */
GardenCell &GardenCell::operator+=(const Plant &new_plant){
  uint8_t pos = new_plant.position;
  // If the requested position is empty, insert the new plant there
  if(this->plants_[pos].is_empty){
    this->plants_[pos] = new_plant;
    this->numplants_ ++;
    this->Schedule();
  }
  return *this;
}

/* Removes the desired plant from the garden cell and updates state variables
 * Ex. garden_cell1 -= garden_cell1.get_plant_at(some_pos);
 */
GardenCell &GardenCell::operator-=(const Plant &plant){
  uint8_t pos = plant.position;
  this->plants_[pos].is_empty = true;;
  this->numplants_ --;
  return *this;
}
