#ifndef fan_h
#define fan_h
#include <Arduino.h>

/* Class for defining the ventilation fans in each cell.
 */
class Fan{
public:
  // Constructor/Destructor
  Fan(uint8_t pin_num)
    {pinMode(pin_num,OUTPUT); pin_ = pin_num; is_active_ = false; }
  ~Fan(){}

   // Getters/Setters
  uint8_t get_pin()
    { return pin_; }
  void set_pin(uint8_t pin_num)
    { pin_ = pin_num; }
  virtual bool is_active()
    { return is_active_; }

  // Critical Functions
  virtual void Activate()
    { digitalWrite(pin_, HIGH); is_active_ = true; }
  virtual void Deactivate()
    { digitalWrite(pin_, LOW); is_active_ = false; }

private:
  uint8_t pin_;
  bool is_active_;
};

/* Class for defining arrays of fans.
 * Because it is publically inherited from the base Fan class, it should work in
 * any place the Fan class does, but can be used to activate clusters of fans all
 * at once, aside from get/set pin functions.
 */
class FanArray : public Fan{
public:
  // Constructor/Destructor
  FanArray(uint8_t num_fans, const uint8_t pins[]): Fan(0){
    size_ = num_fans;
    is_active_ = false;
    pins_ = new uint8_t[size_]; // Allocate memory
    // Initialize fans with given pin numbers
    for(int i = 0; i < size_; i++){
      pinMode(pins_[i], OUTPUT);
      pins_[i] = pins[i];
    }
  }
  ~FanArray(){
    delete [] pins_;
  }

  // Getters
  uint8_t get_size()
    {return size_; }
  bool is_active()
    {return is_active_; }

  // Critical Functions
  void Activate(){
    is_active_ = true;
    for(int i = 0; i < size_; i++)
      digitalWrite(pins_[i], HIGH);
  }
  void Deactivate(){
    is_active_= false;
    for(int i = 0; i < size_; i++)
      digitalWrite(pins_[i], LOW);
  }

private:
  uint8_t size_;
  bool is_active_;
  uint8_t *pins_;
};

#endif
