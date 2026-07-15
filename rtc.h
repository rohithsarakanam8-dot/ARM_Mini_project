#include "types.h"

void InitRTC(void);
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second);
void DisplayRTCTime(s32 hour, s32 minute, s32 second);
void SetRTCTimeInfo(s32 hour, s32 minute, s32 second);
void SetdateInfo(s32 date,s32 month,u32 year);
void GetdateInfo(s32 *date, s32 *month, u32 *year);
void DisplayRTCDate(s32 date, s32 month, u32 year);

//void U32Lcd(u32 n);
void SetdayInfo(u32 dow);
void GetdayInfo(u32 *dow);
void Displayday(u32 dow);
