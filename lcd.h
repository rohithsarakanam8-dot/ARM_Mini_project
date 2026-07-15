#include "types.h"

// LCD FUNCTIONS
void WriteLCD(u8 data);
void CmdLcd(u8 cmd);
void CharLcd(u8 ascii);
void InitLcd(void);
void StrLcd(u8 *str);
void U32Lcd(u32 n);

