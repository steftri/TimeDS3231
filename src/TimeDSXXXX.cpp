#include <Arduino.h>
#include <Wire.h>


#include "TimeDSXXXX.h"


#define ADDR_SEC           0x00
#define ADDR_MIN           0x01
#define ADDR_HOUR          0x02
#define ADDR_DAYOFWEEK     0x03
#define ADDR_DATE          0x04
#define ADDR_CENTURY_MONTH 0x05
#define ADDR_YEAR          0x06

#define BIT_HOUR_1224      0x40
#define BIT_HOUR_AMPM      0x20



TimeDSXXXX::TimeDSXXXX(uint8_t u8_WireAddr)
{
  mu8_WireAddr = u8_WireAddr;
}



int32_t TimeDSXXXX::end(void)
{
  return 0;
}



int32_t TimeDSXXXX::readDateTime(uint16_t *pu16_Year, uint8_t *pu8_Month, uint8_t *pu8_Day,
                     uint8_t *pu8_Hour, uint8_t *pu8_Min, uint8_t *pu8_Sec)
{
  uint8_t au8_Buffer[7]={0,0,0,0,0,0,0};
  
  _read(0x00, 7, au8_Buffer);  // read date and time
  if(pu8_Sec)
    *pu8_Sec=(au8_Buffer[ADDR_SEC]>>4)*10+(au8_Buffer[ADDR_SEC]&0xf);
  if(pu8_Min)
    *pu8_Min=(au8_Buffer[ADDR_MIN]>>4)*10+(au8_Buffer[ADDR_MIN]&0xf);
  if(pu8_Hour)
  {
    if(!(au8_Buffer[ADDR_HOUR]&BIT_HOUR_1224)) // 24h format
      *pu8_Hour=((au8_Buffer[ADDR_HOUR]>>4)&0x3)*10+(au8_Buffer[ADDR_HOUR]&0xf);
    else
      *pu8_Hour=((au8_Buffer[ADDR_HOUR]>>5)&1)*12+((au8_Buffer[ADDR_HOUR]>>4)&0x1)*10+(au8_Buffer[ADDR_HOUR]&0xf);
  }
  if(pu8_Day)
    *pu8_Day=(au8_Buffer[ADDR_DATE]>>4)*10+(au8_Buffer[ADDR_DATE]&0xf);
  if(pu8_Month)
    *pu8_Month=((au8_Buffer[ADDR_CENTURY_MONTH]>>4)&0x1)*10+(au8_Buffer[ADDR_CENTURY_MONTH]&0xf);
  if(pu16_Year)
    *pu16_Year=2000+(au8_Buffer[ADDR_CENTURY_MONTH]>>7)*100+(au8_Buffer[ADDR_YEAR]>>4)*10+(au8_Buffer[ADDR_YEAR]&0xf);
  
  return 0;
}




int32_t TimeDSXXXX::writeDateTime(const uint16_t u16_Year, const uint8_t u8_Month, const uint8_t u8_Day,
                     const uint8_t u8_Hour, const uint8_t u8_Min, const uint8_t u8_Sec)
{
  uint8_t au8_Buffer[7]={0,0,0,0,0,0,0};
  
  au8_Buffer[ADDR_SEC]       = ((u8_Sec/10)<<4) | (u8_Sec%10);
  au8_Buffer[ADDR_MIN]       = ((u8_Min/10)<<4) | (u8_Min%10);
  au8_Buffer[ADDR_HOUR]      = ((u8_Hour/10)<<4) | (u8_Hour%10);
  au8_Buffer[ADDR_DAYOFWEEK] = 0;
  au8_Buffer[ADDR_DATE]      = ((u8_Day/10)<<4) | (u8_Day%10);
  au8_Buffer[ADDR_CENTURY_MONTH] = (((u16_Year/100)&1)<<7) | ((u8_Month/10)<<4) | (u8_Month%10);
  au8_Buffer[ADDR_YEAR]      = (((u16_Year%100)/10)<<4) | (u16_Year%10);
  _write(0x00, 7, au8_Buffer);
  
  return 0;
}





int32_t TimeDSXXXX::_write(const uint8_t u8_Address, const uint8_t u8_BufferSize, const uint8_t *pu8_Buffer)
{
  Wire.beginTransmission(mu8_WireAddr);
  Wire.write(u8_Address);           // write data to address
  for(uint8_t i=0; i<u8_BufferSize; i++)
  {
    Wire.write(pu8_Buffer?pu8_Buffer[i]:0); // write zeros if pu8_Buffer is NULL
  }
  Wire.endTransmission();

  return 0;
}



int32_t TimeDSXXXX::_read(const uint8_t u8_Address, const uint8_t u8_BufferSize, uint8_t *pu8_Buffer)
{
  Wire.beginTransmission(mu8_WireAddr);
  Wire.write(u8_Address);
  Wire.endTransmission();
  
  Wire.requestFrom(mu8_WireAddr, u8_BufferSize);
  for(uint8_t i=0; Wire.available() && i<u8_BufferSize; i++)
    pu8_Buffer[i] = (uint8_t)Wire.read();

  return 0;
}


