#include <modules.h>
#include <bcm2835.h>
#include <iostream>

Module::Module(int mode){
  _mode = mode;
  //  if(!bcm2835_init()){
  //    std::cout<<"bcm2835 init failed Are you running as root?"<<std::endl;
  //  }
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
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);
  //  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

SPIModule::~SPIModule(){
  bcm2835_spi_end();
}

void SPIModule::write(char* buff, size_t len){
  bcm2835_spi_writenb(buff,len);
}

GPIOModule::GPIOModule(int clk_pin, int out_pin):Module(Module::GPIO_MODE){
  _out_pin = out_pin;
  _clk_pin = clk_pin;
  setup_io();
  INP_GPIO(_out_pin);
  OUT_GPIO(_out_pin);
  INP_GPIO(_clk_pin);
  OUT_GPIO(_clk_pin);
}

GPIOModule::~GPIOModule(){

}

void GPIOModule::write(char* buff, size_t len){
  volatile uint8_t high;
  volatile uint8_t low;
  
  for(int i = 0; i < len; i++){
    for(int j = 0; j< 8; j++){
      //      high = 50;
      //      low = 60;
      high = 200;
      low =200;

      GPIO_CLR = 1 << _clk_pin;
      if((buff[i]>>j)&0x01)
	GPIO_SET = 1 << _out_pin;
      else
	GPIO_CLR = 1 << _out_pin;

      

      while(low--);
      GPIO_SET = 1 << _clk_pin;
                  while(high--);
    }
  }

}
void GPIOModule::setup_io(){
  /* open /dev/mem */
  if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
    printf("can't open /dev/mem \n");
    exit(-1);
  }

  /* mmap GPIO */
  gpio_map = mmap(
		  NULL,             //Any adddress in our space will do
		  BLOCK_SIZE,       //Map length
		  PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
		  MAP_SHARED,       //Shared with other processes
		  mem_fd,           //File to map
		  GPIO_BASE         //Offset to GPIO peripheral
		  );

  close(mem_fd); //No need to keep mem_fd open after mmap

  if (gpio_map == MAP_FAILED) {
    printf("mmap error %d\n", (int)gpio_map);//errno also set!
    exit(-1);
  }

  // Always use volatile pointer!
  gpio = (volatile unsigned *)gpio_map;

   


}

