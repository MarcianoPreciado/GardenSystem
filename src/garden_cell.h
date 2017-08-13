#ifndef garden_cell_h
#define garden_cell_h
#include <Arduino.h>
#include <Time.h>
#include "components/valve.h"
#include "components/lights.h"
#include "components/sensors.h"
#include "components/fan.h"

 /* Plant struct
 * @Params
 * String owner;          The name of the owner of the plant
 * int cell;              The garden cell number it is located in (zero-based)
 * int location;          Location in garden cell
 * float gal_per_period;  Water need
 * int water_period;      How many days between watering cycles
 */
struct Plant{
  Plant() : owner("NA"){}
  Plant(String own, uint8_t pos, float gal, float period)
  { owner = own; position = pos; gal_per_period = gal; water_period = period;}
  Plant(const Plant &p){
    this->owner = p.owner;
    this->position = p.position;
    this->gal_per_period = p.gal_per_period;
    this->water_period = p.water_period;
    this->is_scheduled = p.is_scheduled;
    this->schedule = p.schedule;
  }
  String owner;
  uint8_t position;
  float gal_per_period;
  float water_period;
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
  GardenCell(uint8_t cell_num, ValveArray *pvalve_array, Lights *plights);
  GardenCell(uint8_t cell_num, ValveArray *pvalve_array, Lights *plights,
    Fan *pfan_in, Fan *pfan_out);
  ~GardenCell();

  // Enumerations
  enum FanPreset { MANUAL, DAILY, NIGHTLY, ALWAYS_ON, ALL_DAY, ALL_NIGHT};

  // Accessors
  uint8_t get_capacity() const;
  uint8_t get_availability() const;
  uint8_t get_num_plants() const;
  uint8_t get_cell_num() const;
  Plant *get_plant(uint8_t pos) const;
  time_t get_plant_water_start_time(uint8_t pos) const;
  time_t get_plant_water_stop_time(uint8_t pos) const;
  double get_temp_val() const;
  double get_light_val() const;

  bool has_temp_sensor();
  bool has_light_sensor();
  bool has_fans();

  bool is_fanning() const;
  bool is_lighting() const;
  bool needs_water() const;
  bool is_active() const;

  void set_lights_on_time(uint8_t hr, uint8_t mn);
  void set_lights_off_time(uint8_t hr, uint8_t mn);
  void set_temp_sensor(TempSensor *pt_s);
  void set_light_sensor(LightSensor *pl_s);
  void set_fan_preset(FanPreset fan_preset);

  // Critical Functions
  void Update();
  void ActivateLights();
  void DeactivateLights();

  // Emergency Functions
  void Deactivate();
  void Activate();

  // Operator Overloading
  GardenCell & operator+=(Plant *pplant); //TODO add, specify position
  GardenCell & operator-=(Plant *pplant);

  // Misc
  bool is_day();
  bool is_night();
private:
  // Pointers to components
  ValveArray *pvalve_array_;
  Lights *plights_;
  TempSensor *ptemp_sensor_;
  LightSensor *plight_sensor_;
  Fan *pfan_in_;
  Fan *pfan_out_;

  // Dynamic arrays (use operator new)
  Plant **pplants_;
  volatile time_t *water_stop_times_;
  volatile time_t *water_start_times_;

  // Booleans for contained objects
  bool lights_on_;
  volatile bool needs_water_ = false;
  bool has_light_sensor_ = false;
  bool has_temp_sensor_ = false;
  bool has_fans_ = false;
  bool fans_on_ = false;

  uint8_t cell_num_;
  uint8_t capacity_;
  uint8_t num_plants_ = 0;
  bool is_active_ = true;

  // Default lighting start time is 7:30 AM
  uint8_t lights_start_tm_[2] = {7, 30};
  // Default lighting stop time is 9:00 PM
  uint8_t lights_stop_tm_[2] = {21, 0};

  FanPreset fan_preset_ = ALWAYS_ON;
  void Schedule(Plant *pplant);
};

#endif
