#include "include/gbafe.h"
#include "StatScreen.h"

// ORG 0x8088670
void DrawStatScreen(Proc* proc){
	const Unit* unit = (const Unit*)gStatScreenBuffer.unit;
	gStatScreenBuffer.TotalPage = 3;	// Total Page!!!
	
	Text_InitFont();
	_ResetIconGraphics();
	
	MSS_MaybeSetTextBatch();
	
	// Portrait
	u8 iPortratID = GetUnitPortraitId(unit);
	if( 1 & (unit->state>>0x17) )		// (Unit+0xE) & 0x80  -> Increase portrait index by 1 (???)
		iPortratID++;
	MSS_MaybeDrawPortrait(proc, &gBg2MapBuffer[0x21], iPortratID, 0x4E0, 0xB);
	if( 0 == GetPortraitData(iPortratID)->pPortraitGraphics )
		CopyToPaletteBuffer(gPortMSSPal1,0x40,0x20);
	else
		CopyToPaletteBuffer(gPortMSSPal2,0x40,0x20);

	// Sprit
	MU_EndAll();
	gStatScreenBuffer.procMU = MU_CreateForUI((Unit*)unit,80,138);
	
	// Left Page
	MSS_DrawLeftText();
	
	// Right Page
	u32 src = 0;
	CpuFastSet(&src,&gpStatScreenPageBg0Map[0][0],0x1000140);
	CpuFastSet(&src,&gpStatScreenPageBg2Map[0][0],0x1000140);
	new_gStatScreenDrawCallTable[gStatScreenBuffer.CurPage]();
	
	// Apply TSA
	BgMapCopyRect(&gpStatScreenPageBg0Map[0][0],&gBg0MapBuffer[0x4C],0x12,0x12);
	BgMapCopyRect(&gpStatScreenPageBg2Map[0][0],&gBg2MapBuffer[0x4C],0x12,0x12);
	EnableBgSyncByMask(0b111);
	return;
	
}




















// ORG 0x8086E44
void MSS_DrawLeftText(void){
	TextHandle* TextCur = NULL;
	u16* BgMap = NULL;
	int xStart = 0;
	char* cString = NULL;
	Unit* unit = gStatScreenBuffer.unit;
	
	// On init
	FillBgMap(gBg0MapBuffer,0);
	BattleGenerateUiStats(unit, GetUnitEquippedWeaponSlot(unit));
	
	// Draw unit name
	TextCur	= &gStatScreenBuffer.TextL[0x0];
	BgMap 	= &gBg0MapBuffer[0x143];
	cString	= GetStringFromIndex(unit->pCharacterData->nameTextId);
	xStart 	= Text_GetStringTextCenteredPos(0x30,cString);
	DrawTextInline(TextCur,BgMap,0,xStart,0,cString);
	
	// Draw unit class name
	TextCur	= &gStatScreenBuffer.TextL[0x1];
	BgMap 	= &gBg0MapBuffer[0x1A1];
	cString	= GetStringFromIndex(unit->pClassData->nameTextId);
	xStart 	= 0;
	DrawTextInline(TextCur,BgMap,0,xStart,0,cString);
	
	// "LV"+"E"
	DrawSpecialUiChar(&gBg0MapBuffer[0x1E1],3,0x24);					// "L"
	DrawSpecialUiChar(&gBg0MapBuffer[0x1E2],3,0x25);					// "V"
	DrawSpecialUiChar(&gBg0MapBuffer[0x1E5],3,0x1D);					// "E"
	
	// "HP"+"/"
	DrawSpecialUiChar(&gBg0MapBuffer[0x221],3,0x22);					// "H"
	DrawSpecialUiChar(&gBg0MapBuffer[0x222],3,0x23);					// "P"
	DrawSpecialUiChar(&gBg0MapBuffer[0x225],3,0x16);					// "/"
	
	// level + exp
	DrawUiNumberOrDoubleDashes(&gBg0MapBuffer[0x1E4],2,unit->level);	// Unit.level
	DrawUiNumberOrDoubleDashes(&gBg0MapBuffer[0x1E7],2,unit->exp);		// Unit.exp
	
	// Hp Cur+Max
	s8 sHpCur = GetUnitCurrentHp(unit);
	if( sHpCur<99 )
		DrawUiNumberOrDoubleDashes(&gBg0MapBuffer[0x224],2,sHpCur);
	else
	{
		DrawSpecialUiChar(&gBg0MapBuffer[0x223],2,0x14);
		DrawSpecialUiChar(&gBg0MapBuffer[0x224],2,0x14);
	}
	
	s8 sHpMax = GetUnitMaxHp(unit);
	if( sHpMax<99 )
		DrawUiNumberOrDoubleDashes(&gBg0MapBuffer[0x227],2,sHpMax);
	else
	{
		DrawSpecialUiChar(&gBg0MapBuffer[0x226],2,0x14);
		DrawSpecialUiChar(&gBg0MapBuffer[0x227],2,0x14);
	}

	return;
}




















