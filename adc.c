#include <LPC214x.h>
#include <string.h>
#include "lcd.h"
#include "types.h"
#include "lcd_defines.h"
#include "delay.h"
#include "defines.h"


void InitAdc(void)
{
	PINSEL1|=AIN0;
	AD0CR=PDN_BIT|CLKDIV;
}

void Read_ADC(u32 chno,f32 *eAR, u32 *AdcDval)
{
	AD0CR&=~(255<<0);
	AD0CR|=CH0|START_CONV;
	delay_us(3);
	while(((AD0GDR>>DONE_BIT)&1)==0);
	AD0CR&=~(START_CONV);
	*AdcDval=((AD0GDR>>RESULT)&1023);
	*eAR=(3.3/1023)*(*AdcDval);	
}

void Read_temp(s32 *temp, f32 *eAR)
{
	*temp= (*eAR *100)+3;
}
	
void Display_temp(s32 *temp)
{
	CmdLcd(LINE2_POS0+12);
	U32Lcd(*temp);
	CharLcd(0xDF);
	CharLcd('C');
}

