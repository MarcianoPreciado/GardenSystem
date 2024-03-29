/* Author: Marciano C. Preciado
 * Date:  5 April, 2017
 * This sketch demonstrates the basics of the lights class and their operation.
 * Blinks the light every second and prints to the serial monitor.
 *
 * Circuit:
 * Lights: pin 13
 */
 #include "garden_system.h"

const uint8_t light_on_pin = 13;
const uint8_t light_off_pin = 12;
 Lights light(light_on_pin, light_off_pin);

 void setup(){
   Serial.begin(9600);
   Serial.println("Light begins activated.");
   light.Activate();
 }

 void loop(){
   if(light.is_on()){
     Serial.println("Deactivating Light");
     light.Deactivate();
   }
   else{
     Serial.println("Activating Light");
     light.Activate();
   }
   delay(1000);
 }
