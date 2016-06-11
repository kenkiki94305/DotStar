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
  SPIModule();
  ~SPIModule();
  virtual void write(char* buff, size_t len);
};
class GPIOModule: public Module{
  GPIOModule(int output_pin);
  ~GPIOModule();
  virtual void write(char* buff,size_t len);
  int _output_pin;
};
