/* Author: Marciano C. Preciado
 * Date:  17 Mar, 2017
 * This sketch demonstrates the basics of constructing a ValveArray object.
 * Each valve is represented as an LED, at pins 12, 11, and 10.
 *
 * Circuit:
 * Solenoid valves: pins 12, 11, and 10
 *    each with a rate of emission of .5 gallons/hour.
 * Pump: pin 9
 */
#include "garden_system.h"

// Set up pump
uint8_t pump_pin = 9;
Pump pump(pump_pin);

// Set up valves
uint8_t num_valves = 3;
uint8_t * pins = new uint8_t[num_valves]{12,11,10};
float * rates = new float[num_valves]{0.5,0.5, 0.5};
ValveArray v_array(num_valves, pins, rates);

void setup() {

}

unsigned long on_duration = 60 * 2;
unsigned long off_duration = 60 * 1;
unsigned long strt_tm;

void loop() {
  v_array.OpenValve(0);
  v_array.OpenValve(1);
  v_array.OpenValve(2);
  pump.Activate();
  strt_tm = now();
  while(now() - strt_tm < on_duration);

  pump.Deactivate();
  v_array.CloseValve(0);
  v_array.CloseValve(1);
  v_array.CloseValve(2);
  strt_tm = now();
  while(now() - strt_tm < off_duration);
}
