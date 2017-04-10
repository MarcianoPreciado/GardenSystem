#include <Arduino.h>
#include "valve.h"

/* Constructor
 */
Valve::Valve(int pin_num = -1, float flow_rate = 0){
  pin_ = pin_num;
  pinMode(pin_, OUTPUT);
  flow_rate_ = flow_rate;
  is_open_ = false;
}

/* Destructor
 */
Valve::~Valve(){
  CloseValve();
}

/* Opens the valve. TODO: Change for actual solenoids
 */
void Valve::OpenValve(){
  is_open_ = true;
  digitalWrite(pin_, (int)OPEN);
}

/* Closes the valve. TODO: Change for actual solenoids
 */
void Valve::CloseValve(){
  is_open_ = false;
  digitalWrite(pin_, (int)CLOSE);
}

//========================= V a l v e  A r r a y ==============================//

/* Constructor
 */
ValveArray::ValveArray(int num_valves, const int pins[], const float flow_rate[]){
  size_ = num_valves;
  valves_ = new Valve[size_];   // Allocate memory
  // Copy pin and flow rate info to each valve
  for(int i = 0; i < size_; i++){
    valves_[i] = Valve(pins[i], flow_rate[i]);
  }
}

/* Deconstructor
 */
ValveArray::~ValveArray(){
  // Close each valve
  for(int i = 0; i < size_; i++){
    CloseValve(i);
  }
  delete [] valves_; // Deallocate memory
}

/* Opens the valve at the corresponding cell position.
 */
void ValveArray::OpenValve(int pos){
  valves_[pos].OpenValve();
}

/* Closes the valve at the corresponding cell position.
 */
void ValveArray::CloseValve(int pos){
  valves_[pos].CloseValve();
}
