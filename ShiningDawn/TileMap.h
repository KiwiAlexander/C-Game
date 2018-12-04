#pragma once
#include "TileList.h"
#include "Tile.h"

#define WIDTH 10
#define HEIGHT 10
#define PIXELSIZE 16
#define COLLUMS 79
#define ROWS 44

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;

ref class TileMap
{
private:
	TileList^ tileList;
	array<int, 2>^ map;
	Graphics^ canvas;
	Bitmap^ currentTileBitmap;

	int currentTileIndex;

	int tileWidth;
	int tileHeight;

	int drawLocX;
	int drawLocY;

public:
	TileMap(TileList^ startTileList, Graphics^ startCanvas);
	void SetMapEntry(int col, int row, int tileIndex);
	//int GetMapEntry(int col, int row);

	void DrawMap();
	void DrawMap(array<int, 2>^ newMap);

	void LoadMapFromFile(String^ mapFileName);

	bool TargetWalkable(int xPos, int yPos);
};

