#include "pch.h"

cartStruct* cartPointer;
Trampoline* CartExecuter_Main_t;

int invulnerableCart = 0;

int isCartHitObj(EntityData1* data) {

	CollisionInfo* col = data->Collision;

	if (invulnerableCart > 0)
		return 0;

	if ((col->Flag & 1) != 0)
	{
		int colCount = 0;
		if (col->HitCount > 0)
		{
			do
			{
				EntityData1* colHit = data->Collision->CollisionHits[colCount].hit_entity;
				if (sub_621380(colCount, colHit) && !invulnerableCart)
				{
					return 1;
				}
				++colCount;
			} while (colCount < data->Collision->HitCount);
		}
	}

	return 0;
}

void Cart_HurtPlayer(ObjectMaster* obj) {

	if (!cartPointer || TimerStopped != 0 || invulnerableCart > 0 || CurrentLevel != LevelIDs_Route101280)
		return;

	if (isCartHitObj(obj->Data1.Entity)) {

		invulnerableCart = 300;
		bool v4 = cartPointer[1].Forward.x == 0;
		bool v5 = cartPointer[1].Forward.x < 0;
		cartPointer->DistanceIdk = 4;
		cartPointer[2].Forward.z = v5 || v4 ? -1820 : 1820;
		PlaySoundProbably(15, 0, 0, 0);

		if (*(int*)0x1D970FC)
		{
			if (*(int*)0x1D970FC == 1)
			{
				PlayVoice(2, 1321);
			}
		}
		else
		{
			PlayVoice(2, 1693);
		}
	}

	return;
}


void __cdecl CartExecuter_Main_r(ObjectMaster* obj)
{
	ObjectFunc(origin, CartExecuter_Main_t->Target());
	origin(obj);

	if (CurrentLevel == LevelIDs_Route101280) {
		if (invulnerableCart > 0) {
			invulnerableCart--;
		}

		cartPointer = (cartStruct*)obj->EntityData2;
	}
}

void init_CartHack() {

	CartExecuter_Main_t = new Trampoline((int)CartExecuter_Main, (int)CartExecuter_Main + 0x6, CartExecuter_Main_r);
}