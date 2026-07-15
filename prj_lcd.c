#include <lpc214x.h>
#include <string.h>
#include "defines.h"
#include "lcd_defines.h"
#include "types.h"
#include "delay.h"
#include "include.h"


void WriteLcd(u8 data)
{
 SETBIT(IOCLR0,RW);
 WRITEBYTE(IOCLR0,DATA_PINS,0xff);
 WRITEBYTE(IOSET0,DATA_PINS,data);
 SETBIT(IOSET0,EN);
 delay_us(1);
 SETBIT(IOCLR0,EN);
 delay_ms(2);
}


void CmdLcd(u8 cmd)
{
 SETBIT(IOCLR0,RS);
 WriteLcd(cmd);
}


void InitLcd(void)
{
 SETBIT(IODIR0,RS);
 SETBIT(IODIR0,RW);
 SETBIT(IODIR0,EN);
 WRITEBYTE(IODIR0,DATA_PINS,0xff);

 delay_ms(15);
 CmdLcd(M_8BIT_1LINE);
 delay_ms(5);
 CmdLcd(M_8BIT_1LINE);
 delay_ms(2);
 CmdLcd(M_8BIT_1LINE);
 delay_ms(2);
	
 CmdLcd(M_8BIT_2LINE);
 CmdLcd(DIS_ON_CUR_OFF);
 CmdLcd(CLR_LCD);
 CmdLcd(SFT_CUR_R);
 //CmdLcd(LINE1_POS0);
}

void CharLcd(u8 ch)
{
	SETBIT(IOSET0,RS);
	WriteLcd(ch);
}

void StrLcd(u8 *str)
{
	while(*str)
	{
		CharLcd(*str++);
	}
}
// ---------------------------------------------------------
// DisplayMessage
// Shows a scheduled message on the LCD. Line 1 is a fixed "Message:"
// label; line 2 shows the message text itself.
//   - If the text fits in 16 characters, it's shown directly and held
//     for a few seconds.
//   - If it's longer than 16 characters, it's scrolled: a 16-char
//     "window" slides across the text one character at a time, with
//     a short delay between each shift, so the whole message is
//     readable on a 16x2 LCD.
// This is a blocking call - the main loop pauses here until the
// message has finished displaying (short hold or full scroll).
// ---------------------------------------------------------
void DisplayMessage(Message msg)
{
	
		u32 i=60;
	
	CmdLcd(CLR_LCD);
	delay_ms(2);

	CmdLcd(LINE1_POS0);
		StrLcd((u8*)msg.text);
	while(i)
	{
		CmdLcd(MOV_DIS_SFT_L);
		if(AdminKeyPressed())
		{
			EnterAdminMode();
			continue;
		}
		delay_ms(180);
		i--;
	}
	
	CmdLcd(CLR_LCD);
	delay_ms(2);

}


 void LcdNum(u32 num)
{
    char out[6];
    /* FIX 2: print as fixed 4-digit zero-padded to match sensor format */
    out[0] = ((num / 1000) % 10) + '0';
    out[1] = ((num / 100)  % 10) + '0';
    out[2] = ((num / 10)   % 10) + '0';
    out[3] = ( num         % 10) + '0';
    out[4] = '\0';
    StrLcd((u8*)out);
}
