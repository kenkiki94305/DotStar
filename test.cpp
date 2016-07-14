#include <dotstar.h>
#include <bcm2835.h>
#include <sys/time.h>
double get_dtime(void){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((double)(tv.tv_sec) + (double)(tv.tv_usec) * 0.001 * 0.001);
}
int main(){
  int num_leds = 144;
  DotStar dotstar(num_leds,Module::GPIO_MODE);
  vector<vector<double> > data(num_leds);
  vector<vector<double> > data_clear(num_leds);  
  vector<double> pixel(3);

  for(int i = 0; i < 3; i++)
    pixel[i] = 0;
  pixel[2] = 1;
  for(int i = 0; i < data.size(); i++)
    data[i] = pixel;
  
  for(int i = 0; i < 3; i++)
    pixel[i] = 0;
  for(int i = 0; i < data.size(); i++)
    data_clear[i] = pixel;
  double start = get_dtime();


  //  while(true){
    //    for(int i = 0; i < num_leds; i++){
      //    data[287][0] = 1;
  for(int i = 0; i < 1000; i++){
    dotstar.set(data);
    //    std::cout<<"hoge"<<std::endl;
  }
  //    usleep(2000000);
    //    a++;
  double end = get_dtime()-start;
  std::cout<<end/1000.0<<std::endl;
      //      
      //    }
//  }
  //  data = data_clear;
  //  
  for(int i = 0; i < 100; i++)
    dotstar.set(data_clear);
  return 0;
}
