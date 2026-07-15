#include <lpc214x.h>
#include "lcd.h"       // Your LCD driver
#include "delay.h"     // Millisecond delay
#include "defines.h"    // Optional keypad driver
#include "adc.h"
#include "rtc.h"
#include "kpm.h"
#include "lcd_defines.h"
#include "include.h"   // Message struct, TOTAL_MESSAGES, DisplayMessage()
#include "admin.h"      // admin switch / password / RTC edit / message edit

// Fixed schedule of messages. hour/minute is the trigger time (24hr),
// enabled controls whether it's currently active (can be toggled live
// from admin mode via EditMessagesMenu -> this array is shared, not copied).
Message messageList[TOTAL_MESSAGES] = {
    {7, 0, "Good Morning! Start your day!", 1},
    {7, 15, "ARM kit issue time from 7:30am -8am in middle lab", 1},
    {7, 10, "ARM workshop in lab1 at 7:30am", 1},
    {9, 45, "C progrmaming session in 5th floor", 1},
    {10, 0, "C programming session is ongoing", 1},
		{11,20,"break time from 11:15am-11:30am",1},
		{11,25,"ARM session starts soon",1},
		{11,30,"ARM session is ongoing",1},
		{13,0,"Revise today's class programs at home!",1},
		{13,5,"End of Day - See You Tomorrow!",1}
};

// Shared state read from the RTC/ADC each loop iteration
s32 hour,min,sec,date,month,temp;
u32 dow,chno,AdcDval,year;

f32 eAR;      // ADC voltage reading from LM35
int i;        // loop index for scanning messageList

int main()
{
	// LEDs are outputs: Green = idle/normal display, Red = message shown
	SETBIT(IODIR0,Red_Led);
	SETBIT(IODIR0,Green_Led);

	InitLcd();
	InitKPM();
	InitRTC();
	InitAdc();
	InitAdminMode();   // configure admin switch (input) + buzzer (output)

	while(1)
	{
		u8 messageShown = 0;   // tracks whether a scheduled message fired this cycle

		// --- Admin mode check ---
		// If the admin switch is pressed, hand off control to EnterAdminMode()
		// (password check, then time/message editing) and skip the rest of
		// this loop iteration so we don't also try to update the normal display.
		if(AdminKeyPressed())
		{
			EnterAdminMode();
			continue;
		}

		// --- Read current RTC state ---
		GetdayInfo(&dow);
		GetRTCTimeInfo(&hour,&min,&sec);
		GetdateInfo(&date,&month,&year);

		// --- Check if any enabled message matches the current time ---
		for(i=0;i<TOTAL_MESSAGES;i++){
			if((messageList[i].enabled) && (messageList[i].hour==HOUR) &&(messageList[i].minute==MIN))
			{
				messageShown = 1;
				SETBIT(IOCLR0,Red_Led);     // red off
				SETBIT(IOSET0,Green_Led);   // green on: "active schedule display"
				DisplayMessage(messageList[i]);   // handles its own scrolling/timing
				break;   // only show the first matching message this minute
			}
		}

		// --- Idle mode: no message scheduled right now ---
		// Show clock/date/day + LM35 temperature instead, with red LED on
		// to indicate "idle display mode" per the spec.
		if(!messageShown)
		{
			SETBIT(IOCLR0,Green_Led);   // green off
			SETBIT(IOSET0,Red_Led);     // red on: "idle display"

			delay_ms(2);
			DisplayRTCTime(hour,min,sec);
			Displayday(dow);
			DisplayRTCDate(date,month,year);

			Read_ADC(chno,&eAR,&AdcDval);
			Read_temp(&temp,&eAR);
			Display_temp(&temp);
		}
	}
}
