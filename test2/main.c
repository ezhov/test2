#include "stm8l15x.h"

#include "stm8l15x_gpio.h"
#include "stm8l15x_exti.h"
#include "stm8l15x_lcd.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_clk.h"

extern bool toggle;

void startGpio()
{
  GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_In_FL_IT);
  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
}

void LCD_GLASS_Init(void)
{

  /* Enable LCD/RTC clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);

  //#ifdef USE_LSE
  //  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
  //#else
  //  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
  //#endif

  /* Initialize the LCD */
  LCD_Init(LCD_Prescaler_1, LCD_Divider_31, LCD_Duty_1_4, 
      LCD_Bias_1_3, LCD_VoltageSource_Internal);

  /* Mask register
  For declare the segements used.
  in the Discovery we use 0 to 23 segments. */
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
  LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0xff);

  /* To set contrast to mean value */
  LCD_ContrastConfig(LCD_Contrast_3V0);

  LCD_DeadTimeConfig(LCD_DeadTime_0);
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

  /* Enable LCD peripheral */ 
  LCD_Cmd(ENABLE);

  LCD_WriteRAM(LCD_RAMRegister_0, 0xff);
  ;
}

void startLcd()
{
  int i;
  long j;
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);


  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);

  LCD_Init(LCD_Prescaler_1, LCD_Divider_16, LCD_Duty_1_4, LCD_Bias_1_3, LCD_VoltageSource_Internal);
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xff);
  LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xff);
  LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0xff);
  LCD_Cmd(ENABLE);
  for (i=LCD_RAMRegister_0; i<=LCD_RAMRegister_21; i++)
  {
    LCD_WriteRAM(i, 0xff);
    for (j=0;j<500;j++)
      ;
  }

}

void mainLoop()
{
  while (1) {
    if (toggle) {
      GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
      GPIO_ToggleBits(GPIOC, GPIO_Pin_7);
      toggle = FALSE;
      LCD_WriteRAM(LCD_RAMRegister_0, 0x00);

    }
  }
}

int main( void )
{
  startGpio();
  startLcd();
  //LCD_GLASS_Init();
  enableInterrupts();
  mainLoop();
  return 0;
}
