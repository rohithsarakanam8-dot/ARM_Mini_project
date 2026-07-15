#include <lpc214x.h>
#include "defines.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"
#include "kpm.h"
#include "rtc.h"
#include "include.h"
#include "admin.h"

// Change this to whatever passcode you want to protect admin mode with.
// Entered on the keypad as digits, then confirmed by pressing 'C'
// (this matches how ReadNum() already works in kpm.c).
#define ADMIN_PASSWORD 1234

// ---------------------------------------------------------
// InitAdminMode
// Sets up the two extra GPIO pins admin mode needs:
//   - ADMIN_SW  : input,  the physical switch that requests admin mode
//   - BUZZER    : output, gives audible feedback (beeps)
// Called once at boot, alongside InitLcd()/InitKPM()/InitRTC()/InitAdc().
// ---------------------------------------------------------
void InitAdminMode(void)
{
	CLRBIT(IODIR0,ADMIN_SW);   // 0 = input direction for the switch pin
	SETBIT(IODIR0,BUZZER);     // 1 = output direction for the buzzer pin
	SETBIT(IOCLR0,BUZZER);     // drive buzzer low (off) initially
}

// ---------------------------------------------------------
// Buzz
// Simple blocking beep: pin high for `ms` milliseconds, then low.
// Good enough for short confirmation/error beeps; not a tone generator.
// ---------------------------------------------------------
void Buzz(u32 ms)
{
	SETBIT(IOSET0,BUZZER);   // buzzer on
	delay_ms(ms);
	SETBIT(IOCLR0,BUZZER);   // buzzer off
}

// ---------------------------------------------------------
// AdminKeyPressed
// Polled once per main-loop iteration. Just reads the raw pin state,
// so it's a simple "is it pressed right now" check (no debouncing).
// ---------------------------------------------------------
u8 AdminKeyPressed(void)
{
	return (u8)STATUSBIT(IOPIN0,ADMIN_SW);
}

// ---------------------------------------------------------
// CheckPassword
// Prompts the user on the LCD, then blocks on ReadNum() until they
// type digits and press 'C' to submit. Compares against ADMIN_PASSWORD.
// On failure, shows an error, beeps, pauses briefly, and returns 0
// so the caller (EnterAdminMode) can bail back out to normal operation.
// ---------------------------------------------------------
u8 CheckPassword(void)
{
	u32 entered;

	CmdLcd(CLR_LCD);
	delay_ms(2);
	CmdLcd(LINE1_POS0);
	StrLcd((u8*)"Enter Password:");
	CmdLcd(LINE2_POS0);

	entered = ReadNum();   // blocks here until user enters digits + 'C'

	if(entered==ADMIN_PASSWORD)
	{
		return 1;   // correct -> caller proceeds into the admin menu
	}

	// incorrect password: tell the user, beep, and give a moment to read it
	CmdLcd(CLR_LCD);
	delay_ms(2);
	CmdLcd(LINE1_POS0);
	StrLcd((u8*)"Wrong Password");
	Buzz(500);
	delay_s(1);
	return 0;
}

