// ----------------------------------------------------------------------------------------------------- //
//	PROJECT: SHININGDAWN IN628 2016 ASSIGNMENT ONE: 2D ROGUELIKE										 //
//	AUTHOR: ALEXANDER S R PHILLIPS																		 //
//	DATE: 9/10/2016																						 //
//	SUMMARY: ROGUELIKE VIDEO GAME WITH PROCEDURALLY GENERATED DUNGEONS, BATTLE, FOG AND MORE			 //
// ----------------------------------------------------------------------------------------------------- //
#include "Sprite.h"
#include "Dungeon.h"
#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class SpriteList
{
private:
	Sprite^ head;
	Sprite^ tail;
public:
	SpriteList(void);
	void addSprite(Sprite^ newSprite);
	void deleteOneSprite(Sprite^ SpriteToDelete);
	void deleteAllDeadSprites();
	void moveSprites();
	void drawSprites();
	void drawSpritesFog();
	void wanderSprites();
	void updateSprites();
	void updateLocationSprites();
	void terrainCollisionSprites();
	int countSprites();
	Sprite^ checkCollectionSprites(Sprite^ otherSprite);
};

