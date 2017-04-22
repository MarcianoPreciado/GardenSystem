#ifndef valve_h
#define valve_h
#include <Arduino.h>

/*
 * Class for garden's solenoid valves.
 * Each valve leads to a drip emitter with a specific flowrate,
 * thus each valve has a pin number and a flowrate.
 */
class Valve{
public:
  // Constructor/Destructor
  Valve(uint8_t pin_num, float flow_rate);
  Valve(uint8_t &rpin_num, float &rflow_rate);
  ~Valve();

  // Getters/Setters
  uint8_t get_pin()
    { return pin_; }
  void set_pin(uint8_t pin_num)
    { pin_ = pin_num; }
  float get_flow_rate()
    { return flow_rate_; }
  bool is_open()
    { return is_open_; }

  // Critical Functions
  void OpenValve();
  void CloseValve();

private:
  uint8_t pin_;
  float flow_rate_;
  bool is_open_;
  enum { OPEN = 1, CLOSE = 0 };
};

/* Class for an array of valves.
 * Each garden cell has one valve array, this makes it easier to keep track
 * of the valves in each garden cell.
 */
class ValveArray{
public:
  // Constructor/Destructor
  ValveArray(uint8_t num_valves, const uint8_t pins[], const float flow_rate[]);
  ~ValveArray();

  // Getters
  uint8_t get_size()
    { return size_; }
  float get_flow_rate(uint8_t pos)
    { return valves_[pos].get_flow_rate(); }
  bool is_open(uint8_t pos)
    { return valves_[pos].is_open(); }

  // Critical Functions
  void OpenValve(uint8_t pos);
  void CloseValve(uint8_t pos);

private:
  Valve *valves_;
  uint8_t size_ = 0;
};

#endif

/* Arrangement of plants in a cell
 *  --    --    --
 * | 00 || 01 || 02 |
 *   --    --    --
 * | 03 || 04 || 05 |
 *   --    --    --
 */
