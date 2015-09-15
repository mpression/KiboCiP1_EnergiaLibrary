/*
  KiboCiP1.h - Library for Kibo CiP-1.
  Created by Macnica, August 31, 2015.
*/
#ifndef kibocip1_h
#define kibocip1_h

#include "Energia.h"
#include "Wire.h"


#define LDC1612_I2C_SLVADR  (0x2A)
#define HDC1000_I2C_SLVADR  (0x40)
#define OPT3001_I2C_SLVADR  (0x44)
#define TMP007_I2C_SLVADR   (0x45)

class KiboCiP1
{
  public:
    void     begin();
    double   getOpt3001AmbientLight();
    double   getTmp007IrTemperature();
    double   getTmp007DieTemperature();
    double   getHdc1000Temperature();
    double   getHdc1000Humidity();
    double   getLdc1612InductiveCh0();
    double   getLdc1612InductiveCh1();

  private:
    uint16_t readRegister(uint8_t , uint8_t );
    void     writeRegister(uint8_t, uint8_t, uint16_t );

};

#endif
