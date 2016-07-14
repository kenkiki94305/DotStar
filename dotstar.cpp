#include <dotstar.h>
#include <string.h>
#include <math.h>
#include <modules.h>

DotStar::DotStar(int num_led,int mode){
  led_n = num_led;
  buffer_len = sizeof(int)*(led_n+2);
  buffer = new char[buffer_len];
  
  if(mode == Module::SPI_MODE)
    module = new SPIModule();

  else if(mode == Module::GPIO_MODE)
    module = new GPIOModule(21,20);

}

DotStar::~DotStar(){
  delete module;
}
void DotStar::set(vector<vector<double> > &data){
  int buffer_count = 0;
  buffer_count += _addHeader(&buffer[buffer_count]);
  for(int i = 0; i < data.size(); i++){
    buffer_count += _addLEDData(&buffer[buffer_count],
				data[i][0],data[i][1],data[i][2],0.1);
  }
  buffer_count += _addFooter(&buffer[buffer_count]);

  module->write(buffer,buffer_len);
}
size_t DotStar::_addHeader(char* buff){
  char data = _DecToBinary(0);
  for(size_t i = 0; i < 4; i++)
    memcpy(buff+i,&data,1);
  
  return 4;
}
size_t DotStar::_addLEDData(char* buff, double r, double g, double b, double global){
  int global_strength = floor(global*31);
  char data[4];
  data[0] = 0b00000111 << 5 | _DecToBinary(global_strength);
  data[1] = _DecToBinary(floor(b*255));
  data[2] = _DecToBinary(floor(g*255));
  data[3] = _DecToBinary(floor(r*255));
  memcpy(buff,&data[0],sizeof(int));
  return sizeof(int);  
}
size_t DotStar::_addFooter(char* buff){
  char data = _DecToBinary(0);
  int footer_bytes = led_n/8;
  for(size_t i = 0; i < footer_bytes; i++)
    memcpy(buff,&data,1);
  
  return footer_bytes;
}
char DotStar::_DecToBinary(int val){
  char bin = 0x00;
  int i = 0;
  while(val>0){
    bin = bin | val % 2;
    val/=2;
    if(i < 7)
      bin = bin<<1;
    i++;
  }
  return bin;
}
