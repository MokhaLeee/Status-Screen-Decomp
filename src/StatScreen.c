#include "include/gbafe.h"
#include "StatScreen.h"

#include "StatScreenDrawPages.c"

// From (proc)859AB54 - (PlayerPhaseMainIdle)801C9CC 
// ORG 0x808894C
void StartStatScreen(const Unit* unit, Proc* proc) 
{
	gStatScreenBuffer._unk4 = 0;
	gStatScreenBuffer.VranOffset = 0;
	
	gStatScreenBuffer.CurPage = (gChapterData.chapterStateBits&3);
	gStatScreenBuffer.unit = (Unit*)unit;
	gStatScreenBuffer.CurrentHelpTextStruct = 0;
	gStatScreenBuffer._unk2 = 0;
	gStatScreenBuffer._unk8 = 0;
	
	//if( gChapterData.unk41_1 | gChapterData.muteSfxOption )
		m4aSongNumStart(0x6A);
	
	ProcStartBlocking(_gProc_StatScreen,proc);
	return;
}

/*

1. 只有在Press B的情况下才会直接Break Loop：意味着Idle后续紧跟着的6C指令即为结束指令；
2. R-Text对应于Label 0；
3. 上下切换角色、左右切换界面都是通过内插Bolcking-Proc完成的，并未Break Loop。其中：
	切换角色对应函数 MSS_DisplayNextUnit(Unit*, u8, Proc*);
	切换界面对应函数 MSS_PageTransition(u8,u8,Proc*);

*/
// ORG 0x808873C
void gFuncMSS_Idle(ProcStatScreen* proc){
	
	u16 iKeyCur = gpKeyState->pressedKeys;
	Unit* NextUnit = NULL;
	
	if( 0b10 & iKeyCur ) // Press B
	{
		gLCDIOBuffer.dispControl.enableBg0 = 1;
		gLCDIOBuffer.dispControl.enableBg1 = 0;
		gLCDIOBuffer.dispControl.enableBg2 = 1;
		gLCDIOBuffer.dispControl.enableBg3 = 1;
		gLCDIOBuffer.dispControl.enableObj = 1;
		SetColorEffectsParameters(3,0,0,0x10);
		SetColorEffectsFirstTarget(0,0,0,0,0);
		SetColorEffectBackdropFirstTarget(1);
		
		gPaletteBuffer[0] = 0;
		EnablePaletteSync();
		BreakProcLoop( (Proc*)proc );
		//if( gChapterData.unk41_1 | gChapterData.muteSfxOption )
			m4aSongNumStart(0x6B);
		return;
	}
	
	else if( 0x20 & iKeyCur ) // Press Left
	{
		gStatScreenBuffer.CurPage = __modsi3(-1+gStatScreenBuffer.CurPage + gStatScreenBuffer.TotalPage, gStatScreenBuffer.TotalPage);
		MSS_PageTransition(0x20,gStatScreenBuffer.CurPage,proc);
		return;
	}
	
	else if( 0x10 & iKeyCur) // Press Right
	{
		gStatScreenBuffer.CurPage = __modsi3(1+ gStatScreenBuffer.CurPage + gStatScreenBuffer.TotalPage, gStatScreenBuffer.TotalPage);
		MSS_PageTransition(0x10,gStatScreenBuffer.CurPage,proc);
		return;
	}
	
	
	else if( 0x40 & iKeyCur) // Press Up
	{
		NextUnit = MSS_GetNextUnit(gStatScreenBuffer.unit,-1);
		MSS_DisplayNextUnit(NextUnit, -1, proc);
		return;
	}
	
	else if( 0x80 & iKeyCur) // Press Down
	{
		NextUnit = MSS_GetNextUnit(gStatScreenBuffer.unit,1);
		MSS_DisplayNextUnit(NextUnit, 1, proc);
		return;
	}
	
	else if( 0x1  & iKeyCur) // Press A
	{
		if(  gStatScreenBuffer.unit->rescueOtherUnit )
		{
			NextUnit = GetUnit(gStatScreenBuffer.unit->rescueOtherUnit);
			if( 0x10 & gStatScreenBuffer.unit->state )
				MSS_DisplayNextUnit(NextUnit, 0, proc);
			else
				MSS_DisplayNextUnit(NextUnit,-1,proc);
			return;
		}
	}
		
	else if( 0x100& iKeyCur) // Press R
	{
		ProcGoto( (Proc*)proc,0);
		StartStatScreenHelpTextBubble(gStatScreenBuffer.CurPage ,proc);
		return;
	}
	
	return;
}


