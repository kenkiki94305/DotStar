#ifndef DOTSTAR_MODULES_H
#define DOTSTAR_MODULES_H
#define BCM2708_PERI_BASE        0x3F000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

class Module{
 public:
  enum{
    SPI_MODE,
    GPIO_MODE//DEPRECATED
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
  void setup_io();
  int _out_pin;
  int _clk_pin;
  int mem_fd;
  void *gpio_map;
  volatile unsigned *gpio;
};
#endif //DOTSTAR_MODULES_H
