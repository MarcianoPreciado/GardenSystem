#ifndef garden_cell_h
#define garden_cell_h
#include <Arduino.h>
#include <Time.h>
#include "components/valve.h"
#include "components/lights.h"
#include "components/sensors.h"


 /* Plant struct
 * @Params
 * String owner;          The name of the owner of the plant
 * int cell;              The garden cell number it is located in (zero-based)
 * int location;          Location in garden cell
 * float gal_per_period;  Water need
 * int water_period;      How many days between watering cycles
 */
struct Plant{
  Plant() : owner("NA"), is_empty(true){}
  Plant(String own, uint8_t cell_num, uint8_t pos, float gal, float period) :is_empty(false)
  { owner = own; cell = cell_num; position = pos; gal_per_period = gal; water_period = period;}
  Plant(const Plant &p){
    this->owner = p.owner;
    this->cell = p.cell;
    this->position = p.position;
    this->gal_per_period = p.gal_per_period;
    this->water_period = p.water_period;
    this->is_empty = p.is_empty;
    this->is_scheduled = p.is_scheduled;
    this->schedule = p.schedule;
  }
  String owner;
  uint8_t cell;
  uint8_t position;
  float gal_per_period;
  float water_period;

  bool is_empty;
  bool is_scheduled = false;
  time_t schedule = -1;
};

/* GardenCell Class
 * Keeps track of
 * TODO:
 */
class GardenCell{
public:
  // Constructor/Destructor
  GardenCell(uint8_t cell_num, ValveArray *valve_array, Lights *lights);
  ~GardenCell();

  // Accessors
  uint8_t get_capacity() const;
  uint8_t get_availability() const;
  uint8_t get_num_plants() const;
  uint8_t get_cell_num() const;
  Plant *get_plant(uint8_t pos) const;
  double get_temp_val() const;
  double get_light_val() const;

  bool has_temp_sensor();
  bool has_light_sensor();

  bool is_lighting() const;
  bool is_watering() const;
  bool is_active() const;

  void set_lights_on_time(uint8_t hr, uint8_t mn);
  void set_lights_off_time(uint8_t hr, uint8_t mn);
  void set_temp_sensor(TempSensor *pt_s);
  void set_light_sensor(LightSensor *pl_s);


  // Critical Functions
  void Update();
  void WaterPlants();
  void ActivateLights();
  void DeactivateLights();

  // Emergency Functions
  void Deactivate();
  void Activate();

  // Operator Overloading
  GardenCell & operator+=(Plant *pplant);
  GardenCell & operator-=(Plant *pplant);
private:
  // Pointers to components
  ValveArray *pvalve_array_;
  Lights *plights_;
  TempSensor *ptemp_sensor_;
  LightSensor *plight_sensor_;

  // Dynamic arrays (use operator new)
  Plant **pplants_;
  time_t *water_stop_times_;

  bool lights_on_ = false;
  bool watering_ = false;
  bool has_light_sensor_ = false;
  bool has_temp_sensor_ = false;

  uint8_t cell_num_;
  uint8_t capacity_;
  uint8_t num_plants_ = 0;
  bool is_active_ = true;

  // Default lighting start time is 7:30 AM
  uint8_t lights_start_tm_[2] = {7, 30};
  // Default lighting stop time is 9:00 PM
  uint8_t lights_stop_tm_[2] = {21, 0};

  void Schedule();
};

#endif
