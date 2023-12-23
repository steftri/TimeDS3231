//
//  time_ds3231.cpp
//  TimeDS3231
//
//  Created by Stefan on 2021-07-13.
//

#include <Arduino.h>
#include <Wire.h>


#include "TimeDS3231.h"


#define ADDR_ALARM1_MIN    0x08
#define ADDR_ALARM1_HOUR   0x09
#define ADDR_ALARM1_DAY    0x0A

#define ADDR_ALARM2_MIN    0x0B
#define ADDR_ALARM2_HOUR   0x09
#define ADDR_ALARM2_DAY    0x0A

#define ADDR_CTRL          0x0E
#define ADDR_STATUS        0x0F



#define BIT_STATUS_OSF     0x80
#define BIT_STATUS_EN32kHz 0x08

#define BIT_CTRL_INTCN     0x04
#define BIT_CTRL_A2IE      0x02
#define BIT_CTRL_A1IE      0x01



TimeDS3231::TimeDS3231(uint8_t u8_WireAddr) : TimeDSXXXX(u8_WireAddr)
{
}



int32_t TimeDS3231::begin(void)
{
  uint8_t au8_Buffer[2]={0,0};
  
  Wire.begin();

  _read(ADDR_STATUS, 1, &au8_Buffer[1]);
  au8_Buffer[1] &= BIT_STATUS_OSF;  // clear all bits exept the Oscillator Stop Flag
  au8_Buffer[1] |= BIT_STATUS_EN32kHz;  // enable 32kHz
  _write(ADDR_CTRL, 2, au8_Buffer);  // write both CTRL and STATUS
  
  return 0;
}



int32_t TimeDS3231::readDateTime(uint16_t *pu16_Year, uint8_t *pu8_Month, uint8_t *pu8_Day,
                     uint8_t *pu8_Hour, uint8_t *pu8_Min, uint8_t *pu8_Sec, bool *pb_Valid)
{
  uint8_t au8_Buffer[1]={0};
  
  TimeDSXXXX::readDateTime(pu16_Year, pu8_Month, pu8_Day, pu8_Hour, pu8_Min, pu8_Sec);

  _read(0x0F, 1, au8_Buffer);   // read status register to check for valid time
  if(pb_Valid)
    *pb_Valid = (au8_Buffer[0]&BIT_STATUS_OSF)?false:true;
  
  if(au8_Buffer[0]&BIT_STATUS_OSF)
    return -1;  // time is not valid, the oscillator stopped in battery powered mode
  
  return 0;
}



int32_t TimeDS3231::writeDateTime(const uint16_t u16_Year, const uint8_t u8_Month, const uint8_t u8_Day,
                     const uint8_t u8_Hour, const uint8_t u8_Min, const uint8_t u8_Sec)
{
  uint8_t au8_Buffer[1]={0};
  
  TimeDSXXXX::writeDateTime(u16_Year, u8_Month, u8_Day, u8_Hour, u8_Min, u8_Sec);
  
  _read(ADDR_STATUS, 1, au8_Buffer);  // reset Oscillator Stop Flag
  if(au8_Buffer[0]&BIT_STATUS_OSF)
  {
    au8_Buffer[0] &= ~BIT_STATUS_OSF;
    _write(ADDR_STATUS, 1, au8_Buffer);
  }
  
  return 0;
}



int32_t TimeDS3231::setAlarm(const uint8_t u8_Number, const bool b_Enable, const uint8_t u8_Hour, const uint8_t u8_Min)
{
  uint8_t au8_Buffer[3]={0,0,0};
 
  au8_Buffer[0] = 0x00 | ((u8_Min/10)<<4) | (u8_Min%10);
  au8_Buffer[1] = 0x80 | ((u8_Hour/10)<<4) | (u8_Hour%10);
  au8_Buffer[2] = 0x80;

  _write((u8_Number==0)?ADDR_ALARM1_MIN:ADDR_ALARM2_MIN, 3, au8_Buffer);
  
  _read(ADDR_CTRL, 1, au8_Buffer);
  au8_Buffer[0] |= BIT_CTRL_INTCN;
  if(b_Enable)
    au8_Buffer[0] |= (u8_Number==0)?BIT_CTRL_A1IE:BIT_CTRL_A2IE;
  else
    au8_Buffer[0] &= ~(uint8_t)((u8_Number==0)?BIT_CTRL_A1IE:BIT_CTRL_A2IE);
  _write(ADDR_CTRL, 1, au8_Buffer);

  return 0;
}

