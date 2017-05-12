/* Author: Marciano C. Preciado
   Date: 11 May, 2017
   This sketch's purpose is to act as a compilation of unit tests to insure that
   each individual function works appropriately. Due to the nature of the comm
   link class, many of the functions can not be fully tested.

   The results of each unit test will be printed to the serial monitor.
*/
#include "garden_system.h"
#include "comm_link.h"



// Initial parameters/pin declarations
uint8_t num_valves = 2;
uint8_t *v_pins = new uint8_t[2] {33,34};
float * v_rates = new float[3] {0.5, 0.5};
uint8_t lights_pin = 13;
uint8_t pump_pin = 33;

// Construct the objects we will be using
GardenSystem garden_system;


void setup(){
  Serial.begin(9600);

  // Garden System should start with 0 cells
  Serial.println("TEST 1");
  assert_equals(garden_system.get_num_cells(), 0);

  // Adding
}

void loop(){}
