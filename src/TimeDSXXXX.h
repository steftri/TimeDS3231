#ifndef _TIME_DSXXXX_H_
#define _TIME_DSXXXX_H_



class TimeDSXXXX
{
private:
  uint8_t mu8_WireAddr;
  
public:
  TimeDSXXXX(uint8_t u8_WireAddr);
  
  virtual int32_t begin(void) = 0;

  int32_t end(void);
  
  int32_t readDateTime(uint16_t *pu16_Year, uint8_t *pu8_Month, uint8_t *pu8_Day,
                       uint8_t *pu8_Hour, uint8_t *pu8_Min, uint8_t *pu8_Sec);
  
  int32_t writeDateTime(const uint16_t u16_Year, const uint8_t u8_Month, const uint8_t u8_Day,
                       const uint8_t u8_Hour, const uint8_t u8_Min, const uint8_t u8_Sec);
  
protected:
  int32_t _write(const uint8_t u8_Address, const uint8_t u8_BufferSize, const uint8_t *pu8_Buffer);
  int32_t _read(const uint8_t u8_Address, const uint8_t u8_BufferSize, uint8_t *pu8_Buffer);
};



#endif // _TIME_DSXXXX_H_