// ---------------------------------------------------------
// EditRTCTimeInteractive
// Walks the user through setting a new hour and minute, one prompt
// at a time, using the same keypad + ReadNum() pattern as the password.
// Values are clamped to valid ranges so a mistyped entry can't set an
// invalid time. Seconds are reset to 0 since the user isn't asked for
// them (keeps entry quick and avoids drift confusion).
// ---------------------------------------------------------
void EditRTCTimeInteractive(void)
{
	s32 h,m,s,d,mo,y,day;
	u8 choice;
	u32 val;

	// initial menu draw
	CmdLcd(CLR_LCD);
	delay_ms(2);
	CmdLcd(LINE1_POS0);
	StrLcd((u8*)"1:H 2.MI 3.S 4.D");
	CmdLcd(LINE2_POS0);
	StrLcd((u8*)"5.M 6.Y 7:DA 8.E");
	while(1)
	{
		choice = KeyScan();   // blocks until a key is pressed and released

		if(choice=='1')
		{
				// --- get hour ---

	CmdLcd(CLR_LCD);
	delay_ms(2);
	CmdLcd(LINE1_POS0);
	StrLcd((u8*)"Hour(0-23) C:");
	CmdLcd(LINE2_POS0);
	val = ReadNum();
	if(val>23) val=23;         // clamp bad input instead of writing garbage to RTC
	h=(s32)val;
			
			SetRTCTimeInfo(h,m,s);
		}
		else if(choice=='2')
		{
	// --- get minute ---
	CmdLcd(CLR_LCD);
	delay_ms(2);
	CmdLcd(LINE1_POS0);
	StrLcd((u8*)"Min(0-59) C:");
	CmdLcd(LINE2_POS0);
	val = ReadNum();
	if(val>59) val=59;
	m=(s32)val;
			
			SetRTCTimeInfo(h,m,s);
		}
		else if(choice=='3')
		{
				// --- get seconds ---
			CmdLcd(CLR_LCD);
	delay_ms(2);
	CmdLcd(LINE1_POS0);
	StrLcd((u8*)"Sec(0-59) C:");
	CmdLcd(LINE2_POS0);
	val = ReadNum();
	if(val>59) val=59;
	s=(s32)val;
			
			SetRTCTimeInfo(h,m,s);
		}
		else if(choice=='4')
		{	
			// --- get day ---
			CmdLcd(CLR_LCD);
	delay_ms(2);
			CmdLcd(LINE1_POS0);
	StrLcd((u8*)"day(1-31) C:");
	CmdLcd(LINE2_POS0);
	val = ReadNum();
	if(val>31 && val>0) val=31;
	d=(s32)val;
			
			SetdateInfo(d,mo,y);
		}
		else if(choice=='5')
		{
				// --- get month ---

			CmdLcd(CLR_LCD);
	delay_ms(2);
			CmdLcd(LINE1_POS0);
	StrLcd((u8*)"Month(1-12) C:");
	CmdLcd(LINE2_POS0);
	val = ReadNum();
	if(val>12 && val>0) val=12;
	mo=(s32)val;
			
			SetdateInfo(d,mo,y);
		} 
		else if(choice=='6')
		{
				// --- get year ---

			CmdLcd(CLR_LCD);
	delay_ms(2);
			CmdLcd(LINE1_POS0);
	StrLcd((u8*)"year(2000-2999) C:");
	CmdLcd(LINE2_POS0);
	val = ReadNum();
	if(val>2999 && val>=2000) val=2999;
	y=(s32)val;
			
			SetdateInfo(d,mo,y);
		}
		else if(choice=='7')
		{
				// --- get day of week ---
			CmdLcd(CLR_LCD);
	delay_ms(2);
			CmdLcd(LINE1_POS0);
	StrLcd((u8*)"day(0-6) C:");
	CmdLcd(LINE2_POS0);
	val = ReadNum();
	if(val>7) val=6;
	day=(s32)val;
			
			SetdayInfo(day);
		}
			else if(choice=='8')
		{
			break;   // leave the while(1) loop -> exit admin mode
		}
		else
		{
			continue;   // unrecognized key: skip the menu redraw below and re-read
		}
  // confirm to the user
	CmdLcd(CLR_LCD);
	delay_ms(2);
	CmdLcd(LINE1_POS0);
	StrLcd((u8*)"Time & date Updated!");
	Buzz(150);
	delay_s(1);
		
		// redraw the menu after returning from either sub-screen
		// initial menu draw
	CmdLcd(CLR_LCD);
	delay_ms(2);
	CmdLcd(LINE1_POS0);
	StrLcd((u8*)"1:H 2.MI 3.S 4.D");
	CmdLcd(LINE2_POS0);
	StrLcd((u8*)"5.M 6.Y 7:DA 8.E");
	}	

}

