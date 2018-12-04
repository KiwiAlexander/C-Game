// ----------------------------------------------------------------------------------------------------- //
//	PROJECT: SHININGDAWN IN628 2016 ASSIGNMENT ONE: 2D ROGUELIKE										 //
//	AUTHOR: ALEXANDER S R PHILLIPS																		 //
//	DATE: 9/10/2016																						 //
//	SUMMARY: ROGUELIKE VIDEO GAME WITH PROCEDURALLY GENERATED DUNGEONS, BATTLE, FOG AND MORE			 //
// ----------------------------------------------------------------------------------------------------- //
#include "Sprite.h"

Sprite::Sprite(Graphics^ startCanvas, array<String^>^ startSpriteSheet, int startNFrames, Random^ startRGen, TileMap^ startSpriteTilemap) // HUMAN SPRITE CONSTRUCTOR
{
	spriteSheets = gcnew array<Bitmap^>(MAXDIRECTIONS);
	for (int i = 0; i < MAXDIRECTIONS; i++)
	{
		spriteSheets[i] = gcnew Bitmap(startSpriteSheet[i]);
		Color transcolor = spriteSheets[i]->GetPixel(0, 0);
		spriteSheets[i]->MakeTransparent(transcolor);
	}

	spritesTilemap = startSpriteTilemap;
	nFrames = startNFrames;
	canvas = startCanvas;
	rGen = startRGen;
	Next = nullptr;
	currentFrame = 0;

	frameWidth = (int)spriteSheets[0]->Width / nFrames;
	frameHeight = spriteSheets[0]->Height;

	velocityDirections = gcnew array<Point>(MAXDIRECTIONS);
	velocityDirections[EAST] = Point(1, 0); //EAST
	velocityDirections[SOUTH] = Point(0, 1); //SOUTH
	velocityDirections[WEST] = Point(-1, 0); //WEST
	velocityDirections[NORTH] = Point(0, -1); //NORTH

	xVel = STARTVEL;
	yVel = STARTVEL;
	yPos = rGen->Next(RANDOMPOSMIN, RANDOMPOSYMAX);
	xPos = rGen->Next(RANDOMPOSMIN, RANDOMPOSXMAX);

	collisionBounds = Rectangle(xPos, yPos, frameWidth, frameHeight);
	spriteDirection = EDirection::EAST;
	isAlive = true;
	fellInPitt = false;

	coins = 0;
	health = 30;
	attack = 2;
	defence = 1;
}
Sprite::Sprite(Graphics^ startCanvas, String^ startSpriteSheet, Random^ startRGen, TileMap^ startSpriteTilemap, int startitemId) //ITEM CONSTRUCTOR
{
	itemId = startitemId;

	spriteSheets = gcnew array<Bitmap^>(2);
	spriteSheets[0] = gcnew Bitmap(startSpriteSheet);
	Color transcolor = spriteSheets[0]->GetPixel(0, 0);
	spriteSheets[0]->MakeTransparent(transcolor);

	spritesTilemap = startSpriteTilemap;

	frameWidth = (int)spriteSheets[0]->Width;
	frameHeight = spriteSheets[0]->Height;

	canvas = startCanvas;
	rGen = startRGen;
	Next = nullptr;
	currentFrame = 0;

	xVel = 0;
	yVel = 0;

	yPos = rGen->Next(RANDOMPOSMIN, RANDOMPOSYMAX);
	xPos = rGen->Next(RANDOMPOSMIN, RANDOMPOSXMAX);

	spriteDirection = EDirection::EAST;

	collisionBounds = Rectangle(xPos, yPos, frameWidth, frameHeight);
	isAlive = true;
}

void Sprite::Draw() // Draws the sprite to the screen after selecting which direction / spriteBitmap.
{
	int startY = 0;
	int startX = currentFrame * frameWidth;
	Rectangle pixelsToDraw = Rectangle(startX, startY, frameWidth, frameHeight);

	Bitmap^ currentBitmap = spriteSheets[spriteDirection];

	canvas->DrawImage(currentBitmap, xPos, yPos, pixelsToDraw, GraphicsUnit::Pixel);
}
void Sprite::Erase() // Outdated with buffer graphics
{
	Rectangle erase = Rectangle(xPos, yPos, frameWidth, frameHeight);
	canvas->FillRectangle(brush, erase);
}
void Sprite::Move() // Moves the sprite XVel/YVel in the direction of the sprite.
{
	xPos += xVel * velocityDirections[spriteDirection].X;
	yPos += yVel * velocityDirections[spriteDirection].Y;
}

void Sprite::UpdateCollisionBox() // Updates the properties in the CollisionBounds rectangle
{
	collisionBounds = Rectangle(xPos, yPos, frameWidth, frameHeight);
}

