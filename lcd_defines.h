//pins
#define DATA_PINS 0   // P0.0 - P0.7
#define RS  16
#define RW  17
#define EN  18
#define Red_Led 8
#define Green_Led 9
// Admin mode hardware: unused GPIO pins on IO0, chosen to avoid
// clashing with the LEDs (8,9) and LCD control lines (RS/RW/EN = 16,17,18)
#define ADMIN_SW 10   // input: switch that requests entry into admin mode
#define BUZZER 11     // output: audible feedback (beeps)
#define ADMIN_SW 10
#define BUZZER 11

//cmd
#define CLR_LCD        0x01
#define LINE1_POS0       0X80
#define LINE2_POS0       0xc0
//#define LINE2_POS15      0xce
#define M_4BIT_1LINE    0x20 //001 DL L F 00(001 0 0 0 00) 
#define M_4BIT_2LINE    0x28 //001 0 1 0 00
#define M_8BIT_1LINE    0x30 //001 1 0 0 00
#define M_8BIT_2LINE    0x38 //001 1 1 0 00
#define DIS_OFF          0x08 //0000 1DCB
#define DIS_ON_CUR_OFF   0x0c //0000 1100
#define DIS_ON_CUR_ON    0x0E //0000 1110
#define DIS_ON_CUR_BLINK 0x0F //0000 1111
#define RET_CUR_HOME     0x02
#define MOV_DIS_SFT_L    0x18
#define MOV_DIS_SFT_R    0x1c
#define MOV_CUR_L        0x10
#define MOV_CUR_R        0x14
#define SFT_CUR_L        0x04
#define SFT_CUR_R        0x06

//TIMERS
#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)

//ADC
#define ADCLK 3000000
#define CLK_DIV ((PCLK/ADCLK)-1)

#define CH0 0x01
#define CH1 0x02
#define CH2 0x04
#define CH3 0x08

#define PDN_BIT (1<<21)
#define CLKDIV (CLK_DIV<<8)
#define START_CONV (1<<24)
       
#define RESULT 6
#define DONE_BIT 31

#define AIN0 0x00400000
#define AIN1 0x01000000
#define AIN2 0x04000000
#define AIN3 0x10000000

//kpm
//KPM_defines.h

#define ROW0 16 //P1.16
#define ROW1 17
#define ROW2 18
#define ROW3 19

#define COL0 20 //P1.20
#define COL1 21
#define COL2 22
#define COL3 23
