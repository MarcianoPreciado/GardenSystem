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
//#include "comm_link.h"
#include "garden_system.h"

Pump * GardenSystem::ppump_;
std::vector<GardenCell*> GardenSystem::pgarden_cells_;
bool GardenSystem::is_on_ = false;
uint8_t GardenSystem::num_cells_ = 0;
//GardenSystem garden_system;

void GardenSystem::Begin(Pump *ppump){
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
  if(ind != pgarden_cells_.end()){
    return *ind;
  }
  else
    return nullptr;
}

//==================== C R I T I C A L  F U N C T I O N S ====================//

/* Checks the state of each member GardenCell and performs all necessary
 * operations. To be called regularly, ie. insert in loop();
 */
void GardenSystem::Update(){
  // If active, update and check if the cells need water.
  // If one of them needs water, turn the pump on.
  if(is_on_){
    bool need_water = false;
    for(int i = 0; i < num_cells_; i++){
      pgarden_cells_[i]->Update();
      need_water |= pgarden_cells_[i]->needs_water();
    }
    if(need_water && !ppump_->is_active())
      ppump_->Activate();
    else if (!need_water && ppump_->is_active())
      ppump_->Deactivate();
  }
}

/* Adds the input plant pointer to the specified garden cell reference
 */
void GardenSystem::AddPlantToCell(Plant *pplant, GardenCell &rgarden_cell){
  rgarden_cell += pplant;
}

/* Removes the input plant pointer from the specified garden cell reference
 */
void GardenSystem::RemovePlantFromCell(Plant *pplant, GardenCell &rgarden_cell){
   rgarden_cell -= pplant;
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
