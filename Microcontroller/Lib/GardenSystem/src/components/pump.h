#ifndef pump_h
#define pump_h

class Pump{
public:
  // Constructor/Destructor
  Pump(uint8_t pin)
    { _pin = pin; }
  ~Pump();

  // Getters/Setters
  bool is_on()
    { return is_on_; }
  int get_pin()
    { return pin_; }
  void set_pin(int pin)
    { pin_ = pin; }

  // Critical Functions TODO: Fix for actual pump
  void Activate()
    { digitalWrite(_pin, HIGH); is_on_ = true; }
  void Deactivate()
    { digitalWrite(_pin, LOW); is_on_ = false; }

private:
  int pin_;
  bool is_on_;
};

#endif