bool Sprite::CheckCollisionBox(Sprite^ otherSprite) // Passes in a Sprite and matches it against the current sprite
{
	collided = true;

	UpdateCollisionBox();
	otherSprite->UpdateCollisionBox();

	if (collisionBounds.Bottom < otherSprite->collisionBounds.Top) // If statements that check the edges of the two rectangles to see if they have overlapped.
	{
		collided = false;
	}
	if (collisionBounds.Top > otherSprite->collisionBounds.Bottom)
	{
		collided = false;
	}
	if (collisionBounds.Right < otherSprite->collisionBounds.Left)
	{
		collided = false;
	}
	if (collisionBounds.Left > otherSprite->collisionBounds.Right)
	{
		collided = false;
	}
	return collided;
}
bool Sprite::checkCollisionTerrain() // Collision between the sprite and terrain worked out by finding the next step then checking if ontop of a unwalkable tile
{
	// Collision will happen at the feet of the sprite, this makes it clear to the user to navigate and makes it easier to get around tight spaces.
	int nextXPos = (xPos + XPLACEMENT) + xVel * velocityDirections[spriteDirection].X;
	int nextYPos = (yPos + YPLACEMENT) + yVel * velocityDirections[spriteDirection].Y;

	int xTile = nextXPos / SPRITELENGTH;
	int yTile = nextYPos / SPRITELENGTH;

	if (spritesTilemap->TargetWalkable(xTile, yTile) == false)
	{
		return false;
	}
	return true;
}
bool Sprite::checkCollisionFog() // If ontop of a tile that is not walkable the sprite must be in fog because they are all placed on walkable tiles
{
	int xTile = ((xPos + XPLACEMENT) / SPRITELENGTH);
	int yTile = ((yPos + YPLACEMENT) / SPRITELENGTH);

	if (spritesTilemap->TargetWalkable(xTile, yTile) == false)
	{
		return false;
	}
	return true;
}
void Sprite::generateSpriteLocation() // Runs a while loop to check if the sprites location is in the wall and if so updates with a new location untill the tile is walkable
{
	bool spriteInWall = true;
	while (spriteInWall == true)
	{
		int newXPos = rGen->Next(RANDOMPOSMIN, RANDOMPOSXMAX);
		int newYPos = rGen->Next(RANDOMPOSMIN, RANDOMPOSYMAX);

		int spriteLocationX = (newXPos / SPRITELENGTH);
		spriteLocationX = Math::Abs(spriteLocationX);
		int spriteLocationY = (newYPos / SPRITELENGTH);
		spriteLocationY = Math::Abs(spriteLocationY);

		if (spritesTilemap->TargetWalkable(spriteLocationX, spriteLocationY))
		{
			spriteInWall = false;
			setSpriteLocation(newXPos - XPLACEMENT, newYPos - YPLACEMENT);
		}
	}
}

void Sprite::UpdateFrame() // Cycles through the frames in the spriteSheet
{
	currentFrame = (currentFrame + 1) % nFrames;
}
void Sprite::SetSpriteSheet(array<Bitmap^>^ newSpriteSheet, int newNFrames) // Sets up the sprite sheet frameWidth and frameHight which will be used in the animation
{
	spriteSheets = newSpriteSheet;
	nFrames = newNFrames;

	frameWidth = (int)spriteSheets[0]->Width / nFrames;
	frameHeight = spriteSheets[0]->Height;
}
void Sprite::Wander() // Runs a 1/5 to change the direction of the sprite giving it lifelike properties that make it look like its walking around
{
	int randMove = rGen->Next(CHANCEMOVE);

	if (randMove == 0)
	{
		int randDirection = rGen->Next(MAXDIRECTIONS);
		switch (randDirection)
		{
		case 0:
			if (xPos < 1100)
			{
				spriteDirection = EDirection::EAST;
			}
			break;
		case 1:
			if (yPos < 620)
			{
				spriteDirection = EDirection::SOUTH;
			}
			break;
		case 2:
			if (xPos > 100)
			{
				spriteDirection = EDirection::WEST;
			}
			break;
		case 3:
			if (yPos > 100)
			{
				spriteDirection = EDirection::NORTH;
			}
			break;
		default:
			break;
		}
	}
}

// ACCESSORS

Rectangle^ Sprite::getCollisionBox()
{
	UpdateCollisionBox();
	return collisionBounds;
}
Point^ Sprite::getSpriteLocation()
{
	Point^ spriteLocation = gcnew Point(xPos, yPos);
	return spriteLocation;
}
void Sprite::setSpriteLocation(int newXPos, int newYPos)
{
	xPos = newXPos;
	yPos = newYPos;
}
void Sprite::setFellInPitt(bool fell)
{
	fellInPitt = fell;
}
bool Sprite::getFellInPitt()
{
	return fellInPitt;
}

int Sprite::getItemId()
{
	return itemId;
}


int Sprite::getCoins()
{
	return coins;
}
int Sprite::getHealth()
{
	return health;
}
int Sprite::getAttack()
{
	return attack;
}
int Sprite::getDefence()
{
	return defence;
}

void Sprite::setCoins(int newCoins)
{
	coins = newCoins;
}
void Sprite::setHealth(int newHealth)
{
	health = newHealth;
}
void Sprite::setAttack(int newAttack)
{
	attack = newAttack;
}
void Sprite::setDefence(int newDefence)
{
	defence = newDefence;
}