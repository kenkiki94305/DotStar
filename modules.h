#ifndef DOTSTAR_MODULES_H
#define DOTSTAR_MODULES_H
#include <stdio.h>

class Module{
 public:
  enum{
    SPI_MODE,
    GPIO_MODE
  };
  Module(int mode);
  ~Module();
  virtual void write(char* buff,size_t len) = 0;
 private:
  int _mode;
};
class SPIModule: public Module{
 public:
  SPIModule();
  ~SPIModule();
  virtual void write(char* buff, size_t len);
};
class GPIOModule: public Module{
 public:
  GPIOModule(int clk_pin, int out_pin);
  ~GPIOModule();
  virtual void write(char* buff,size_t len);
  int _out_pin;
  int _clk_pin;
};
#endif //DOTSTAR_MODULES_H
