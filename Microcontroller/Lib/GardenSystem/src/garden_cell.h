#include "components/valve.h"
#include "components/lights.h"

#ifndef garden_cell_h
#define garden_cell_h
#include <Arduino.h>

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
  String owner;
  uint8_t cell;
  uint8_t position;
  float gal_per_period;
  float water_period;
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

  // Getters/Setters
  int get_capacity();
  int get_availability();
  bool is_lighting();
  bool is_watering();

  // Critical Functions
  void update();
  void water_plants();
  void activate_lights();
  void deactivate_lights();

  // Emergency Functions
  void deactivate();
  void activate();

private:
  // Pointers to components
  ValveArray *_pvalve_array;
  Lights *_plights;
  // Dynamic arrays (use operator new)
  Plant *_plants;
  time_t *_water_stop_times;

  bool _lights_on;
  bool _watering;

  uint8_t _cell_num;
  int _capacity;
  int _availability;
  int _num_plants;

  bool _is_on;
};

#endif
