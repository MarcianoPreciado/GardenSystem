#ifndef garden_system_h
#define garden_system_h

// C++ Standard Library
#include <ArduinoSTL.h>
#include <vector>
// Component Headers
#include "components/pump.h"
#include "garden_cell.h"

// TODO: Make sort procedure more efficient. (Takes too long currently)
class GardenSystem{
public:
  GardenSystem(Pump *ppump);
  // Accessors
  int get_num_cells();
  GardenCell &get_cell(uint8_t pos);
  GardenCell *get_cell_ptr(uint8_t pos);

  // Critical Functions
  void Update();

  // Emergency Functions
  void Deactivate();
  void Activate();

  /* Add a GardenCell object to the system. It will be stored as a pointer in the
   * pgarden_cells_ vector and be supported by the system. Re-sorts vector by
   * cell number.
   * Ex. System += &garden_cell1;
   * Ex. System += pgarden_cell;
   */
  GardenSystem &operator+=(GardenCell *pg_c){
   pgarden_cells_.push_back(pg_c);
   num_cells_ ++;
   typedef GardenCell* pGC;
   auto SortLambda = [](pGC pc1, pGC pc2){return (pc1->get_cell_num() < pc2->get_cell_num());};
   std::sort(pgarden_cells_.begin(), pgarden_cells_.end(), SortLambda);
   // TODO: Add event message
   return *this;
  }

  /* Remove a GardenCell object from the system. Must be the same instance as the
  * one stored in the pgarden_cells_ vector.
  * Ex. System -= System.get_cell_ptr(pos);
  */
  GardenSystem &operator-=(GardenCell *pg_c){
   // TODO: "find" index may be incorrect.
   //        add event message
   auto ind = std::find(pgarden_cells_.begin(), pgarden_cells_.end(), pg_c);
   pgarden_cells_.erase(ind);
   num_cells_ --;
   return *this;
  }
  void PrintCells(){
    for(int i = 0; i < num_cells_; i++){
      Serial.println(pgarden_cells_[i]->get_cell_num());
    }
  }
private:
  // Pointer to contained pump
   Pump *ppump_;
  // GardenCell pointer vector
   std::vector<GardenCell*> pgarden_cells_;

   uint8_t num_cells_ = 0;
   bool is_on_ = false;
};

#endif
