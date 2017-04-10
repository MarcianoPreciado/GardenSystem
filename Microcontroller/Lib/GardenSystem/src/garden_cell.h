#ifndef garden_cell_h
#define garden_cell_h
#include <Arduino.h>
#include <Time.h>
#include "components/valve.h"
#include "components/lights.h"


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
  int get_capacity();
  int get_availability();
  Plant get_plant_at(uint8_t pos);
  bool is_lighting();
  bool is_watering();
  void set_lights_on_time(uint8_t hr, uint8_t mn);
  void set_lights_off_time(uint8_t hr, uint8_t mn);

  // Critical Functions
  void Update();
  void WaterPlants();
  void ActivateLights();
  void DeactivateLights();

  // Emergency Functions
  void DeactivateCell();
  void ActivateCell();

  // Operator Overloading
  GardenCell & operator+=(const Plant & plant);
  GardenCell & operator-=(const Plant & plant);
private:
  // Pointers to components
  ValveArray *pvalve_array_;
  Lights *plights_;
  // Dynamic arrays (use operator new)
  Plant *plants_;
  time_t *water_stop_times_;

  bool lights_on_;
  bool watering_;

  uint8_t cell_num_;
  int capacity_;
  int numplants_;
  bool is_on_;

  // Default lighting start time is 7:30 AM
  uint8_t lights_start_hr_ = 7;
  uint8_t lights_start_mn_ = 30;
  // Default lighting stop time is 9:00 PM
  uint8_t lights_stop_hr_ = 21;
  uint8_t lights_stop_mn_ = 0;

  void Schedule();
};

#endif
