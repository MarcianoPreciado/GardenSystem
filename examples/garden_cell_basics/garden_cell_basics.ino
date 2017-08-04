/* Author: Marciano C. Preciado
   Date:  9 April, 2017
   This sketch demonstrates the basics of manually constructing a GardenCell object.
   We begin by constructing a ValveArray object.
   Each valve is represented as an LED, at pins 12, 11, and 10.
   Next we create a Lights object, represented as the built-in LED.
   We pass cell number 0 in the GardenCell constructor as well as a pointer
   to the Lights and a pointer to the ValveArray.

   This sketch shows how to use operator overloading with plants to add them
   to the GardenCell and print some information from the GardenCell to the
   Serial monitor.

   Circuit:
   Solenoid valves: pins 12, 11, and 10
      each with a rate of emission of .5 gallons/hour.
   Lights: built-in LED (usually pin 13)
*/
#include "garden_system.h"

// Define pins and important values
uint8_t num_valves = 3;
uint8_t * v_pins = new uint8_t[3] {12, 11, 10};
float * v_rates = new float[3] {0.75, 0.75, 0.75};

uint8_t lights_on_pin = LED_BUILTIN;
uint8_t lights_off_pin = LED_BUILTIN;

uint8_t pump_pin = 4;

/* Construct a garden cell and all of its necessary components. */
ValveArray v_array(num_valves, v_pins, v_rates);
Lights lights(lights_on_pin, lights_off_pin);
GardenCell garden_cell(0, &v_array, &lights);

// Construct pump for garden_system
Pump pump(pump_pin);

// Create a plant owned by Someone, for cell 0, position 0, needs
// 1 gallon every time its watered, and needs to be watered every 3 days
Plant plant0("Someone", 0, 1, 3);
// Create a plant owned by Someone, for cell 0, position 1, needs
// 1/2 gallon every time its watered, and needs to be watered every 2 days
Plant plant1("Somebody", 1, 0.5, 2);

GardenSystem garden_system;

void setup() {
  garden_system.Begin(& pump);

  garden_system += &garden_cell;
  garden_cell += &plant0;
  garden_cell += &plant1;

  Serial.println("getting light sensor");
  while (!light_sensor.Begin()) {
    Serial.println("can't find sensor");
    Serial.flush();
  }
  garden_cell.set_light_sensor(&light_sensor);
}

void loop() {
  garden_system.Update();
}

// Example messages
//  {"kind":"handshake"}
//  {"kind":"cell","op":"sensors","cell":0}
