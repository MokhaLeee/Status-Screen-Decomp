#ifndef GBAFE_STATSCREEN_H
#define GBAFE_STATSCREEN_H

// StatScreen.h
// FE8U only!

struct StatScreenData{
	/* 0x0 	*/	u8 CurPage;		// Maybe Related to ChapterState
	/* 0x0 	*/	u8 TotalPage;
	/* 0x2	*/	u16 _unk2;
	/* 0x4	*/	u16 _unk4;
	/* 0x6	*/	u16 VranOffset;
	/* 0x8	*/	u8 _unk8;
	/* 0x8	*/	u8 _unk9;
	/* 0x8	*/	u8 _unkA;
	/* 0x8	*/	u8 _unkB;
	/* 0xC	*/	Unit* unit;
	/* 0x10	*/	struct MUProc* procMU;
	/* 0x14	*/	void* CurrentHelpTextStruct;
	/* 0x18	*/	TextHandle TextL[3];
	/* 0x30	*/	TextHandle Text[12];
	/* 0x90	*/	TextHandle TextItem[5];			// start: 0x2003C8C
	/* 0xB8	*/	TextHandle TextPage2[5];	// start: 0x2003CB4
};

typedef struct ProcStatScreen ProcStatScreen;
struct ProcStatScreen{
	PROC_FIELDS;	
};


struct MSS_TextStruct{
	/* 0x0	*/	TextHandle* Text;
	/* 0x4	*/	u16* Buffer;
	/* 0x8	*/ 	u8 color;
	/* 0x9	*/ 	u8 xStart;
	/* 0xA	*/ 	u16 unkA;	// Maybe unused for align
	/* 0xC	*/ 	u32* pStringIndexAt; 
	/* 0x10	*/
};

typedef void (*MSS_Func)(void);
extern MSS_Func gStatScreenDrawCallTable[];	// 0x8205B24
extern MSS_Func new_gStatScreenDrawCallTable[];


extern struct StatScreenData gStatScreenBuffer;	// 0x2003BFC in FE8U
extern const ProcCode _gProc_StatScreen[];	// 0x8A009D8
extern u16 MSS_Buffer[][0x20]; // 0x2003C94
//#define gpStatScreenPageBg0Map &MSS_Buffer[0x2][0xC]
//#define gpStatScreenPageBg2Map &MSS_Buffer[0x2A][0xC]
extern u16 gpStatScreenPageBg0Map[][0x20];
extern u16 gpStatScreenPageBg2Map[][0x20];


//Functions here
void MSS_DrawLeftText(void);
void MSS_DrawPage1_UnitPage(void);

// Idle
extern void MSS_PageTransition(u8,u8,ProcStatScreen*); // 0x8087AD8+1
extern Unit* MSS_GetNextUnit(Unit* unitCur, u32 Count); // 0x8087920+1
extern void MSS_DisplayNextUnit(Unit*, u8, ProcStatScreen*); // 0x8087E7C+1
extern void StartStatScreenHelpTextBubble(u8 CurPage, ProcStatScreen*); // 0x80889A0+1


// DrawStatScreen
extern const u16 gPortMSSPal1[]; // 0x8A01F04
extern const u16 gPortMSSPal2[]; // 0x8A01EE4
void MSS_MaybeSetTextBatch(void); // 0x8086DF0+1
void MSS_MaybeDrawPortrait(Proc*, u16* dest, u8 PortraitID, u16, u8); // 0x8005E98+1
void MSS_DrawLeftText(void); // 0x8086E44+1
void DrawStatScreenPage(u8 CurPage); // 0x80878CC+1


// Draw Unit Page
extern struct MSS_TextStruct MSS1_StatusTextData[]; // 0x8205964
extern struct MSS_TextStruct MSS_TextPage1[];
void DrawUnitStatScreen(void); // 0x8087184+1
void MSS_DrawStatusTextPage1(struct MSS_TextStruct*); // 0x8086E00+1
// void DrawTextInline(struct TextHandle*, u16* bg, int color, int xStart, int tileWidth, const char* cstring);
void DrawStatScreenBar(u16 BarId, u8 x, u8 y, s8 ValueBase, s8 ValueReal, s8 MaxValue); // 0x80870BC


// Draw Item Page
extern u16 MSS_ItemPageTSA[]; // 0x8A02204
extern u16 MSS_gBgBufferItemPage[]; // 0x20049EE
extern u16 MSS_MaybeSomeUiTSA[]; // 0x8A02250
extern u16 MSS_gBgBufferSomeUiPage[][0x20]; // 0x200472E


// Draw Rank Page
void DrawUnitWeaponRank(u8,u8,u8,u8); // (Index,x,y,RankId) 0x8087789
void DrawUnitScreenSupportList(void); // 0x8087699


static inline void MSS_DrawTextInline(u8 Index, u8 x, u8 y, u16 StringID){ 
	DrawTextInline(&gStatScreenBuffer.Text[Index],&MSS_Buffer[y][x],3,0,0,GetStringFromIndex(StringID)); 
	}
static inline void MSS_DrawTextInlinePage2(u8 Index, u8 x, u8 y, u16 StringID){ 
	DrawTextInline(&gStatScreenBuffer.TextPage2[Index],&MSS_Buffer[y][x],3,6,0,GetStringFromIndex(StringID)); 
	}
static inline void MSS_DrawTextInsert(u8 Index, u8 x, char* String){
	Text_InsertString(&gStatScreenBuffer.Text[Index],x,0x2,String);	//(Text, xPos, color, String);
}
#endif // GBAFE_STATSCREEN_H
