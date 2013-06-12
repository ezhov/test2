enum
{
LCD_Flag_Colon = 0x01,
LCD_Flag_Point = 0x02
};

void startLcd();
void writeChar(char ch, int pos, int flags);
