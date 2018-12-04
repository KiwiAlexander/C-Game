// ----------------------------------------------------------------------------------------------------- //
//	PROJECT: SHININGDAWN IN628 2016 ASSIGNMENT ONE: 2D ROGUELIKE										 //
//	AUTHOR: ALEXANDER S R PHILLIPS																		 //
//	DATE: 9/10/2016																						 //
//	SUMMARY: ROGUELIKE VIDEO GAME WITH PROCEDURALLY GENERATED DUNGEONS, BATTLE, FOG AND MORE			 //
// ----------------------------------------------------------------------------------------------------- //

#include "Dungeon.h"


Dungeon::Dungeon(int startnRooms, Random^ startRandom, Sprite^ mainCharaterSprite) // Constructor for the dungeon class
{
	rGen = startRandom;
	mainCharater = mainCharaterSprite;
	nRooms = startnRooms;

	roomArray = gcnew array<Room^>(nRooms);

	cellArray = gcnew array<Cell^, 2>(COLLUMNS, ROWS);
	fogArray = gcnew array<Cell^, 2>(COLLUMNS, ROWS);

	corridorArrayX = gcnew array<Rectangle>(nRooms);
	corridorArrayY = gcnew array<Rectangle>(nRooms);
	corridorArrayMID = gcnew array<Rectangle>(nRooms);

	mainCharaterSprite->setHealth(100);
}

// CONVERTING

array<int, 2>^ Dungeon::ConvertCellToInt(array<Cell^, 2>^ cellArray) // Converts the arry of Cells to array of ints for the TileMap to draw.
{
	array<int, 2>^ dungeonMap = gcnew array<int, 2>(COLLUMNS, ROWS); // Set up 2D int array to pass back

	for (int c = 0; c < COLLUMNS; c++) // Runs through collums and rows so every value is meet
	{
		for (int r = 0; r < ROWS; r++)
		{
			dungeonMap[c, r] = cellArray[c, r]->GetCellType(); // sets these values by getting the cell array type of int.
		}
	}
	return dungeonMap; // Then returns the ready array filled with cell values
}

// DRAWING DUNGEON

void Dungeon::DrawDungeon() // Draws the base dungeon tiles this is to ensure the generated dungeon stays intact after other moving tiles have passed through.
{
	for (int c = 0; c < COLLUMNS; c++) // Two for loops that run for the collum and row of a 2D array.
	{
		for (int r = 0; r < ROWS; r++)
		{
			// -- DUNGEON WALL -- //
			cellArray[c, r] = gcnew Cell(WALL); // Start by setting everything to wall.

			// -- ROOMS -- //
			for (int i = 0; i < nRooms; i++) // for loop to run through all rooms grabbing its rectangle and if any overlap draw the room Cell.
			{
				if (roomArray[i]->getRoom()->Contains(c, r) == true)
				{
					cellArray[c, r] = gcnew Cell(DIRT);
				}
			}

			// -- CORRODORS -- //

			for (int i = 0; i < roomArray->Length - 1; i++) // Same here but just for the corridors
			{
				Rectangle^ tempCorridorX = corridorArrayX[i];
				Rectangle^ tempY = corridorArrayY[i];
				Rectangle^ tempMID = corridorArrayMID[i];

				if (tempCorridorX->Contains(c, r) == true)
				{
					cellArray[c, r] = gcnew Cell(DIRT);
				}
				if (tempY->Contains(c, r) == true)
				{
					cellArray[c, r] = gcnew Cell(DIRT);
				}
				if (tempMID->Contains(c, r) == true)
				{
					cellArray[c, r] = gcnew Cell(DIRT);
				}
			}
			// -- PIT -- //
			if (pitlocation->Contains(c, r) == true) // and lastly the pit
			{
				cellArray[c, r] = gcnew Cell(PIT);
			}

		}
	}

}

// CREATING DUNGEON

void Dungeon::GenerateDungeon() // Creates all the data to be used for the dungeon.
{
	for (int i = 0; i < nRooms; i++) // Generate the rooms to be used and drawn.
	{
		MakeRoom(i); // Passing in i to give it a unique space in the array.
	}

	for (int i = 0; i < roomArray->Length - 1; i++) // Creating corridors 
	{
		int RoomAcenterX = roomArray[i]->getRoom()->Location.X + (roomArray[i]->getRoom()->Width / 2); // Getting the center points for each room.
		int RoomAcenterY = roomArray[i]->getRoom()->Location.Y + (roomArray[i]->getRoom()->Height / 2);

		Point^ ACenter = gcnew Point(RoomAcenterX, RoomAcenterY); // Then saving them into a point

		int RoomBcenterX = roomArray[i + 1]->getRoom()->Location.X + (roomArray[i + 1]->getRoom()->Width / 2);
		int RoomBcenterY = roomArray[i + 1]->getRoom()->Location.Y + (roomArray[i + 1]->getRoom()->Height / 2);

		Point^ BCenter = gcnew Point(RoomBcenterX, RoomBcenterY);

		MakeCorridor(ACenter, BCenter, i); // Then passing the points through to MakeCorridor().
	}

	for (int c = 0; c < COLLUMNS; c++)
	{
		for (int r = 0; r < ROWS; r++)
		{
			cellArray[c, r] = gcnew Cell(WALL);
		}
	}

	bool pitmade = false; // Used to create a portal

	for (int c = 0; c < COLLUMNS; c++)
	{
		for (int r = 0; r < ROWS; r++)
		{
			for (int i = 0; i < nRooms; i++)
			{
				if (roomArray[i]->getRoom()->Contains(c,r) == true)
				{
					cellArray[c, r] = gcnew Cell(DIRT);

					if (pitmade == false)
					{
						cellArray[c, r] = gcnew Cell(PIT); // Creating the pit for the tilemap and collision for the player.
						pitlocation = gcnew Rectangle(c, r, CELLLENGTH, CELLLENGTH);
						pitCollision = gcnew Rectangle((c * TILELENGTH), (r * TILELENGTH), TILELENGTH, TILELENGTH);
						pitmade = true;
					}
				}
			}
		}
	}
}

