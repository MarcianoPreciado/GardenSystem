#ifndef lights_h
#define lights_h

/* Class for the set of lights in each garden cell.
 * Covers all the basic functionality of the lights.
 */
class Lights{
public:
  // Constructor/Destructor
  Lights(int pin)
    { pin_ = pin; pinMode(pin_, OUTPUT); }
  ~Lights(){}

  // Getters/Setters
  bool is_active()
    { return is_active_; }
  int get_pin()
    { return pin_; }
  void set_pin(int pin)
    { pin_ = pin; }

  // Critical Functions TODO: Fix for actual lights
  void Activate()
    { digitalWrite(pin_, HIGH); is_active_ = true; }
  void Deactivate()
    { digitalWrite(pin_, LOW); is_active_ = false; }

private:
  int pin_;
  bool is_active_;

};

#endif
