#include <Time.h>
#include <TimeAlarms.h>
#include "garden_cell.h"
#include "components/valve.h"
#include "components/lights.h"
#include "components/sensors.h"
#include "components/fan.h"

GardenCell::GardenCell(uint8_t cell_num, ValveArray *pvalve_array, Lights *plights){
  cell_num_ = cell_num;
  capacity_ = pvalve_array->get_size();
  // Assign Components
  pvalve_array_ = pvalve_array;
  plights_ = plights;
  lights_on_ = plights->is_active();
  // Allocate memory for plant pointers, fill with nullptrs
  pplants_ = new Plant*[capacity_];
  // Allocate memory for water start/stop times
  water_start_times_ = new time_t[capacity_];
  water_stop_times_ = new time_t[capacity_];
  for(int i = 0; i < capacity_; i++){
    pplants_[i] = nullptr;
    water_start_times_[i] = 0;
    water_stop_times_[i] = 0;
  }
}

GardenCell::GardenCell(uint8_t cell_num, ValveArray *pvalve_array, Lights *plights,
  Fan *pfan_in, Fan *pfan_out){
    cell_num_ = cell_num;
    capacity_ = pvalve_array->get_size();
    // Assign Components
    pvalve_array_ = pvalve_array;
    plights_ = plights;
    lights_on_ = plights->is_active();
    pfan_in_ = pfan_in;
    pfan_out_ = pfan_out;
    has_fans_ = true;
    // Allocate memory for plant pointers, fill with nullptrs
    pplants_ = new Plant*[capacity_];
    // Allocate memory for water start/stop times
    water_start_times_ = new time_t[capacity_];
    water_stop_times_ = new time_t[capacity_];
    for(int i = 0; i < capacity_; i++){
      pplants_[i] = nullptr;
      water_start_times_[i] = 0;
      water_stop_times_[i] = 0;
    }
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

time_t GardenCell::get_plant_water_start_time(uint8_t pos) const{
  return water_start_times_[pos];
}

time_t GardenCell::get_plant_water_stop_time(uint8_t pos) const{
  return water_stop_times_[pos];
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

bool GardenCell::has_fans(){
  return has_fans_;
}

bool GardenCell::is_fanning() const {
  return fans_on_;
}

bool GardenCell::is_active() const{
  return is_active_;
}

bool GardenCell::is_lighting() const{
  return plights_->is_active();
}

bool GardenCell::needs_water() const{
  return needs_water_;
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

void GardenCell::set_fan_preset(FanPreset fan_preset){
  fan_preset_ = fan_preset;
}

//==================== C R I T I C A L  F U N C T I O N S ====================//

/* Updates the state of the lights and the solenoids based on when they are
 * scheduled to be activated/deactivated or opened/closed.
 */
void GardenCell::Update(){
  /* if the lights are on, and it is past time to shut the lights off
   * turn the lights off */
  if(is_night() && lights_on_){
    DeactivateLights();
  }
  /* if the lights aren't on, it is past time to turn the lights on, and not
   * yet late enough to turn the lights off, turn the lights on */
  else if(!(lights_on_) && is_day()){
    ActivateLights();
  }

  /* Checks to see if any of the solenoids should be open, if so it opens them
   * and sets still_watering true.
   * Reschedules watering times if needed. */
  bool still_watering = false;
  for(int i = 0; i < capacity_; i++){
    // Make sure we have a plant in the position
    if(pplants_[i]){
      // If the plant is scheduled to stop earlier than now
      if(water_stop_times_[i] <= now()){
        Schedule(pplants_[i]);
        pvalve_array_->CloseValve(i);
      }
      // If the plant is scheduled to start earlier than now
      else if(water_start_times_[i] <= now()){
        still_watering = true;
        pvalve_array_->OpenValve(i);
      }
    }
  }
  /* Checks if the cell owns any fans, and if so, updates their state based on
   * the fan preset*/
  if(has_fans_){
    switch (fan_preset_) {
      case ALWAYS_ON:
        if(!fans_on_){
          pfan_in_->Activate();
          pfan_out_->Activate();
          fans_on_ = true;
        }
        break;
      case ALL_DAY:
        if(is_day() && !fans_on_){
          pfan_in_->Activate();
          pfan_out_->Activate();
          fans_on_ = true;
        }
        else if(is_night() && fans_on_){
          pfan_in_->Deactivate();
          pfan_out_->Deactivate();
          fans_on_ = false;
        }
        break;
      case ALL_NIGHT:
        if(is_day() && fans_on_){
          pfan_in_->Deactivate();
          pfan_out_->Deactivate();
          fans_on_ = false;
        }
        else if(is_night() && !fans_on_){
          pfan_in_->Activate();
          pfan_out_->Activate();
          fans_on_ = true;
        }
        break;
      case DAILY:
        break;
      case NIGHTLY:
        break;
      case MANUAL:
        break;
    }
  }
  needs_water_ = still_watering;
}

void GardenCell::ActivateLights(){
  plights_->Activate();
  lights_on_ = true;
}

void GardenCell::DeactivateLights(){
  plights_->Deactivate();
  lights_on_ = false;
}

void GardenCell::Schedule(Plant *pplant){
  // TODO: fill in
  uint8_t pos = pplant->position;
  float rate = pvalve_array_->get_flow_rate(pos); // Gal/Hr
  long duration = (pplant->gal_per_period) * rate * 60 * 60; // In seconds

  // tmp = right now + water_period days, final units in seconds
  time_t tmp = now() + pplant->water_period * 24 * 60 * 60;
  tmElements_t tmp_tm;
  breakTime(tmp, tmp_tm);
  tmp_tm.Second = 0;
  tmp_tm.Minute = lights_start_tm_[1];
  tmp_tm.Hour = lights_start_tm_[0];
  tmp = makeTime(tmp_tm);
  water_start_times_[pos] = tmp; // Water start time
  water_stop_times_[pos] = tmp + duration; // Water stop time
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
    Schedule(pplant);
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

//===================== P R I V A T E  F U N C T I O N S =====================//

bool GardenCell::is_day(){
  tmElements_t dusk; breakTime(now(), dusk);
  dusk.Hour = lights_stop_tm_[0];
  dusk.Minute = lights_stop_tm_[1];
  dusk.Second = 0;
  tmElements_t dawn; breakTime(now(), dawn);
  dawn.Hour = lights_start_tm_[0];
  dawn.Minute = lights_start_tm_[1];
  dawn.Second = 0;
  time_t dawn_tm = makeTime(dawn);
  time_t dusk_tm = makeTime(dusk);

  return (now() >= dawn_tm && now() < dusk_tm);
}

bool GardenCell::is_night(){
  tmElements_t dusk; breakTime(now(), dusk);
  dusk.Hour = lights_stop_tm_[0];
  dusk.Minute = lights_stop_tm_[1];
  dusk.Second = 0;
  tmElements_t dawn; breakTime(now(), dawn);
  dawn.Hour = lights_start_tm_[0];
  dawn.Minute = lights_start_tm_[1];
  dawn.Second = 0;
  time_t dawn_tm = makeTime(dawn);
  time_t dusk_tm = makeTime(dusk);

  return (now() < dawn_tm || now() >= dusk_tm);
}
