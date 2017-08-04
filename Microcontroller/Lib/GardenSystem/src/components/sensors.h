#ifndef sensors_h
#define sensors_h

#include <Wire.h>
#include <Adafruit_MCP9808.h>
#include <TSL2561.h>
//#include <freqmeasure.h>

class Sensor{
public:
  double get_val()
    { return ReadSensor(); }
protected:
  const String type_;
  const String sensor_;
  virtual double ReadSensor() = 0;
};

class I2cSensor: public Sensor{
public:
  uint8_t get_addr()
    { return addr_; }
protected:
  uint8_t addr_;
};

class AnalogSensor: public Sensor{
public:
  uint8_t get_pin()
    { return pin_; }
protected:
  uint8_t pin_;
};

//----------------------------- SPECIFIC SENSORS -----------------------------//

class TempSensor: public I2cSensor{
private:
  Adafruit_MCP9808 temp_sensor_;
  const String type_ = "i2c";
  const String sensor_ = "temp";
  double ReadSensor(){
    temp_sensor_.shutdown_wake(0);
    double data = temp_sensor_.readTempC();
    temp_sensor_.shutdown_wake(1);
    return data;
  }
public:
  TempSensor(uint8_t addr = 0x18):temp_sensor_(){
    addr_ = addr;
  }
  bool Begin(){
    if(temp_sensor_.begin(addr_)){
      temp_sensor_.shutdown_wake(1);
      return true;
    }
    return false;
  }
};

class LightSensor: public I2cSensor{
private:
  TSL2561 light_sensor_;
  const String type_ = "i2c";
  const String sensor_ = "light";
  double ReadSensor(){
    uint32_t lum = light_sensor_.getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    return light_sensor_.calculateLux(full, ir);
  }
public:
  LightSensor(uint8_t addr = 0x39):light_sensor_(addr){
    addr_ = addr;
  }
  bool Begin(){
    if(light_sensor_.begin()){
      light_sensor_.setGain(TSL2561_GAIN_16X);
      light_sensor_.setTiming(TSL2561_INTEGRATIONTIME_13MS);
      return true;
    }
    return false;
  }
};

class FlowSensor : public AnalogSensor {
public:

private:

};

#endif
