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
    { return _is_on; }
  int get_pin()
    { return _pin; }
  void set_pin(int pin)
    { _pin = pin; }

  // Critical Functions TODO: Fix for actual pump
  void activate()
    { digitalWrite(_pin, HIGH); _is_on = true; }
  void deactivate()
    { digitalWrite(_pin, LOW); _is_on = true; }

private:
  int _pin;
  bool _is_on;
};

#endif
