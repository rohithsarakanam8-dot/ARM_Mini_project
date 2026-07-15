#include <LPC214x.h>
#include "Lcd_defines.h"
#include "lcd.h"

#define PREFRAC_VAL  (PCLK-(PREINT +1)*32768)
#define PREINT_VAL ((PCLK/32768)-1)
#define CCR_RESET (1<<1) //1=reset of ctc 0=normal of ctc
#define RTC_ENABLE (1<<0)
#define RTC_RESET (1<<1)


u8 week[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

void InitRTC(void)
{
	CCR= RTC_RESET;
	PREINT=PREINT_VAL;
	PREFRAC=PREFRAC_VAL;
  CCR=RTC_ENABLE;
}
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
	*hour = HOUR;
	*minute = MIN;
	*second = SEC;
}
void DisplayRTCTime(s32 hour, s32 minute, s32 second)
{
	CmdLcd(LINE1_POS0);
	CharLcd((hour/10)+48);
	CharLcd((hour%10)+48);
	CharLcd(':');
	CharLcd((minute/10)+48);
	CharLcd((minute%10)+48);
	CharLcd(':');
	CharLcd((second/10)+48);		
	CharLcd((second%10)+48);	
}
void SetRTCTimeInfo(s32 hour, s32 minute, s32 second)
{
	HOUR = hour;
	MIN = minute;
	SEC = second;
}
void SetdateInfo(s32 date,s32 month,u32 year)
{
	DOM=date;
	MONTH=month;
	YEAR=year;
}
void GetdateInfo(s32 *date, s32 *month, u32 *year)
{
	*date = DOM;
	*month = MONTH;
	*year = YEAR;
}
void DisplayRTCDate(s32 date, s32 month, u32 year)
{
	CmdLcd(LINE2_POS0);
	CharLcd(date/10+48);
	CharLcd(date%10+48);
	CharLcd('/');
	CharLcd(month/10+48);
	CharLcd(month%10+48);
	CharLcd('/');
	U32Lcd(year);
}
void U32Lcd(u32 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
		{
			CharLcd('0');
			
		}
	else{
				while(n)
				{
					a[i++]=(n%10)+'0';
					n/=10;
				}
				for(--i; i>=0; i--)
				{
					CharLcd(a[i]);
				}
			}
		}
void SetdayInfo(u32 dow)
{
	DOW=dow;
}
void GetdayInfo(u32 *dow)
{
	*dow=DOW;
}
void Displayday(u32 dow)
{
	CmdLcd(LINE1_POS0+13);
	StrLcd(week[dow]);
}

