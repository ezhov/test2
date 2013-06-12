#include "stm8l15x.h"

#include "lcd.h"
#include "rtc.h"

#include "stm8l15x_gpio.h"
#include "stm8l15x_exti.h"

bool toggle;



INTERRUPT_HANDLER(UserHandler,9)
{
  resetTime();
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

void startGpio()
{
  GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_In_FL_IT);
  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
}

void delay (uint32_t interval)
{
  int i;
  for (i=0; i<interval; i++)
    ;
}


void valueToChars (uint8_t value, char *hiChar, 
  char *loChar)
{
  uint8_t valHi, valLo;
  *hiChar = ((value & 0xf0) >> 4)+ '0';
  *loChar = (value & 0x0f) + '0';
}

void putTimeOnLCD()
{
  char charHi, charLo;
  int i,j;

  int time[3];
  getTime(time);
  for (i=0,j=0; i<3; i++,j+=2)
  {
    valueToChars(time[i], &charHi, &charLo);
    writeChar(charHi, j, 0);
    writeChar(charLo, j+1, LCD_Flag_Colon);
  }
}


void writeTimeLoop ()
{
  while(TRUE)
  {
    putTimeOnLCD();
    delay (1000);
  }
}

int main( void )
{
  int i = 0, j;
  startGpio();
  startLcd();
  startRtc();
  
  enableInterrupts();

  writeChar('H', 0, 0);
  writeChar('E', 1, 0);
  writeChar('L', 2, 0);
  writeChar('L', 3, 0);
  writeChar('O', 4, 0);
  delay(50000);

  writeTimeLoop();

  return 0;
}
