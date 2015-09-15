/*
  KiboCiP1.cpp - Library for Kibo CiP-1.
  Created by Macnica, August 31, 2015.
*/

#include <KiboCiP1.h>

void KiboCiP1::begin()
{
  uint16_t result;

  // OPT3001
  //Serial.println("<OPT3001>");
  // MANUFACTUREID_REG
  result = readRegister(OPT3001_I2C_SLVADR,0x7E);
  //Serial.print("\tMANUFACTUREID_REG : ");
  //Serial.println(result,HEX);
  // DEVICEID_REG
  result = readRegister(OPT3001_I2C_SLVADR,0x7F);
  //Serial.print("\tDEVICEID_REG : ");
  //Serial.println(result,HEX);
  // CONFIGURATION
  writeRegister(OPT3001_I2C_SLVADR,0x01,0xC400);
  result = readRegister(OPT3001_I2C_SLVADR,0x01);
  //Serial.print("\tCONFIG_REG : ");
  //Serial.println(result,HEX);


  // TMP007
  //Serial.println("<TMP007>");
  // MANUFACTUREID_REG
  result = readRegister(TMP007_I2C_SLVADR,0x1E);
  //Serial.print("\tMANUFACTUREID_REG : ");
  //Serial.println(result,HEX);
  // DEVICEID_REG
  result = readRegister(TMP007_I2C_SLVADR,0x1F);
  //Serial.print("\tDEVICEID_REG : ");
  //Serial.println(result,HEX);


  // HDC1000
  //Serial.println("<HDC1000>");
  // MANUFACTUREID_REG
  result = readRegister(HDC1000_I2C_SLVADR,0xFE);
  //Serial.print("\tMANUFACTUREID_REG : ");
  //Serial.println(result,HEX);
  // DEVICEID_REG
  result = readRegister(HDC1000_I2C_SLVADR,0xFF);
  //Serial.print("\tDEVICEID_REG : ");
  //Serial.println(result,HEX);
  // CONFIGURATION
  result = readRegister(HDC1000_I2C_SLVADR,0x02);
  //Serial.print("\tCONFIG_REG : ");
  //Serial.println(result,HEX);


  // LDC1612
  //Serial.println("<LDC1612>");
  // MANUFACTUREID_REG
  result = readRegister(LDC1612_I2C_SLVADR,0x7E);
  //Serial.print("\tMANUFACTUREID_REG : ");
  //Serial.println(result,HEX);
  // DEVICEID_REG
  result = readRegister(LDC1612_I2C_SLVADR,0x7F);
  //Serial.print("\tDEVICEID_REG : ");
  //Serial.println(result,HEX);
  // CONFIGURATION
  writeRegister(LDC1612_I2C_SLVADR,0x1C,0x8000);
  writeRegister(LDC1612_I2C_SLVADR,0x08,0x04CE);
  writeRegister(LDC1612_I2C_SLVADR,0x09,0x04CE);
  writeRegister(LDC1612_I2C_SLVADR,0x0C,0x0000);
  writeRegister(LDC1612_I2C_SLVADR,0x0D,0x0000);
  writeRegister(LDC1612_I2C_SLVADR,0x10,0x0013);
  writeRegister(LDC1612_I2C_SLVADR,0x11,0x0013);
  writeRegister(LDC1612_I2C_SLVADR,0x14,0x1002);
  writeRegister(LDC1612_I2C_SLVADR,0x15,0x1002);
  writeRegister(LDC1612_I2C_SLVADR,0x18,0x0001);
  writeRegister(LDC1612_I2C_SLVADR,0x1A,0x1601);
  writeRegister(LDC1612_I2C_SLVADR,0x1B,0x820C);
  writeRegister(LDC1612_I2C_SLVADR,0x1D,0x0200);
  writeRegister(LDC1612_I2C_SLVADR,0x1E,0xD000);
  writeRegister(LDC1612_I2C_SLVADR,0x1F,0xD000);

  return;
}

uint16_t KiboCiP1::readRegister(uint8_t slaveAdr, uint8_t registerName)
{
  int16_t lsb;
  int16_t msb;
  int16_t result;


  // Initialize Wire
  Wire.begin();

  /* Begin Transmission at address of device on bus */
  Wire.beginTransmission(slaveAdr);

  /* Send Pointer to register you want to read */
  Wire.write(registerName);

  /* Sends Stop */
  Wire.endTransmission(true);
  delay(15);

  /* Requests 2 bytes from Slave */
  Wire.requestFrom(slaveAdr, (uint8_t)2);

  /* Wait Until 2 Bytes are Ready*/
  while(Wire.available() < 2)	{}

  /* Read*/
  msb = Wire.read();
  lsb = Wire.read();
  result = (msb << 8) | lsb;

  return result;
}

