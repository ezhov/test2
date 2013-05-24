#include "stm8l15x.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_exti.h"

bool toggle;

INTERRUPT_HANDLER(UserHandler,9)
{
  toggle = TRUE;
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}


int main( void )
{
  GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_In_FL_IT);
  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
  enableInterrupts();
  while (1) {
    if (toggle) {
      GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
      GPIO_ToggleBits(GPIOC, GPIO_Pin_7);
      toggle = FALSE;
    }
  }
  return 0;
}
