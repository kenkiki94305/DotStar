#include <dotstar.h>

int main(){
  int num_leds = 288;
  DotStar dotstar(num_leds,Module::SPI_MODE);
  vector<vector<double> > data(num_leds);
  vector<vector<double> > data_clear(num_leds);  
  vector<double> pixel(3);
  for(int i = 0; i < 3; i++)
    pixel[i] = 0;
  for(int i = 0; i < data.size(); i++)
    data[i] = pixel;
  data_clear = data;

  while(true){
    for(int i = 0; i < num_leds; i++){
      data = data_clear;
      data[i][0] = 1;
      dotstar.set(data);
      usleep(10000);
    }
  }
  return 0;
}
