#ifndef _TIME_DS1307_H_
#define _TIME_DS1307_H_


#include "TimeDSXXXX.h"


#define DS3231_DEFAULT_ADDR 0x68


class TimeDS1307 : public TimeDSXXXX
{
public:
  TimeDS1307(uint8_t u8_WireAddr = DS3231_DEFAULT_ADDR);
  
  int32_t begin(void);
  
  int32_t readDateTime(uint16_t *pu16_Year, uint8_t *pu8_Month, uint8_t *pu8_Day,
                       uint8_t *pu8_Hour, uint8_t *pu8_Min, uint8_t *pu8_Sec, bool *pb_Valid = NULL);
};



#endif // _TIME_DS1307_H_