void KiboCiP1::writeRegister(uint8_t slaveAdr, uint8_t registerName, uint16_t data)
{
  int8_t lsb = data & 0xFF;
  int8_t msb = data >> 8;

  // Initialize Wire
  Wire.begin();

  /* Begin Transmission at address of device on bus */
  Wire.beginTransmission(slaveAdr);

  /* Send Pointer to register you want to write */
  Wire.write(registerName);

  /* Write */
  Wire.write(msb);
  Wire.write(lsb);

  /* Sends Stop */
  Wire.endTransmission();

  return;
}

double KiboCiP1::getOpt3001AmbientLight() {
  uint16_t raw;
  double lux;

  raw = readRegister(OPT3001_I2C_SLVADR,0x00);
  //Serial.print("getOpt3001AmbientLight raw: ");
  //Serial.println(raw,HEX);

  lux = pow(2,((raw>>12)&0x000F)) * (raw&0x0FFF) / 100;

  return lux;
}

double KiboCiP1::getTmp007IrTemperature() {
  uint16_t raw;
  double objtemp;

  raw = readRegister(TMP007_I2C_SLVADR,0x03);
  //Serial.print("getTmp007IrTemperature raw: ");
  //Serial.println(raw,HEX);

  objtemp     = (raw &0x0001) == 0 ? (float)((raw >> 2) * 0.03125) : 0 ;
  return objtemp;
}

double KiboCiP1::getTmp007DieTemperature() {
  uint16_t raw;
  double dietemp;

  raw = readRegister(TMP007_I2C_SLVADR,0x01);
  //Serial.print("getTmp007DieTemperature raw: ");
  //Serial.println(raw,HEX);

  dietemp     = (raw &0x0001) == 0 ? (float)((raw >> 2) * 0.03125) : 0 ;
  return dietemp;
}

double KiboCiP1::getHdc1000Temperature() {
  uint16_t raw;
  double temperature;

  raw = readRegister(HDC1000_I2C_SLVADR,0x00);
  //Serial.print("getHdc1000Temperature raw: ");
  //Serial.println(raw,HEX);

  temperature  = (double)raw * 165 / 65536 - 40 ;
  return temperature;
}

double KiboCiP1::getHdc1000Humidity() {
  uint16_t raw;
  double humidity;

  raw = readRegister(HDC1000_I2C_SLVADR,0x01);
  //Serial.print("getHdc1000Humidity raw: ");
  //Serial.println(raw,HEX);

  humidity     = (double)raw * 100 / 65536 ;
  return humidity;
}

double KiboCiP1::getLdc1612InductiveCh0() {
  uint16_t msb;
  uint16_t lsb;
  double inductive;

  msb = readRegister(LDC1612_I2C_SLVADR,0x00);
  lsb = readRegister(LDC1612_I2C_SLVADR,0x01);
  //Serial.print("getLdc1612InductiveCh0 raw: ");
  //Serial.print(msb,HEX);
  //Serial.print(" ");
  //Serial.println(lsb,HEX);

  float outputcode;
  int  fullscaleResolution;
  float sensorFrequency;
  float d32;
  float d33;

  outputcode = ((msb & 0x0fff) << 16) | lsb ;
  fullscaleResolution = pow(2,28) - 1 ;
  sensorFrequency = 1 * (40 / 2) * (outputcode / fullscaleResolution + 0 /65536);
  d32 = 1500 * 0.000000000001 ;
  d33 = 1 / (d32 * (pow((sensorFrequency * 1000000 * 2 * 3.14), 2))) ;
  inductive = d33 * 1000000 ;

  //Serial.println(inductive,DEC);
  return inductive;
}

double KiboCiP1::getLdc1612InductiveCh1() {
  uint16_t msb;
  uint16_t lsb;
  double inductive;

  msb = readRegister(LDC1612_I2C_SLVADR,0x02);
  lsb = readRegister(LDC1612_I2C_SLVADR,0x03);
  //Serial.print("getLdc1612InductiveCh1 raw: ");
  //Serial.print(msb,HEX);
  //Serial.print(" ");
  //Serial.println(lsb,HEX);

  float outputcode;
  int  fullscaleResolution;
  float sensorFrequency;
  float d32;
  float d33;

  outputcode = ((msb & 0x0fff) << 16) | lsb ;
  fullscaleResolution = pow(2,28) - 1 ;
  sensorFrequency = 1 * (40 / 2) * (outputcode / fullscaleResolution + 0 /65536);
  d32 = 1500 * 0.000000000001 ;
  d33 = 1 / (d32 * (pow((sensorFrequency * 1000000 * 2 * 3.14), 2))) ;
  inductive = d33 * 1000000 ;

  //Serial.println(inductive,DEC);
  return inductive;
}