// ORG 0x8087184
void MSS_DrawPage1_UnitPage(void){
	Unit* unit = gStatScreenBuffer.unit;
	
	// -------------------------- Draw Text --------------------------
	// Judge for draw text Pow or Mag
	u32 uTedxtId = 0x4FE;
	if( UnitHasMagicRank(unit) )
		uTedxtId = 0x4FF;
	MSS_DrawTextInline(0x0,0xD,0x3,uTedxtId);

	// Draw text: Skl & Spd & lck ....
	MSS_DrawTextInline(0x1,0xD,0x5,0x4EC);
	MSS_DrawTextInline(0x2,0xD,0x7,0x4ED);
	MSS_DrawTextInline(0x3,0xD,0x9,0x4EE);
	MSS_DrawTextInline(0x4,0xD,0xB,0x4EF);
	MSS_DrawTextInline(0x5,0xD,0xD,0x4F0);
	MSS_DrawTextInline(0x6,0x15,0x3,0x4F6);
	MSS_DrawTextInline(0x7,0x15,0x5,0x4F7);
	MSS_DrawTextInline(0x8,0x15,0x7,0x4F8);
	MSS_DrawTextInline(0x9,0x15,0x9,0x4F9);
	MSS_DrawTextInline(0xA,0x15,0xB,0x4F1);
	MSS_DrawTextInline(0xB,0x15,0xD,0x4FA);

	// -------------------------- Draw Bars --------------------------
	s8 base=0, real=0, max=0;
	
	// Pow
	base = unit->pow;
	real = GetUnitPower(unit);
	max  = unit->pClassData->maxPow;
	DrawStatScreenBar(0,5,1,base,real,max);
	
	// Skl
	base = unit->skl;
	real = GetUnitSkill(unit);
	max  = unit->pClassData->maxSkl;
	if( 0x10 & unit->state )
		DrawStatScreenBar(1,5,3,base/2,real/2,max);
	else
		DrawStatScreenBar(1,5,3,base,real,max);
	
	// Spd
	base = unit->spd;
	real = GetUnitSpeed(unit);
	max  = unit->pClassData->maxSpd;
	if( 0x10 & unit->state )
		DrawStatScreenBar(2,5,5,base/2,real/2,max);
	else
		DrawStatScreenBar(2,5,5,base,real,max);
	
	// Lck
	base = unit->lck;
	real = GetUnitLuck(unit);
	max  = 0x1E;
	DrawStatScreenBar(3,5,7,base,real,max);
	
	// Def
	base = unit->def;
	real = GetUnitDefense(unit);
	max  = unit->pClassData->maxDef;
	DrawStatScreenBar(4,5,9,base,real,max);
	
	// Res
	base = unit->res;
	real = GetUnitResistance(unit);
	max  = unit->pClassData->maxRes;
	DrawStatScreenBar(5,5,0xB,base,real,max);
	
	// Mov
	base = unit->pClassData->baseMov;
	real = base + unit->movBonus;
	max = 0xF;
	DrawStatScreenBar(6,0xD,1,base,real,max);
	
	// Con
	base = unit->pClassData->baseCon + unit->pCharacterData->baseCon;
	real = base + unit->conBonus;
	max = unit->pClassData->maxCon;
	DrawStatScreenBar(7,0xD,3,base,real,max);
	
	
	// -------------------------- Draw Others --------------------------
	// Aid
	DrawUiNumberOrDoubleDashes(&MSS_Buffer[0x7][0x19],2,GetUnitAid(unit));
	u8 uAidIconId = GetUnitAidIconId(unit->pCharacterData->attributes | unit->pClassData->attributes);
	DrawIcon(&MSS_Buffer[0x7][0x1A],uAidIconId,0x5000);
	
	// Rescue name
	char* RescueName = GetUnitRescueName(unit);
	MSS_DrawTextInsert(9,0x18,RescueName);
	
	// Status
	Unit UnitTmp = *unit;
	if( 0xA == unit->statusIndex )
	{
		UnitTmp.statusIndex = 0;
		if( 0 == UnitTmp.statusDuration )
			MSS_DrawTextInsert(11,0x18,GetUnitStatusName(&UnitTmp));
		else
			MSS_DrawTextInsert(11,0x16,GetUnitStatusName(&UnitTmp));
	}
	else
	{
		if( 0x0 != unit->statusIndex )
		{
			MSS_DrawTextInsert(11,0x16,GetUnitStatusName(unit));
			DrawUiSmallNumber(&MSS_Buffer[0xD][0x1C],0x0,unit->statusDuration);
		}
		else
			MSS_DrawTextInsert(11,0x18,GetUnitStatusName(unit));
	}
	DrawIcon(&MSS_Buffer[0xB][0x18], GetUnitAffinityIconId(unit), 0x5000 );
	
	// Useless BWL CALL: bl 0x8086FAC
	return;
}

















