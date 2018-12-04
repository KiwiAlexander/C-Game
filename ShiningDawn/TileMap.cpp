#include "TileMap.h"

TileMap::TileMap(TileList^ startTileList, Graphics^ startCanvas)
{
	tileList = startTileList;
	canvas = startCanvas;

	tileWidth = PIXELSIZE;
	tileHeight = PIXELSIZE;

	int drawLocX = 0;
	int drawLocY = 0;

	map = gcnew array<int, 2>(COLLUMS, ROWS);
}

void TileMap::SetMapEntry(int col, int row, int tileIndex) // assigns a value to that space in the array
{
	map[col, row] = tileIndex;
}
bool TileMap::TargetWalkable(int xPos, int yPos) // gets the tile from the array and checks if the walkable is true or false
{
	int currTileIndex = map[xPos, yPos];
	if (tileList->GetTileWalkable(currTileIndex) == false)
	{
		return false;
	}
	return true;
}

void TileMap::DrawMap() // Runs through the array and draws each tile seperated along the screen
{
	for (int c = 0; c < COLLUMS; c++)
	{
		for (int r = 0; r < ROWS; r++)
		{
			drawLocX = c * tileWidth;
			drawLocY = r * tileHeight;

			currentTileIndex = map[c, r];
			currentTileBitmap = tileList->GetTileBitmap(currentTileIndex);

			if (currentTileBitmap != nullptr)
			{
				canvas->DrawImage(currentTileBitmap, drawLocX, drawLocY);
			}
		}
	}
}
void TileMap::DrawMap(array<int, 2>^ newMap) // Same as draw map but accepts a 2d array and updates map to it
{
	map = newMap;
	for (int c = 0; c < COLLUMS; c++)
	{
		for (int r = 0; r < ROWS; r++)
		{
			drawLocX = c * tileWidth;
			drawLocY = r * tileHeight;

			currentTileIndex = map[c, r];
			currentTileBitmap = tileList->GetTileBitmap(currentTileIndex);

			if (currentTileBitmap != nullptr)
			{
				canvas->DrawImage(currentTileBitmap, drawLocX, drawLocY);
			}
		}
	}
}
void TileMap::LoadMapFromFile(String^ mapFileName) // reads a CSV file
{
	StreamReader^ sr = File::OpenText(mapFileName);
	String^ currentLine = "";
	array<String^>^ indexHolder = gcnew array<String^>(COLLUMS);

	int rowCounter = 0;

	while (currentLine = sr->ReadLine())
	{
		indexHolder = currentLine->Split(',');

		for (int i = 0; i < COLLUMS; i++)
		{
			SetMapEntry(i, rowCounter, Convert::ToInt32(indexHolder[i]));
		}
		rowCounter++;
	}
}