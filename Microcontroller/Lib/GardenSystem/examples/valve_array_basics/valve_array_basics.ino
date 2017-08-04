/* Author: Marciano C. Preciado
 * Date:  17 Mar, 2017
 * This sketch demonstrates the basics of constructing a ValveArray object.
 * Each valve is represented as an LED, at pins 13, 12, and 11.
 *
 * Circuit:
 * Solenoid valves: pins 13, 12, and 11
 *    each with a rate of emission of .5 gallons/hour.
 */
#include "garden_system.h"

uint8_t num_valves = 2;
uint8_t * pins = new uint8_t[num_valves]{13,12};
float * rates = new float[num_valves]{0.5,0.5};
ValveArray v_array(num_valves, pins, rates);

void setup() {
  delete [] pins;
  delete [] rates;
}

void loop() {
  for(uint8_t i = 0; i < num_valves; i++){
    v_array.OpenValve(i);
    delay(500);
  }
  for(uint8_t i = 0; i < num_valves; i++){
    v_array.CloseValve(i);
    delay(500);
  }
}