static void MSS_DrawPage2_SubDrawItem(Unit*);
#define MSS_Page2_DrawItem ( (void(*)(TextHandle*,u16,u8,u16*))(0x8016A2D) )
// ORG 0x8087484
void MSS_DrawPage2_ItemPage(void){
	Unit* unit = gStatScreenBuffer.unit;
	s8 num = 0;
	
	// Apply TSA
	Decompress(MSS_ItemPageTSA,gGenericBuffer);
	BgMap_ApplyTsa(MSS_gBgBufferItemPage,gGenericBuffer,0x3040);
	
	// Draw text
	MSS_DrawTextInlinePage2(0x0,0x15,0x0D,0x500);	// "Rng"
	MSS_DrawTextInlinePage2(0x1,0x0E,0x0F,0x4F3);	// "Atk"
	MSS_DrawTextInlinePage2(0x2,0x0E,0x11,0x4F4);	// "Hit"
	MSS_DrawTextInlinePage2(0x3,0x15,0x0F,0x501);	// "Crit"
	MSS_DrawTextInlinePage2(0x4,0x15,0x11,0x4F5);	// "Avoi"

	const u8 cClassId = unit->pClassData->number;
	
	#define ClassEgg2 0x62
	#define ClassEgg1 0x34
	
	if( (ClassEgg2!=cClassId) & (ClassEgg1!=cClassId) & (0!=unit->items[0]) )
		// Draw Item
		MSS_DrawPage2_SubDrawItem(unit);
	
	s8 sItemSlot = GetUnitEquippedWeaponSlot(unit);
	
	if( (ClassEgg2!=cClassId) & (ClassEgg1!=cClassId) )
	{
		if( sItemSlot >= 0 )
		{
			// Draw a Bar for weapon-equip
			DrawSpecialUiChar(&MSS_Buffer[2+1+(sItemSlot<<1)][0x1C],0,0x35);
			BgMap_ApplyTsa(&MSS_gBgBufferSomeUiPage[2+(sItemSlot<<1)][0],MSS_MaybeSomeUiTSA,0x3040);
			
			// Draw number
			num = gBattleActor.battleAttack;
			DrawUiNumberOrDoubleDashes(&MSS_Buffer[0xF][0x14],2,num);	// Atk
			
			num = gBattleActor.battleHitRate;
			DrawUiNumberOrDoubleDashes(&MSS_Buffer[0x11][0x14],2,num);	// Hit
			
			num = gBattleActor.battleCritRate;
			DrawUiNumberOrDoubleDashes(&MSS_Buffer[0xF][0x1B],2,num);	// Atk
			
			num = gBattleActor.battleAvoidRate;
			DrawUiNumberOrDoubleDashes(&MSS_Buffer[0x11][0x1B],2,num);	// Avoid
		}
	}
	else
	{
		// Draw numbers for Egg
		num = 0xFF;
		DrawUiNumberOrDoubleDashes(&MSS_Buffer[0xF][0x14],2,num);	// Atk
		DrawUiNumberOrDoubleDashes(&MSS_Buffer[0x11][0x14],2,num);	// Hit
		DrawUiNumberOrDoubleDashes(&MSS_Buffer[0xF][0x1B],2,num);	// Atk
		num = gBattleActor.battleAvoidRate;
		DrawUiNumberOrDoubleDashes(&MSS_Buffer[0x11][0x1B],2,num);	// Avoid
	}
	
	// Draw weapon range
	u16 item = (sItemSlot<0)?0:unit->items[sItemSlot];
	const char* cString = GetItemDisplayRangeString(item);
	s8 xStart = 0x37- Text_GetStringTextWidth(cString);
	Text_InsertString(&gStatScreenBuffer.TextPage2[0],xStart,0x2,cString);
	
	// Draw a word "Equipmer"
	for( u8 iCount=0; iCount<7; iCount++ ){
		gpStatScreenPageBg0Map[0xB][1+iCount] = 0x7060+iCount;
		gpStatScreenPageBg0Map[0xC][1+iCount] = 0x7068+iCount;
	}
	return;
}



