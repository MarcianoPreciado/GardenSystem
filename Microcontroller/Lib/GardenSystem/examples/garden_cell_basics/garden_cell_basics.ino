/* Author: Marciano C. Preciado
 * Date:  9 April, 2017
 * This sketch demonstrates the basics of manually constructing a GardenCell object.
 * We begin by constructing a ValveArray object.
 * Each valve is represented as an LED, at pins 12, 11, and 10.
 * Next we create a Lights object, represented as the built-in LED.
 * We pass cell number 0 in the GardenCell constructor as well as a pointer
 * to the Lights and a pointer to the ValveArray.
 *
 * This sketch shows how to use operator overloading with plants to add them
 * to the GardenCell and print some information from the GardenCell to the
 * Serial monitor.
 *
 * Circuit:
 * Solenoid valves: pins 12, 11, and 10
 *    each with a rate of emission of .5 gallons/hour.
 * Lights: built-in LED (usually pin 13)
 */
#include "garden_system.h"

// Construct a garden cell and all of its necessary components.
int num_valves = 3;
int * pins = new int[3]{12,11,10};
float * rates = new float[3]{0.5,0.5,0.5};
ValveArray v_array(num_valves, pins, rates);
Lights lights(LED_BUILTIN);
GardenCell garden_cell(0, &v_array, &lights);

// Create a plant owned by Someone, for cell 0, position 0, needs
// 1 gallon every time its watered, and needs to be watered every 4 days
Plant plant0("Someone", 0, 0, 1, 4);
// Create a plant owned by Someone, for cell 0, position 1, needs
// 1/2 gallon every time its watered, and needs to be watered every 2 days
Plant plant1("Somebody", 0, 1, 0.5, 2);

void setup() {
  // Initialize serial comm
  Serial.begin(9600);

  Serial.print("Capacity: ");
  Serial.println(garden_cell.get_capacity());
  Serial.print("Availablility: ");
  Serial.println(garden_cell.get_availability());
  Serial.println();

  // Insert plants into garden_cell
  Serial.println("Adding 2 new plants");
  garden_cell += plant0;
  garden_cell += plant1;
  Serial.print("Plant0 needs to be watered every ");
  Serial.print(garden_cell.get_plant_at(0).water_period);
  Serial.println(" days.");
  Serial.print("Plant1 needs to be watered with ");
  Serial.print(garden_cell.get_plant_at(1).gal_per_period);
  Serial.println(" gallons each time");
  Serial.print("Availablility is now: ");
  Serial.println(garden_cell.get_availability());
  Serial.println();

  // Remove a plant from the garden_cell
  garden_cell -= garden_cell.get_plant_at(0);
  Serial.println("Removing plant 0");
  Serial.print("Availablility is now: ");
  Serial.println(garden_cell.get_availability());
  Serial.println();
}

void loop() {
  garden_cell.ActivateLights();
  delay(900);
  garden_cell.DeactivateLights();
  delay(900);
}
