#include"types.h"

// Total number of fixed scheduled messages stored in messageList[]
#define TOTAL_MESSAGES 10

// One scheduled message: fires when the RTC hour/minute matches,
// as long as `enabled` is set. `enabled` is toggled live from admin
// mode (see admin.c: EditMessagesMenu).
typedef struct {
    u8 hour;
    u8 minute;
    char text[80];
    u8 enabled;   // 1 = show, 0 = skip (user controlled)
} Message;

// The actual array is defined once in main1.c; declared extern here
// so other files (e.g. admin.c, prj_lcd.c) can read/modify it too.
extern Message messageList[TOTAL_MESSAGES];

void DisplayMessage(Message msg);
void Home_dis(void);
void DisplayDate(s32 d, s32 m, u32 y);
void DisplayDay(u32 dow);
void DisplayRTCTime(s32 h, s32 m, s32 s);
void DisplayTemperature(f32 temp);
f32 ReadTemperature(void);
void EnterAdminMode(void);
u8 AdminKeyPressed(void);
s32 MessageScheduled(s32 h, s32 m);


