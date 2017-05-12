#ifndef unit_test_h
#define unit_test_h

#include <Arduino.h>
#include "garden_system.h"

// Construct pump for garden_system
Pump pump(pump_pin);
//--------------------------  Forward Declarations ---------------------------//
template <class test>
void assert_equals(test real, test expected);
void assert_not_equals(test real, test expected);
// Cell Tests
void test_cell();
void test_plants(GardenCell garden_cell);
void test_sensors(GardenCell garden_cell);

//--------------------------- Function Definitions ---------------------------//
void assert_equals(test real, test expected){
  if((a - b) < 0.01){
    Serial.println("TEST PASSED");
  }
  else{
    Serial.print("TEST FAILED. EXPECTED: ");
    Serial.print(expected);
    Serial.print(" RECIEVED: ");
    Serial.println(real);
  }
  return;
}

void assert_not_equals(test real, test expected){
  if((a - b) > 0.01){
    Serial.println("TEST PASSED");
  }
  else{
    Serial.print("TEST FAILED");
  }
  return;
}

/* Runs all test functions of the garden cell class
 */
void test_cell(){
  // Define pins and important values
  uint8_t num_valves = 3;
  uint8_t * v_pins = new uint8_t[3] {12, 11, 10};
  float * v_rates = new float[3] {0.5, 0.5, 0.5};
  uint8_t lights_pin = LED_BUILTIN;
  uint8_t pump_pin = 4;

  /* Construct a garden cell and all of its necessary components. */
  ValveArray v_array(num_valves, v_pins, v_rates);
  Lights lights(lights_pin);
  GardenCell garden_cell(0, &v_array, &lights);

  test_plants(garden_cell);
}

/* Test the functionality of plants in unison with the garden cell class
 */
void test_plants(GardenCell garden_cell){
  Serial.println("===== Cell Plant Tests ======");
  // Create some plants
  // Params: owner, cell, position, gal/period, period
  Plant plant0("test0", 0, 0, 0.5, 4);
  Plant plant1("test1", 0, 2, 0.5, 4);

  // Initiation tests
  Serial.println("--Initiation Test (get_num_plants)");
  assert_equals(garden_cell.get_num_plants(), 0);
  Serial.println("--Initiation Test (get_plant)");
  for(int i = 0; i < garden_cell.get_capacity(); i++)
    assert_equals(garden_cell.get_plant(i), nullptr);
  Serial.println("--Initiation Test (get_availability)");
  assert_equals(garden_cell.get_availability, garden_cell.get_capacity);
  // Addition Test 1
  garden_cell += &plant0;
  Serial.println("--Addition Test 1 (get_num_plants)");
  assert_equals(garden_cell.get_num_plants(), 1);
  Serial.println("--Addition Test 1 (get_plant)");
  assert_equals(garden_cell.get_plant(0), &plant0);
  Serial.println("--Addition Test 1 (get_availability)");
  assert_equals(garden_cell.get_availability, garden_cell.get_capacity - 1);
  // Addition Test 2
  garden_cell += &plant1;
  Serial.println("--Addition Test 2 (get_num_plants)");
  assert_equals(garden_cell.get_num_plants(), 2);
  Serial.println("--Addition Test 2 (get_plant)");
  assert_equals(garden_cell.get_plant(2), &plant1);
  Serial.println("--Addition Test 2 (get_availability)");
  assert_equals(garden_cell.get_availability, garden_cell.get_capacity - 2);
  // Removal Test 1
  garden_cell -= &plant0;
  Serial.println("--Removal Test 1 (get_num_plants)");
  assert_equals(garden_cell.get_num_plants(), 1);
  Serial.println("--Removal Test 1 (get_plant)");
  assert_equals(garden_cell.get_plant(0), nullptr);
  Serial.println("--Removal Test 2 (get_availability)");
  assert_equals(garden_cell.get_availability, garden_cell.get_capacity - 1);
  // Removal Test 2
  garden_cell -= &plant1;
  Serial.println("--Removal Test 2 (get_num_plants)");
  assert_equals(garden_cell.get_num_plants(), 0);
  Serial.println("--Removal Test 2 (get_plant)");
  assert_equals(garden_cell.get_plant(2), nullptr);
  Serial.println("--Removal Test 2 (get_availability)");
  assert_equals(garden_cell.get_availability, garden_cell.get_capacity);
}

/* Tests the functionality of the sensors in unison with the garden cell class
 */
void test_sensors(GardenCell garden_cell){
  Serial.println("===== Cell Sensor Tests ======");
  TempSensor temp_sensor;
  LightSensor light_sensor;
  // Test before sensors are set
  Serial.println("--Not Set Test (has_temp_sensor)");
  assert_equals(garden_cell.has_temp_sensor(), false);
  Serial.println("--Not Set Test (has_light_sensor)");
  assert_equals(garden_cell.has_light_sensor(), false);
  Serial.println("--Not Set Get Value Test (get_temp_val)");
  assert_equals(garden_cell.get_temp_val(), 0);
  Serial.println("--Not Set Get Value Test (get_light_val)");
  assert_equals(garden_cell.get_light_val(), 0);

  // Test after sensors are set
  garden_cell.set_temp_sensor(temp_sensor);
  garden_cell.set_temp_sensor(temp_sensor);
  Serial.println("--Set Test (has_temp_sensor)");
  assert_equals(garden_cell.has_temp_sensor(), true);
  Serial.println("--Set Test (has_light_sensor)");
  assert_equals(garden_cell.has_temp_sensor(), true);
  Serial.println("--Set Get Value Test (get_temp_val)");
  assert_not_equals(garden_cell.get_temp_val(), 0);
  Serial.println("--Set Get Value Test (get_light_val)");
  assert_not_equals(garden_cell.get_light_val(), 0);
}


#endif;
