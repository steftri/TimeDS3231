//
//  time_ds1307.cpp
//  TimeDS3231
//
//  Created by Stefan on 2021-07-13.
//

#include <Arduino.h>
#include <Wire.h>



#include "TimeDS1307.h"


#define ADDR_CTRL   0x07


TimeDS1307::TimeDS1307(uint8_t u8_WireAddr) : TimeDSXXXX(u8_WireAddr)
{
}



int32_t TimeDS1307::begin(void)
{
  uint8_t au8_Buffer[1]={0x10};

  Wire.begin();
  _write(ADDR_CTRL, 1, au8_Buffer);  // write CTRL

  return 0;
}



int32_t TimeDS1307::readDateTime(uint16_t *pu16_Year, uint8_t *pu8_Month, uint8_t *pu8_Day,
                     uint8_t *pu8_Hour, uint8_t *pu8_Min, uint8_t *pu8_Sec, bool *pb_Valid)
{
  uint8_t u8_Sec;
  bool b_Valid = true;
  
  TimeDSXXXX::readDateTime(pu16_Year, pu8_Month, pu8_Day, pu8_Hour, pu8_Min, &u8_Sec);
  
  if(pu8_Sec)
    *pu8_Sec = u8_Sec;

  if(u8_Sec == 80)
    b_Valid = false;
  
  if(pb_Valid)
    *pb_Valid = b_Valid;
  
  if(!b_Valid)
    return -1;  // time is not valid, the oscillator stopped in battery powered mode
  
  return 0;
}
