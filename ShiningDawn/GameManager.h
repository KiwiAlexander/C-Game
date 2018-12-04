// ----------------------------------------------------------------------------------------------------- //
//	PROJECT: SHININGDAWN IN628 2016 ASSIGNMENT ONE: 2D ROGUELIKE										 //
//	AUTHOR: ALEXANDER S R PHILLIPS																		 //
//	DATE: 9/10/2016																						 //
//	SUMMARY: ROGUELIKE VIDEO GAME WITH PROCEDURALLY GENERATED DUNGEONS, BATTLE, FOG AND MORE			 //
// ----------------------------------------------------------------------------------------------------- //
#pragma once

#include "Tile.h"
#include "TileList.h"
#include "TileMap.h"

#include "Sprite.h"
#include "SpriteList.h"

#include "Dungeon.h"
#include "Room.h"
#include "Cell.h"

#define NUMROOMS 6
#define DODGECHANCE 5
#define MAXCOINS 100
#define HEALTHINCRASE 25
#define STATINCRASE 1
#define MAXTILES 9
#define NUMTILES 7
#define TILEENTRY 6
#define FRAMESPERSHEET 4
#define DIRECTIONS 4
#define NUMENEMIES 3

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class GameManager
{
private:

	Graphics^ graphics;
	Random^ rGen;
	Dungeon^ mainDungeon;
	bool genSprite = false;
	bool genLocation = false;
	bool createSprites = false;
	bool isWalking;

	array<Tile^>^ otherTiles;
	array<Bitmap^>^ otherTileBitmaps;

	TileList^ tilelistMain;
	TileMap^ tileMapMain;

	Bitmap^ spriteSheets;
	Sprite^ mainCharaterSprite;
	SpriteList^ mageSpriteList;
	SpriteList^ rogueSpriteList;
	SpriteList^ itemSpriteList;

public:

	GameManager(Graphics^ newGraphics, Random^ newRandom);

	void generateTileMapMain();
	void gameRun();
	Sprite^ generateMainCharacter();
	SpriteList^ generateMageSprite();
	SpriteList^ generateRogueSprite();
	SpriteList^ generateItems();
	Sprite^ getMainCharaterSprite();
	void setIsWalking(bool newIsWalking);
	bool getIsWalking();
	void setgenLocation(bool newGenLocation);
	void generateCharacters();
	Rectangle^ fogRec();
};

