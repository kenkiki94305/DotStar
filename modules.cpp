#include <modules.h>
#include <bcm2835.h>
#include <iostream>

Module::Module(int mode){
  _mode = mode;
  if(!bcm2835_init()){
    std::cout<<"bcm2835 init failed Are you running as root?"<<std::endl;
  }
}

Module::~Module(){
  bcm2835_close();
}
SPIModule::SPIModule():Module(Module::SPI_MODE){
  if(!bcm2835_spi_begin()){
    std::cout<<"Failed to start SPI. Are you running as root?"<<std::endl;
  }
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

SPIModule::~SPIModule(){
  bcm2835_spi_end();
}

void SPIModule::write(char* buff, size_t len){
  bcm2835_spi_writenb(buff,len);
}

GPIOModule::GPIOModule(int output_pin):Module(Module::GPIO_MODE){
  if(!bcm2835_init()){
    std::cout<<"bcm2835 init failed Are you running as root?"<<std::endl;
  }
  _output_pin = output_pin;
  bcm2835_gpio_fsel(_output_pin, BCM2835_GPIO_FSEL_OUTP);
}

GPIOModule::~GPIOModule(){

}

void GPIOModule::write(char* buff, size_t len){

}
