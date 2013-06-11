#include "stm8l15x.h"

#include "stm8l15x_lcd.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_clk.h"

/*  =========================================================================
                                 LCD MAPPING
    =========================================================================
	    A
     _  ----------
COL |_| |\   |J  /|
       F| H  |  K |B
     _  |  \ | /  |
COL |_| --G-- --M--
        |   /| \  |
       E|  Q |  N |C
     _  | /  |P  \|   
DP  |_| -----------  
	    D         

 An LCD character coding is based on the following matrix:
      { E , D , P   , J  }
      { M , C , COL , H  }
      { B , A , K   , N  }
      { G , F , Q   , DP }

 The character 'A' for example is:
  -------------------------------
LSB   { 1 , 0 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
MSB   { 1 , 1 , 0 , 0   }
      -------------------
  'A' =  F    E   0   0 hexa

*/

void startLcd()
{
  int i;
  long j;
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);


  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);

  LCD_Init(LCD_Prescaler_1, LCD_Divider_31, LCD_Duty_1_4, LCD_Bias_1_3, LCD_VoltageSource_Internal);
  LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xff);
  LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xff);
  LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0xff);
  LCD_ContrastConfig(LCD_Contrast_3V0);
  LCD_DeadTimeConfig(LCD_DeadTime_0);
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

  LCD_Cmd(ENABLE);
}

typedef struct pixel_t {
  uint8_t reg;
  uint8_t shift;} pixel_t;
  
  
pixel_t digit[][] = 
{
{
  LCD_RAMRegister_2, 6, LCD_RAMRegister_0, 0,
  LCD_RAMRegister_6, 2, LCD_RAMRegister_3, 4,
  LCD_RAMRegister_9, 6, LCD_RAMRegister_7, 0,
  LCD_RAMRegister_10, 4, LCD_RAMRegister_13, 2,
},
{
  LCD_RAMRegister_2, 4, LCD_RAMRegister_0, 2,
  LCD_RAMRegister_6, 0, LCD_RAMRegister_3, 6,
  LCD_RAMRegister_9, 4, LCD_RAMRegister_7, 2, 
  LCD_RAMRegister_10, 6, LCD_RAMRegister_13, 0,
},  
{
  LCD_RAMRegister_2, 2, LCD_RAMRegister_0, 4,
  LCD_RAMRegister_5, 6, LCD_RAMRegister_4, 0,
  LCD_RAMRegister_9, 2, LCD_RAMRegister_7, 4,
  LCD_RAMRegister_11, 0, LCD_RAMRegister_12, 6,
},
{
  LCD_RAMRegister_2, 0, LCD_RAMRegister_0, 6,
  LCD_RAMRegister_5, 4, LCD_RAMRegister_4, 2,
  LCD_RAMRegister_9, 0, LCD_RAMRegister_7, 6,
  LCD_RAMRegister_11, 2, LCD_RAMRegister_12, 4,
},
{
  LCD_RAMRegister_1, 6, LCD_RAMRegister_1, 0,
  LCD_RAMRegister_5, 2, LCD_RAMRegister_4, 4,
  LCD_RAMRegister_8, 6, LCD_RAMRegister_8, 0,
  LCD_RAMRegister_11, 4, LCD_RAMRegister_12, 2,
},
{
  LCD_RAMRegister_1, 4, LCD_RAMRegister_1, 2,
  LCD_RAMRegister_5, 0, LCD_RAMRegister_4, 6,
  LCD_RAMRegister_8, 4, LCD_RAMRegister_8, 2,
  LCD_RAMRegister_11, 6, LCD_RAMRegister_12, 0,
},
};

void writeChar(uint16_t ch, uint8_t pos)
{
  char c[8];
  int i,j;
  for (i=14,j=0; j<8; j++, i-=2)
  {
    c[j] = (ch & (0x3 << i)) >> i;
  }
  for (i=0; i<8; i++)
  {
    LCD->RAM[digit[pos][i].reg] &= ~(0x3 << digit[pos][i].shift);
    LCD->RAM[digit[pos][i].reg] |= c[i] << digit[pos][i].shift;
  }
}
