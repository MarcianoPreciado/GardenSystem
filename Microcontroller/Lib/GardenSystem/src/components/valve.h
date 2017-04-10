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
  Valve(int pin_num, float flow_rate);
  ~Valve();

  // Getters/Setters
  int get_pin()
    { return pin_; }
  void set_pin(int pin_num)
    { pin_ = pin_num; }
  float get_flow_rate()
    { return flow_rate_; }
  bool is_open()
    { return is_open_; }

  // Critical Functions
  void OpenValve();
  void CloseValve();

private:
  int pin_;
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
  ValveArray(int num_valves, const int pins[], const float flow_rate[]);
  ~ValveArray();

  // Getters
  int get_size()
    { return size_; }
  float get_flow_rate(int pos)
    { return valves_[pos].get_flow_rate(); }
  bool is_open(int pos)
    { return valves_[pos].is_open(); }

  // Critical Functions
  void OpenValve(int pos);
  void CloseValve(int pos);

private:
  Valve *valves_;
  int size_ = 0;
};

#endif

/* Arrangement of plants in a cell
 *  --    --    --
 * | 00 || 01 || 02 |
 *   --    --    --
 * | 03 || 04 || 05 |
 *   --    --    --
 */
