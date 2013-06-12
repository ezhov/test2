#include "rtc.h"

#include "stm8l15x_rtc.h"

void resetTime()
{
  RTC_TimeTypeDef timeType;
  RTC_TimeStructInit(&timeType);
  timeType.RTC_Hours=0x00;
  timeType.RTC_Minutes=0x00;
  timeType.RTC_Seconds=0x00;
  RTC_SetTime(RTC_Format_BCD, &timeType);
}

void startRtc()
{
  RTC_InitTypeDef initType;
  RTC_StructInit(&initType);
  RTC_Init(&initType);
  RTC_WaitForSynchro();
}

void getTime(int time[3])
{
  RTC_TimeTypeDef timeType;
  RTC_TimeStructInit(&timeType);
  
  RTC_GetTime(RTC_Format_BCD, &timeType);
  time[0] = timeType.RTC_Hours;
  time[1] = timeType.RTC_Minutes;
  time[2] = timeType.RTC_Seconds;
}