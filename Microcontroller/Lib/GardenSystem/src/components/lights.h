#ifndef lights_h
#define lights_h

/* Class for the set of lights in each garden cell.
 * Covers all the basic functionality of the lights.
 */
class Lights{
public:
  // Constructor/Destructor
  Lights(uint8_t on_pin, uint8_t off_pin)
    { on_pin_ = on_pin; pinMode(on_pin_, OUTPUT);
      off_pin_ = off_pin; pinMode(off_pin_, OUTPUT);}
  ~Lights(){}

  // Getters/Setters
  bool is_active()
    { return is_active_; }
  int get_on_pin()
    { return on_pin_; }
  void set_on_pin(int pin)
    { on_pin_ = pin; }
  int get_off_pin()
    { return off_pin_; }
  void set_off_pin(int pin)
    { off_pin_ = pin; }

  // Critical Functions TODO: Fix for actual lights
  void Activate()
    { TripPin(on_pin_); is_active_ = true; }
  void Deactivate()
    { TripPin(off_pin_); is_active_ = false; }

private:
  uint8_t on_pin_;
  uint8_t off_pin_;
  bool is_active_;
  TripPin(uint8_t pin){
    digitalWrite(pin, HIGH);
    delay(20);
    digitalWrite(pin, LOW);
  }
};

#endif