// ---------------------------------------------------------
// EditMessagesMenu
// Lets the user browse the fixed message list and flip each one's
// "enabled" flag on/off. This directly controls whether main1.c's
// scheduling loop will ever display that message.
//
// Controls:
//   '#' -> next message      (wraps from last back to first)
//   '*' -> previous message  (wraps from first back to last)
//   '5' -> toggle enabled/disabled for the message shown
//   '=' -> save (changes are already live in messageList[]) and exit
// ---------------------------------------------------------
void EditMessagesMenu(void)
{
	s32 idx=0;          // index of the message currently being shown
	u8 exitMenu=0;

	while(!exitMenu)
	{
		char preview[17];   // 16 visible chars + null terminator, for line 2
		u32 k;
		u8 key;

		CmdLcd(CLR_LCD);
		delay_ms(2);

		// Line 1: message number (1-based for the user) + ON/OFF status
		CmdLcd(LINE1_POS0);
		StrLcd((u8*)"Msg ");
		U32Lcd((u32)(idx+1));
		StrLcd((u8*)": ");
		StrLcd(messageList[idx].enabled ? (u8*)"ON " : (u8*)"OFF");

		// Line 2: first 16 characters of the message text as a preview
		// (full scrolling text is only shown when the message actually fires)
		for(k=0;k<16;k++)
		{
			preview[k] = messageList[idx].text[k] ? messageList[idx].text[k] : ' ';
		}
		preview[16]='\0';
		CmdLcd(LINE2_POS0);
		StrLcd((u8*)preview);

		key=KeyScan();   // blocks until a key is pressed and released

		if(key=='#')          // next message
		{
			idx++;
			if(idx>=TOTAL_MESSAGES) idx=0;   // wrap around
		}
		else if(key=='*')     // previous message
		{
			idx--;
			if(idx<0) idx=TOTAL_MESSAGES-1;  // wrap around
		}
		else if(key=='5')     // toggle enabled/disabled for current message
		{
			messageList[idx].enabled = !messageList[idx].enabled;
			Buzz(80);   // short beep confirms the toggle registered
		}
		else if(key=='=')     // done editing, return to admin menu
		{
			exitMenu=1;
		}
		// any other key is ignored and the loop just redraws
	}
}

// ---------------------------------------------------------
// EnterAdminMode
// Entry point called from main1.c the moment the admin switch is
// detected as pressed. Flow:
//   1. Beep to acknowledge the switch press.
//   2. Require a correct password (CheckPassword) - bail out if wrong.
//   3. Show a simple menu and dispatch to time editing or message
//      editing based on keypad input, redrawing the menu after each.
//   4. 'C' at the menu level exits admin mode back to normal operation.
// ---------------------------------------------------------
void EnterAdminMode(void)
{
	u8 choice;

	Buzz(100);   // acknowledge that the switch press was registered

	if(!CheckPassword())
	{
		return;   // wrong password -> straight back to normal display
	}

	// initial menu draw
	CmdLcd(CLR_LCD);
	delay_ms(2);
	CmdLcd(LINE1_POS0);
	StrLcd((u8*)"1:RTC Edit");
	CmdLcd(LINE2_POS0);
	StrLcd((u8*)"2.Msgs 3:Exit ");

	while(1)
	{
		choice = KeyScan();   // blocks until a key is pressed and released

		if(choice=='1')
		{
			EditRTCTimeInteractive();
		}
		else if(choice=='2')
		{
			EditMessagesMenu();
		}
		else if(choice=='3')
		{
			break;   // leave the while(1) loop -> exit admin mode
		}
		else
		{
			continue;   // unrecognized key: skip the menu redraw below and re-read
		}

		// redraw the menu after returning from either sub-screen
		CmdLcd(CLR_LCD);
		delay_ms(2);
		CmdLcd(LINE1_POS0);
		StrLcd((u8*)"1:RTC Edit");
		CmdLcd(LINE2_POS0);
		StrLcd((u8*)"2.Msgs 3:Exit");
	}

	// leaving admin mode: clear the screen so the main loop starts fresh
	CmdLcd(CLR_LCD);
	delay_ms(2);
}