static void MSS_DrawPage2_SubDrawItem(Unit* unit){
	u8 TmpColor = 0;
	u8 NumCur = 0;
	u8 NumMax = 0;
	u8 color = 0;
	u16 item = 0;
	TextHandle* Text = NULL;
	
	for(u8 iCount=0; iCount<5; iCount++)
	{
		item = unit->items[iCount];
		if( 0 == item )
			return;
		
		if( (0x1000&unit->state) & ((GetUnitItemCount(unit)-1)==iCount) ) 	// Drop last item
			color = 4;
		else if( IsItemDisplayUsable(unit,item) )
			color = 1;
		else
			color = 0;
		
		// ORG 0x8016A2C : DrawItemStatScreenLine(&gStatScreenBuffer.TextItem[iCount],item,color,&MSS_Buffer[0x3+2*iCount][0xD]);
		Text = &gStatScreenBuffer.TextItem[iCount];
		Text_Clear(Text);

		// Draw "/"
		TmpColor = (1==color)?1:0;
		DrawSpecialUiChar(&MSS_Buffer[0x3+2*iCount][0x19],TmpColor,0x16);
		
		// Draw item Cur use & Max use
		TmpColor = (1==color)? 2:1;
		if( ItemTable[item&0xFF].attributes & 0x8 )
			NumCur = 0xFF, NumMax = 0xFF;
		else
			NumCur = item>>0x8, NumMax = ItemTable[item&0xFF].maxUses;
		DrawUiNumberOrDoubleDashes(&MSS_Buffer[0x3+2*iCount][0x18],TmpColor,NumCur);	// Cur num
		DrawUiNumberOrDoubleDashes(&MSS_Buffer[0x3+2*iCount][0x1B],TmpColor,NumMax);	// Max num
		
		// Draw item name
		TmpColor = (1==color)?0:1;
		Text_SetColorId(Text,TmpColor);
		Text_DrawString(Text,String_ExpandTactName(GetStringFromIndex(ItemTable[item&0xFF].nameTextId)) );
		Text_Display(Text,&MSS_Buffer[0x3+2*iCount][0xF]);
		
		// Draw icon
		if(0==item)
			DrawIcon(&MSS_Buffer[0x3+2*iCount][0xD],-1,0x4000);
		else
			DrawIcon(&MSS_Buffer[0x3+2*iCount][0xD],ItemTable[item&0xFF].iconId,0x4000);
	}
	return;
}





















void MSS_DrawPage3_RankPage(void){
	Unit* unit = gStatScreenBuffer.unit;
	
	if( UnitHasMagicRank(unit) )
	{
		DrawUnitWeaponRank(0,1,1,5);
		DrawUnitWeaponRank(1,1,3,6);
		DrawUnitWeaponRank(2,9,1,7);
		DrawUnitWeaponRank(3,9,3,4);
	}
	else
	{
		DrawUnitWeaponRank(0,1,1,0);
		DrawUnitWeaponRank(1,1,3,1);
	/* 	DrawUnitWeaponRank(2,9,1,2);
		DrawUnitWeaponRank(3,9,3,3); */
		DrawUnitWeaponRank(2,1,5,2);
		DrawUnitWeaponRank(3,1,7,3);
	}
	DrawUnitScreenSupportList();
	return;
}
