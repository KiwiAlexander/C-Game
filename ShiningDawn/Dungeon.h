// ----------------------------------------------------------------------------------------------------- //
//	PROJECT: SHININGDAWN IN628 2016 ASSIGNMENT ONE: 2D ROGUELIKE										 //
//	AUTHOR: ALEXANDER S R PHILLIPS																		 //
//	DATE: 9/10/2016																						 //
//	SUMMARY: ROGUELIKE VIDEO GAME WITH PROCEDURALLY GENERATED DUNGEONS, BATTLE, FOG AND MORE			 //
// ----------------------------------------------------------------------------------------------------- //
#pragma once

#include "Cell.h"
#include "Room.h"
#include "Sprite.h"

public enum ECellType {BLANK, DIRT, WALL, FOG, ANTI, PIT};

#define COLLUMNS 79
#define ROWS 44
#define TILELENGTH 16
#define FOGLENGTH 10
#define FOGHEIGHT 4
#define FOGWIDTH 4
#define CELLLENGTH 1
#define ROOMMIN 6
#define ROOMMAX 10
#define ROOMSTARTMIN 1
#define ROOMSTARTXMAX 65
#define ROOMSTARTYMAX 35

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class Dungeon
{
public:

	Dungeon(int nRooms, Random^ startRandom, Sprite^ mainCharaterSprite);

	void GenerateDungeon();
	void MakeRoom(int nRoom);
	void MakeCorridor(Point^ a, Point^ b, int number);
	void GenerateDungeonFog();
	void fellInPit();
	void DrawDungeon();

	array<int, 2>^ ConvertCellToInt(array<Cell^, 2>^);

	Rectangle^ getFogRectangle();
	array<Cell^, 2>^ getCellArray();
	array<Cell^, 2>^ getFogArray();

private:

	int nRooms;
	Random^ rGen;
	Sprite^ mainCharater;

	Rectangle^ pitlocation;
	Rectangle^ pitCollision;
	Rectangle^ fog;

	array<Cell^, 2>^ cellArray;
	array<Cell^, 2>^ fogArray;

	array<Room^>^ roomArray;
	array<Rectangle>^ corridorArrayX;
	array<Rectangle>^ corridorArrayY;
	array<Rectangle>^ corridorArrayMID;
};

