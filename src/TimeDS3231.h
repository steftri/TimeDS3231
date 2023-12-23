#ifndef _TIME_DS3231_H_
#define _TIME_DS3231_H_


#include "TimeDSXXXX.h"



#define DS3231_DEFAULT_ADDR 0x68


class TimeDS3231 : public TimeDSXXXX
{
public:
  TimeDS3231(uint8_t u8_WireAddr = DS3231_DEFAULT_ADDR);

  int32_t begin(void);
  
  int32_t readDateTime(uint16_t *pu16_Year, uint8_t *pu8_Month, uint8_t *pu8_Day,
                       uint8_t *pu8_Hour, uint8_t *pu8_Min, uint8_t *pu8_Sec, bool *pb_Valid = NULL);

  int32_t writeDateTime(const uint16_t u16_Year, const uint8_t u8_Month, const uint8_t u8_Day,
                        const uint8_t u8_Hour, const uint8_t u8_Min, const uint8_t u8_Sec);

  int32_t setAlarm(const uint8_t u8_Number, const bool b_Enable, const uint8_t u8_Hour = 0, const uint8_t u8_Min = 0);
};



#endif // _TIME_DS3231_H_

