// Arduino Libraries
#include <Arduino.h>
// C++ Standard Library
#include <ArduinoSTL.h>
#include <vector>
// Component Headers
#include "components/valve.h"
#include "components/lights.h"
#include "components/pump.h"
#include "garden_cell.h"
#include "comm_link.h"
#include "garden_system.h"

GardenSystem::GardenSystem(Pump *ppump){
  num_cells_ = 0;
  is_on_ = true;
  ppump_ = ppump;
  pgarden_cells_.clear();
  // TODO: send event message
}

//============================ A C C E S S O R S =============================//

int GardenSystem::get_num_cells(){
  return num_cells_;
}

GardenCell &GardenSystem::get_cell(uint8_t pos){
  // TODO: finish
  auto CellSearchLamda = [pos](GardenCell *pg_c){return pg_c->get_cell_num() == pos;};
  auto ind = std::find_if(pgarden_cells_.begin(), pgarden_cells_.end(), CellSearchLamda);
  return **ind;
}

GardenCell *GardenSystem::get_cell_ptr(uint8_t pos){
  // TODO: finish
  auto CellSearchLamda = [pos](GardenCell *pg_c){return pg_c->get_cell_num() ==  pos;};
  auto ind = std::find_if(pgarden_cells_.begin(), pgarden_cells_.end(), CellSearchLamda);
  return *ind;
}

//==================== C R I T I C A L  F U N C T I O N S ====================//

/* Checks the state of each member GardenCell and performs all necessary
 * operations. To be called regularly, ie. insert in loop();
 */
void GardenSystem::Update(){
  // If active, update ...
  if(is_on_){
    // TODO: finish
    for(int i = 0; i < num_cells_; i++){
      pgarden_cells_[i]->Update();
      Serial.println("yes");
    }
  }
}

//=================== E M E R G E N C Y  F U N C T I O N S ===================//

/*
 */
void GardenSystem::Deactivate(){
  is_on_ = false;
  // TODO: Send event message
}

/*
 */
void GardenSystem::Activate(){
  is_on_ = true;
  // TODO: Send event message
}
