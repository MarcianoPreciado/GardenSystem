#ifndef lights_h
#define lights_h

/* Class for the set of lights in each garden cell.
 * Covers all the basic functionality of the lights.
 */
class Lights{
public:
  // Constructor/Destructor
  Lights(int pin)
    { _pin = pin; }
  ~Lights();

  // Getters/Setters
  bool is_on()
    { return _is_on; }
  int get_pin()
    { return _pin; }
  void set_pin(int pin)
    { _pin = pin; }

  // Critical Functions TODO: Fix for actual lights
  void activate()
    { digitalWrite(_pin, HIGH); _is_on = true; }
  void deactivate()
    { digitalWrite(_pin, LOW); _is_on = true; }

private:
  int _pin;
  bool _is_on;

};

#endif
