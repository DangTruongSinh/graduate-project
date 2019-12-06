#include "LCD.h"
#include "string.h"

int8_t Nhan_dang_RFID(uint8_t * str )
{		int8_t p=0;
		if(str[0]==0x56 && str[1]==0xa1 && str[2]==0x6f && str[3]==0x1f && str[4]==0x87)
		{		LCD_SHOW_STRING(50,100,"Tran Van A  ", BLACK,WHITE);
				return 1;
		}
		else if(str[0]==0xe7 && str[1]==0xb1 && str[2]==0xc7 && str[3]==0x1c && str[4]==0x8d)
		{		LCD_SHOW_STRING(50,100,"Nguyen Van B", BLACK,WHITE);
				return 2;
		}
		else
		{		LCD_SHOW_STRING(50,100,"The chua co", BLACK,WHITE);
				return 0;
		}
}
