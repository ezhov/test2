#include "lcd.h"

#include "stm8l15x.h"

#include "stm8l15x_lcd.h"
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

const uint16_t CapLetterMap[26]=
    {
        /* A      B      C      D      E      F      G      H      I  */
        0xFE00,0x6711,0x1d00,0x4711,0x9d00,0x9c00,0x3f00,0xfa00,0x0011,
        /* J      K      L      M      N      O      P      Q      R  */
        0x5300,0x9844,0x1900,0x5a42,0x5a06,0x5f00,0xFC00,0x5F04,0xFC04,
        /* S      T      U      V      W      X      Y      Z  */
        0xAF00,0x0411,0x5b00,0x18c0,0x5a84,0x00c6,0x0052,0x05c0
    };
const uint16_t NumberMap[10]=
    {
        /* 0      1      2      3      4      5      6      7      8      9  */
        0x5F00,0x4200,0xF500,0x6700,0xEa00,0xAF00,0xBF00,0x04600,0xFF00,0xEF00
    };

const uint16_t CapLetterMapRu[4] = 
{
  /* æ î ï à */
  0x00d7, 0x5F00, 0x5e00, 0xFE00
};

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

static void writeRep(uint16_t ch, uint8_t pos)
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

void writeChar(char ch, int pos, int flags)
{
  uint16_t modifier = 0x00;
  if (flags & LCD_Flag_Colon)
    modifier |= 0x0020;
  if (flags & LCD_Flag_Point)
    modifier |= 0x0008;
  if (ch >= '0' && ch <= '9')
    writeRep(NumberMap[ch-'0'] | modifier, pos);
  else if (ch >= 'a' && ch <= 'z')
    writeRep(CapLetterMap[ch-'a'] | modifier, pos);
  else if (ch >= 'A' && ch <= 'Z')
    writeRep(CapLetterMap[ch-'A'] | modifier, pos);
}
