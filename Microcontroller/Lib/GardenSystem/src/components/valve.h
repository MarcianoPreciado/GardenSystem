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
    { return _pin; }
  void set_pin(int pin_num)
    { _pin = pin_num; }
  float get_flow_rate()
    { return _flow_rate; }
  bool is_open()
    { return _is_open; }

  // Critical Functions
  void open_valve();
  void close_valve();

private:
  int _pin;
  float _flow_rate;
  bool _is_open;
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
    { return _size; }
  float get_flow_rate(int pos)
    { return _valves[pos].get_flow_rate(); }
  bool is_open(int pos)
    { return _valves[pos].is_open(); }

  // Critical Functions
  void open_valve(int pos);
  void close_valve(int pos);

private:
  Valve * _valves;
  int _size = 0;
};

#endif

/* Arrangement of plants in a cell
 *  --    --    --
 * | 00 || 01 || 02 |
 *   --    --    --
 * | 03 || 04 || 05 |
 *   --    --    --
 */
