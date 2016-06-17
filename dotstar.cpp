#include <dotstar.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <modules.h>
#include <bcm2835.h>
#include <sys/mman.h>
bool DotStar::init(){
}
  
DotStar::DotStar(int num_led){
  led_n = num_led;
  buffer_len = sizeof(int)*(led_n+2);
  buffer = new char[buffer_len];
  
  //  if(mode == Module::SPI_MODE)
     module = new SPIModule();

    //  else if(mode == Module::GPIO_MODE)
     //  module = new GPIOModule(RPI_V2_GPIO_P1_18,RPI_V2_GPIO_P1_16);

}

DotStar::~DotStar(){
  delete module;
}
void DotStar::set(vector<vector<double> > &data){
  int buffer_count = 0;
  buffer_count += _addHeader(&buffer[buffer_count]);
  for(int i = 0; i < data.size(); i++){
    buffer_count += _addLEDData(&buffer[buffer_count],
				data[i][0],data[i][1],data[i][2]);
  }
  buffer_count += _addFooter(&buffer[buffer_count]);

  module->write(buffer,buffer_len);
}
size_t DotStar::_addHeader(char* buff){
  char data = _DecToBinary(0);
  for(size_t i = 0; i < sizeof(int); i++)
    memcpy(buff+i,&data,1);
  
  return sizeof(int);
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
  char data = _DecToBinary(255);
  for(size_t i = 0; i < sizeof(int); i++)
    memcpy(buff,&data,1);
  
  return sizeof(int);
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
double get_dtime(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((double)(tv.tv_sec) + (double)(tv.tv_usec) * 0.001 * 0.001);
}
int main(){

  DotStar dotstar(144);
  vector<vector<double> > data(144);
  vector<double> pixel(3);
  for(int i = 0; i < 3; i++)
    pixel[i] = 0;
  for(int i = 0; i < data.size(); i++)
    data[i] = pixel;

  double last_time = get_dtime();
  double step = 1;
  data[143][0] = 1;
  data[143][1] = 0;
  data[143][2] = 0;
  int loop_times = 10000;

  int buffer_count = 0;
  char *buffer;
  buffer = new char[144];
  buffer_count += dotstar._addHeader(&buffer[buffer_count]);
  for(int i = 0; i < data.size(); i++){
    buffer_count += dotstar._addLEDData(&buffer[buffer_count],
				data[i][0],data[i][1],data[i][2]);
  }
  buffer_count += dotstar._addFooter(&buffer[buffer_count]);
  double start_time = get_dtime();  
  for(int i = 0; i < loop_times; i++){
    dotstar.module->write(buffer,buffer_count);
    //     if(last_time + step > get_dtime()){
    //    last_time = get_dtime();
    //    dotstar.set(data);
  }
  double duration = get_dtime() - start_time;
  cout << "time to send " << duration/loop_times*1000 << " ms"<<endl;

  return 0;
}
