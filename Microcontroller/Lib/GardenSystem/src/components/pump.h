#ifndef pump_h
#define pump_h

class Pump{
public:
  // Constructor/Destructor
  Pump(uint8_t pin)
    { pin_ = pin; pinMode(pin_, OUTPUT); }
  ~Pump(){}

  // Getters/Setters
  bool is_active()
    { return is_active_; }
  int get_pin()
    { return pin_; }
  void set_pin(int pin)
    { pin_ = pin; }

  // Critical Functions TODO: Fix for actual pump
  void Activate()
    { digitalWrite(pin_, HIGH); is_active_ = true; }
  void Deactivate()
    { digitalWrite(pin_, LOW); is_active_ = false; }

private:
  int pin_;
  bool is_active_;
};

#endif