void Dungeon::MakeRoom(int nRoom)
{
	int startleftCol = rGen->Next(ROOMSTARTMIN, ROOMSTARTXMAX); // Generate start X & Y locations for the rooms.
	int starttopRow = rGen->Next(ROOMSTARTMIN, ROOMSTARTYMAX);
	int startwidthCells = rGen->Next(ROOMMIN, ROOMMAX); // Generate the Width and Height of the room.
	int startheightCells = rGen->Next(ROOMMIN, ROOMMAX);

	Room^ newRoom = gcnew Room(startleftCol, starttopRow, startwidthCells, startheightCells); // Creates a new room with generated values.

	for (int i = 0; i < roomArray->Length; i++) // Runs through each room checking if they intersect with each other, if so makes a new room.
	{
		if (roomArray[i] != nullptr)
		{
			// Turning the pointer of the value http://stackoverflow.com/questions/10172735/how-to-cast-convert-pointer-to-reference-in-c
			if (newRoom->getRoom()->IntersectsWith(*roomArray[i]->getRoom()))
			{
				MakeRoom(i);
			}
		}
	}

	roomArray[nRoom] = newRoom; // add the room to the room array;
}

void Dungeon::MakeCorridor(Point^ roomA, Point^ roomB, int arrayNumber)
{	
	Point^ middle = gcnew Point(roomB->X, roomA->Y); // Find the middle intersecting point of the X & Y corridors

	int lineXLength;
	int lineYLength;
	Rectangle^ lineX;
	Rectangle^ lineY;

	lineX = gcnew Rectangle(roomA->X, roomA->Y, CELLLENGTH, CELLLENGTH);
	lineY = gcnew Rectangle(roomB->X, roomB->Y, CELLLENGTH, CELLLENGTH);

	Rectangle^ lineMid = gcnew Rectangle(roomB->X, roomA->Y, CELLLENGTH, CELLLENGTH);

	corridorArrayMID[arrayNumber] = *lineMid;

	// X
	if (roomA->X > middle->X) // Checks to see if the corridor needs to grow left of right, then changes the starting point of either the intersecting point of middle of roomA-X.
	{
		lineXLength = middle->X - roomA->X;
		lineXLength = Math::Abs(lineXLength);
		lineX = gcnew Rectangle(roomB->X, roomA->Y, lineXLength, CELLLENGTH);
		corridorArrayX[arrayNumber] = *lineX;
	}
	else
	{
		lineXLength = roomA->X - middle->X;
		lineXLength = Math::Abs(lineXLength);
		lineX = gcnew Rectangle(roomA->X, roomA->Y, lineXLength, CELLLENGTH);
		corridorArrayX[arrayNumber] = *lineX;
	}

	// Y

	if (roomB->Y > roomA->Y)
	{
		lineYLength = roomA->Y - roomB->Y;
		lineYLength = Math::Abs(lineYLength);
		lineY = gcnew Rectangle(roomB->X, roomA->Y, CELLLENGTH, lineYLength);
		corridorArrayY[arrayNumber] = *lineY;
	}
	else
	{
		lineYLength = roomA->Y - roomB->Y;
		lineYLength = Math::Abs(lineYLength);
		lineY = gcnew Rectangle(roomB->X, roomB->Y, CELLLENGTH, lineYLength);
		corridorArrayY[arrayNumber] = *lineY;
	}

	for (int c = 0; c < COLLUMNS; c++)
	{
		for (int r = 0; r < ROWS; r++)
		{
			for (int i = 0; i < nRooms; i++)
			{
				if (lineX->Contains(c, r) == true)
				{
					cellArray[c, r] = gcnew Cell(DIRT);
				}
				if (lineY->Contains(c, r) == true)
				{
					cellArray[c, r] = gcnew Cell(DIRT);
				}
				if (lineMid->Contains(c, r) == true)
				{
					cellArray[c, r] = gcnew Cell(DIRT);
				}

			}
		}
	}
}
// CREATING FOG

void Dungeon::GenerateDungeonFog()
{
	int spriteXPos = mainCharater->getSpriteLocation()->X / TILELENGTH; // gets the player location
	int spriteYPos = mainCharater->getSpriteLocation()->Y / TILELENGTH;

	fog = gcnew Rectangle(spriteXPos - FOGWIDTH, spriteYPos - FOGHEIGHT, FOGLENGTH, FOGLENGTH); // makes a rectangle around the player

	for (int c = 0; c < COLLUMNS; c++)
	{
		for (int r = 0; r < ROWS; r++)
		{
			//ANTIFOG
			if (fog->Contains(c, r) == false)
			{
				cellArray[c, r] = gcnew Cell(FOG); // fill in everything with fog that is not that rectangle
			}
		}
	}

}

void Dungeon::fellInPit() // Checks if the player location is ontop of the Pit Cell/Tile, if so a new Dungeon will be generated.
{
	if (pitCollision->IntersectsWith(*mainCharater->getCollisionBox()))
	{
		GenerateDungeon();
		mainCharater->setFellInPitt(false);
	}
}

// ACCESSORS

Rectangle^ Dungeon::getFogRectangle()
{
	return fog;
}
array<Cell^, 2>^ Dungeon::getCellArray()
{
	return cellArray;
}
array<Cell^, 2>^ Dungeon::getFogArray()
{
	return fogArray;
}