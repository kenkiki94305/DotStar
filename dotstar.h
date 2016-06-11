
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;
class DotStar{
 public:
  /**
   * @fn
   * constuctor
   * @param (num_led) Number of LEDs in the strip
   */
  DotStar(int num_led);

    /**
   * @fn
   * destructor
   */
  ~DotStar();
  /**
   * @fn
   * Init SPI module
   */
  bool init();
  /**
   * @fn
   * Add LED data to the buffer
   * @param (buff) the front pointer where you want to put the data
   * @param (r) the brightness of the red LED (from 0 to 1)
   * @param (g) the brightness of the green LED (from 0 to 1)
   * @param (b) the brightness of the blue LED (from 0 to 1)
   * @param (global) the magnification of the brightness to all LEDs. (from 0 to 1)
   * @return the size of the data (4 bytes)
   */
  size_t _addLEDData(char* buff,double r=1, double g=1, double b=1, double global=1);

  /**
   * @fn
   * Add header to the buffer
   * @param (buff) the front pointer where you want to put the data
   * @return the size of the data (4 bytes)
   */
  size_t _addHeader(char* buff);
  /**
   * @fn
   * Add footer to the buffer
   * @param (buff) the front pointer where you want to put the data
   * @return the size of the data (4 bytes)
   */

  size_t _addFooter(char* buff);
  
  /**
   * @fn
   * set the led pixels
   * @param (data) the 2d array of the brightness data
   */
  void set(vector<vector<double> > &data);

  char _DecToBinary(int val);
  
  int led_n;
  char* buffer;
  size_t buffer_len;
};
