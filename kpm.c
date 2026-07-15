#include <LPC214x.h>
#include "defines.h"
#include "types.h"
#include "lcd.h"

u8 kpno[4][4]={{'1','2','3','-'},{'4','5','6','/'},{'7','8','9','+'},{'*','0','#','='}};

void InitKPM(void)
{
	//cfg row pins as outputs(p1.16 to p1.19)
	WRITENIBBLE(IODIR1,16,0xf);
}	

u8 ColScan(void)
{
	if((READNIBBLE(IOPIN1,20))<0xf)
		return 0;
	
	else 
		return 1;
}

u8 RowCheck(void)
{
	u8 rno;
	for(rno=0; rno<4; rno++)
	{
		WRITENIBBLE(IOPIN1,16,~(1<<rno));
		if(ColScan()==0)
		{
			break;
		}
	}
	WRITENIBBLE(IOPIN1,16,0x0);
	return rno;
}

u8 ColCheck(void)
{
	u8 cno;
	for(cno=0; cno<4; cno++)
	{
		if(STATUSBIT(IOPIN1,(20+cno))==0)
		{
			break;
		}
	}
	return cno;	
}

u8 KeyScan(void)
{
	u8 keyv,rno,cno;
	//wait switch press
	while(ColScan());
	//take the rno
	rno =RowCheck();
	//take the cno
	cno=ColCheck();
	//collect key value from KpmLUT
	keyv=kpno[rno][cno];
	//wait for switch relese
	while(!ColScan());
	return keyv;
}

u32 ReadNum(void)
{
	u8  key;
	u32 sum=0;
	while(1)
	{
	key=KeyScan();
	//CharLcd(key);
	if((key>='0')&&(key<='9'))
	{
		sum=(sum*10)+(key-48);
		CharLcd(key);
	}
	else if(key=='=')
		break;
	}
	return sum;
}
