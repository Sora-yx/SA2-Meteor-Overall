#include "pch.h"

int timerMeteor = 0;
int rngTiming = 400;
char meteorQty = 1;

Trampoline* FireBall_t;
Trampoline* DamagePlayer_t;


void setMeteorTiming() {

	int rng = rand() % 4;

	switch (rng) {
	case 0:
	default:
		rngTiming = 230;
		break;
	case 1:
		rngTiming = 500;
		break;
	case 2:
		rngTiming = 100;
		break;
	case 3:
		rngTiming = 320;
		break;
	}

	if (CurrentLevel == LevelIDs_Route101280)
	{
		if (rngTiming > 100)
			rngTiming /= 1.5;
	} 
	else if (CurrentLevel == LevelIDs_ChaoWorld)
	{
		rngTiming *= 3;
	}
	else if (CurrentCharacter > Characters_Shadow || CurrentLevel >= LevelIDs_BigFoot && CurrentLevel <= LevelIDs_EggGolemE && CurrentLevel != LevelIDs_FinalHazard)
	{
		rngTiming *= 2 -50;
	}

	if (MissionNum == 1)
	{
		rngTiming += 250;
	}
}

NJS_VECTOR SetMeteorInChaoGardens() {

	int rng = rand() % 3;
	NJS_VECTOR pos = ProbablyChaoSpawnPoints[16 * CurrentChaoArea
		- (__int64)((double)rand() * 0.000030517578125 * -15.89999961853027)
		- 16];

	if (!rng) {
		pos = MainCharObj1[0]->Position;
		pos.x += 5;
	}

	return pos;
}

void SetMeteor() {

	for (uint8_t i = 0; i < meteorQty; i++) {

		NJS_VECTOR MeteorEntryPos;
		NJS_VECTOR MeteorDest;

		char pNum = 0;

		if (MainCharObj1[1])
		{
			pNum = rand() % 2;
		}

		float posY = 100;

		if (CurrentLevel == LevelIDs_Route101280)
		{
			posY = 40;
		} else if (CurrentCharacter <= Characters_Shadow)
		{
			posY -= 40;
		}

		MeteorEntryPos.y = ((double)rand() * 0.000030517578125 - 0.5) * 100.0 + MainCharObj1[pNum]->Position.y + posY;
		MeteorEntryPos.x = ((double)rand() * 0.000030517578125 - 0.5) * 500.0 + MainCharObj1[pNum]->Position.x;
		MeteorEntryPos.z = ((double)rand() * 0.000030517578125 - 0.5) * 500.0 + MainCharObj1[pNum]->Position.z;

		MeteorDest = MeteorEntryPos;

		MeteorDest.y = MainCharObj1[pNum]->Position.y;

		if (CurrentLevel == LevelIDs_Route101280) {

			int rng2 = rand() % 2;

			if (rng2)
				rng2 = 10;

			MeteorDest.x = MainCharObj1[pNum]->Position.x + rng2;
			MeteorDest.z = MainCharObj1[pNum]->Position.z;
		}
		else if (CurrentLevel == LevelIDs_ChaoWorld)
		{
			MeteorDest = SetMeteorInChaoGardens();
		}
		else {
			int rng = rand() % 2;

			if (rng) {

				MeteorDest.x =  MainCharObj1[pNum]->Position.x + 30;
				MeteorDest.z =  MainCharObj1[pNum]->Position.z + 10;
			}
			else {
				MeteorDest.x = MeteorEntryPos.x;
				MeteorDest.z = MeteorEntryPos.z;
			}
		}

		ObjectMaster* MeteorObj = LoadObject(2, "CustomMeteor", ObjectFireBall, LoadObj_Data1);

		if (MeteorObj)
		{
			EntityData1* data = MeteorObj->Data1.Entity;

			data->Position = MeteorEntryPos;
			data->Scale = MeteorDest;
		}
		else
		{
			DeleteObject_(MeteorObj);
		}
	}
}

void Load_MeteorObject() {

	if (GameState != GameStates_Ingame || GameMode != GameMode_Level || !MainCharObj1[0] || TimerStopped != 0 
		|| CurrentChaoArea > 3 || CurrentLevel == LevelIDs_KartRace || !ControllerEnabled[0])
		return;

	if (++timerMeteor == rngTiming) {
		timerMeteor = 0;
		SetMeteor();
		setMeteorTiming();
		meteorQty = rand() % 4 + 1;
		return;
	}
}

void Load_MeteorTex() {

	timerMeteor = 0;
	rngTiming = 300;
	LoadTexPacks((TexPackInfo*)0x1155ACC, (NJS_TEXLIST***)0x1155A00);
	LoadTexPacks((TexPackInfo*)0x11650FC, (NJS_TEXLIST***)0x11650F4);

	if (!ParticleFunc_ptr)
	{
		ParticleFunc_ptr = SomethingParticleRelated;
	}

	if (!DropRingsFunc_ptr) //fix chao garden crash
	{
		DropRingsFunc_ptr = DropRings;
		if (!DropRings)
		{
			DropRingsFunc_ptr = DropRings;
		}
	}

	ResetMusic();
}

//prevent player to die in Chao Garden
int __cdecl DamagePlayer_r(EntityData1* data1, CharObj2Base* data2)
{

	auto original = reinterpret_cast<decltype(DamagePlayer_r)*>(DamagePlayer_t->Target());

	if (CurrentLevel == LevelIDs_ChaoWorld)
	{
		if (data1->Status & Status_Hurt) {
			data2->Powerups |= Powerups_Barrier;
		}
	}

	return original(data1, data2);
}

//hack the meteor obj to make it works on kart level
void FireBall_r(ObjectMaster* obj)
{
	Cart_HurtPlayer(obj);

	ObjectFunc(origin, FireBall_t->Target());
	origin(obj);
}


void init_MeteorHack() {
	WriteCall((void*)0x43CBCB, Load_MeteorTex);
	FireBall_t = new Trampoline((int)0x5C1490, (int)0x5C1496, FireBall_r);
	DamagePlayer_t = new Trampoline((int)DamagePlayer, (int)DamagePlayer + 0xA, DamagePlayer_r);
}