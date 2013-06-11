#include "stm8l15x.h"

#include "lcd.h"

#include "stm8l15x_gpio.h"
#include "stm8l15x_exti.h"

bool toggle;

INTERRUPT_HANDLER(UserHandler,9)
{
  toggle = TRUE;
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

void startGpio()
{
  GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_In_FL_IT);
  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
}


void mainLoop()
{
  while (1) {
    if (toggle) {
      GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
      GPIO_ToggleBits(GPIOC, GPIO_Pin_7);
      toggle = FALSE;
    }
  }
}

void delay (uint32_t interval)
{
  int i;
  for (i=0; i<interval; i++)
    ;
}

const uint16_t CapLetterMap[26]=
    {
        /* A      B      C      D      E      F      G      H      I  */
        0xFE00,0x6711,0x1d00,0x4711,0x9d00,0x9c00,0x3f00,0xfa00,0x0011,
        /* J      K      L      M      N      O      P      Q      R  */
        0x5300,0x9844,0x1900,0x5a42,0x5a06,0x5f00,0xFC00,0x5F04,0xFC04,
        /* S      T      U      V      W      X      Y      Z  */
        0xAF00,0x0411,0x5b00,0x18c0,0x5a84,0x00c6,0x0052,0x05c0
    };

int main( void )
{
  int i = 0, j;
  startGpio();
  startLcd();


  while (TRUE)
  {
    for (j=0; j<6; j++)
      writeChar(CapLetterMap[i], j);
    delay(10000);
    for (j=0; j<6; j++)
      writeChar(0x0000, j);
    i++;
    if (i == 26)
      i=0;
  }

  enableInterrupts();
  mainLoop();
  return 0;
}
