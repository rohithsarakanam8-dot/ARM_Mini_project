// Sets bit BP in WORD (was previously an empty macro that did nothing -
// meant no GPIO direction/output register was ever actually being written)
#define SETBIT(WORD,BP) (WORD|=(1<<BP))
#define CLRBIT(WORD,BP) (WORD&=~(1<<BP))
#define STATUSBIT(WORD,BP) ((WORD>>BP)&1)
#define WRITEBIT(WORD,BP,BIT) (WORD=((WORD&~(1<<BP))|(BIT<<BP)))
#define WRITENIBBLE(WORD,SBP,NIBBLE) (WORD=((WORD&~(0xf<<SBP))|(NIBBLE<<SBP)))
#define WRITEBYTE(WORD,SBP,BYTE) (WORD=((WORD&~(0xff<<SBP))|(BYTE<<SBP)))
#define WRITEHWORD(WORD,SBP,HWORD) (WORD=((WORD&~(0xffff<<SBP))|(HWORD<<SBP)))
#define READNIBBLE(WORD,BP)  ((WORD>>BP)&15)

