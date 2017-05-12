#include <Time.h>
#include <TimeAlarms.h>
#include "garden_cell.h"
#include "components/valve.h"
#include "components/lights.h"
#include "components/sensors.h"

GardenCell::GardenCell(uint8_t cell_num, ValveArray *pvalve_array, Lights *plights){
  cell_num_ = cell_num;
  capacity_ = pvalve_array->get_size();
  // Assign Components
  pvalve_array_ = pvalve_array;
  plights_ = plights;
  // Allocate memory for plant pointers, fill with nullptrs
  pplants_ = new Plant*[capacity_];
  for(int i = 0; i < capacity_; i++)
    pplants_[i] = nullptr;
  // Allocate memory for water stop times
  water_stop_times_ = new time_t[capacity_];
}

GardenCell::~GardenCell(){
  // Delete each plant
  for(int i = 0; i < capacity_; i++){
    if(pplants_[i])
      delete pplants_[i];
  }
  // Deallocate memory
  delete [] pplants_;
  delete [] water_stop_times_;
  // Delete sensors
  if(has_light_sensor_)
    delete plight_sensor_;
  if(has_temp_sensor_)
    delete ptemp_sensor_;
}

//============================ A C C E S S O R S =============================//

uint8_t GardenCell::get_capacity() const{
  return capacity_;
}

uint8_t GardenCell::get_availability() const{
  return (capacity_ - num_plants_);
}

uint8_t GardenCell::get_num_plants() const{
  return num_plants_;
}

uint8_t GardenCell::get_cell_num() const{
  return cell_num_;
}

Plant *GardenCell::get_plant(uint8_t pos) const{
  return pplants_[pos];
}

double GardenCell::get_temp_val() const{
  if(has_temp_sensor_){
    return ptemp_sensor_->get_val();
  }
  return 0.0;
}

double GardenCell::get_light_val() const{
  if(has_light_sensor_){
    return plight_sensor_->get_val();
  }
  return 0.0;
}

bool GardenCell::has_temp_sensor(){
  return has_temp_sensor_;
}

bool GardenCell::has_light_sensor(){
  return has_light_sensor_;
}

bool GardenCell::is_active() const{
  return is_active_;
}

bool GardenCell::is_lighting() const{
  return plights_->is_active();
}

bool GardenCell::is_watering() const{
  return watering_;
}

void GardenCell::set_lights_on_time(uint8_t hr, uint8_t mn){
  lights_start_tm_[0] = hr;
  lights_start_tm_[1] = mn;
}

void GardenCell::set_lights_off_time(uint8_t hr, uint8_t mn){
  lights_stop_tm_[0] = hr;
  lights_stop_tm_[1] = mn;
}

void GardenCell::set_temp_sensor(TempSensor *pt_s){
  ptemp_sensor_ = pt_s;
  has_temp_sensor_ = true;
}

void GardenCell::set_light_sensor(LightSensor *pl_s){
  plight_sensor_ = pl_s;
  has_light_sensor_ = true;
}

//==================== C R I T I C A L  F U N C T I O N S ====================//

/* Updates the state of the lights and the solenoids based on when they are
 * scheduled to be activated/deactivated or opened/closed.
 */
void GardenCell::Update(){
  if(plights_->is_active() && hour() >= lights_stop_tm_[0] && minute() >= lights_stop_tm_[1])
    DeactivateLights();
  else if(!plights_->is_active() && hour() >= lights_start_tm_[0] && minute() >= lights_start_tm_[1])
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
  // TODO: fill in
}

//=================== E M E R G E N C Y  F U N C T I O N S ===================//

/*
 */
void GardenCell::Deactivate(){
  is_active_ = false;
  // TODO: Send event message
}

/*
 */
void GardenCell::Activate(){
  is_active_ = true;
  // TODO: Send event message
}

//===================== O P E R A T O R  O V E R L O A D =====================//

/* Copy plant to GardenCell object if there is room and schedule a watering time
 * for the new plant, also updates state variables.
 * Ex. garden_cell1 += some_plant;
 */
GardenCell &GardenCell::operator+=(Plant *pplant){
  uint8_t pos = pplant->position;
  // If the requested position is empty, insert the new plant there
  if(this->pplants_[pos] == nullptr){
    this->pplants_[pos] = pplant;
    this->num_plants_ ++;
    this->Schedule();
  }
  return *this;
}

/* Removes the desired plant from the garden cell and updates state variables
 * Ex. garden_cell1 -= garden_cell1.get_plant(some_pos);
 */
GardenCell &GardenCell::operator-=(Plant *pplant){
  if(pplant == nullptr)
    return *this;
  uint8_t pos = pplant->position;
  this->pplants_[pos] = nullptr;
  this->num_plants_ --;
  return *this;
}
