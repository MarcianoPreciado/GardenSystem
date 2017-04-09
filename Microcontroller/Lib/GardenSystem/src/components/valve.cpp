#include <Arduino.h>
#include "valve.h"

/* Constructor
 */
Valve::Valve(int pin_num = -1, float flow_rate = 0){
  _pin = pin_num;
  pinMode(_pin, OUTPUT);
  _flow_rate = flow_rate;
  _is_open = false;
}

/* Destructor
 */
Valve::~Valve(){
  close_valve();
}

/* Opens the valve. TODO: Change for actual solenoids
 */
void Valve::open_valve(){
  _is_open = true;
  digitalWrite(_pin, (int)OPEN);
}

/* Closes the valve. TODO: Change for actual solenoids
 */
void Valve::close_valve(){
  _is_open = false;
  digitalWrite(_pin, (int)CLOSE);
}

//========================= V a l v e  A r r a y ==============================//

/* Constructor
 */
ValveArray::ValveArray(int num_valves, const int pins[], const float flow_rate[]){
  _size = num_valves;
  _valves = new Valve[_size];   // Allocate memory
  // Copy pin and flow rate info to each valve
  for(int i = 0; i < _size; i++){
    _valves[i] = Valve(pins[i], flow_rate[i]);
  }
}

/* Deconstructor
 */
ValveArray::~ValveArray(){
  // Close each valve
  for(int i = 0; i < _size; i++){
    close_valve(i);
  }
  delete [] _valves; // Deallocate memory
}

/* Opens the valve at the corresponding cell position.
 */
void ValveArray::open_valve(int pos){
  _valves[pos].open_valve();
}

/* Closes the valve at the corresponding cell position.
 */
void ValveArray::close_valve(int pos){
  _valves[pos].close_valve();
}
