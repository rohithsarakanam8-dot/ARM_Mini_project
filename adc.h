#include"types.h"
//ADC FUNCTIONS
void InitAdc(void);
void Read_ADC( u32 chno,f32 *eAR, u32 *AdcDval);
void Read_temp(s32 *temp,f32 *eAR);
void Display_temp(s32 *temp);
void EditTempSettings(void);

