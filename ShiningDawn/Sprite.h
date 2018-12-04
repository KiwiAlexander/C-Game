// ----------------------------------------------------------------------------------------------------- //
//	PROJECT: SHININGDAWN IN628 2016 ASSIGNMENT ONE: 2D ROGUELIKE										 //
//	AUTHOR: ALEXANDER S R PHILLIPS																		 //
//	DATE: 9/10/2016																						 //
//	SUMMARY: ROGUELIKE VIDEO GAME WITH PROCEDURALLY GENERATED DUNGEONS, BATTLE, FOG AND MORE			 //
// ----------------------------------------------------------------------------------------------------- //
#pragma once
#include "TileMap.h"

#define MAXDIRECTIONS 4
#define STARTVEL 5
#define CHANCEMOVE 5
#define RANDOMPOSMIN 50
#define RANDOMPOSXMAX 900
#define RANDOMPOSYMAX 620
#define SPRITELENGTH 16
#define XPLACEMENT 12
#define YPLACEMENT 18

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public enum EDirection
{
	EAST, SOUTH, WEST, NORTH
};

ref class Sprite
{
private:

	Graphics^ canvas;
	array<Bitmap^>^ spriteSheets;
	Bitmap^ itemSprite;
	Random^ rGen;
	SolidBrush^ brush;
	array<Point>^velocityDirections;
	TileMap^ spritesTilemap;

	int nFrames;
	int currentFrame;
	int xPos;
	int yPos;
	int frameWidth;
	int frameHeight;
	int xVel;
	int yVel;
	int itemId;
	bool collided;
	bool fellInPitt;

	int coins;
	int health;
	int attack;
	int defence;

public:

	EDirection spriteDirection;
	property bool isAlive;
	property Rectangle collisionBounds;

	Sprite(Graphics^ startCanvas, array<String^>^ startSpriteSheet, int startNFrames, Random^ startRGen, TileMap^ startSpriteTilemap);
	Sprite(Graphics^ startCanvas, String^ startSpriteSheet, Random^ startRGen, TileMap^ startSpriteTilemap, int startitemId);

	void Draw();
	void Erase();
	void Move();
	void UpdateFrame();
	void SetSpriteSheet(array<Bitmap^>^ newSpriteSheet, int newNFrames);
	void Wander();
	bool CheckCollisionBox(Sprite^ checkSprite);
	void UpdateCollisionBox();
	Rectangle^ getCollisionBox();
	bool checkCollisionTerrain();
	bool checkCollisionFog();
	Point^ getSpriteLocation();
	void setSpriteLocation(int newXPos, int newYPos);
	void generateSpriteLocation();
	void setFellInPitt(bool fell);
	bool getFellInPitt();

	int getItemId();

	int getCoins();
	int getHealth();
	int getAttack();
	int getDefence();

	void setCoins(int newCoins);
	void setHealth(int newHealth);
	void setAttack(int newAttack);
	void setDefence(int newDefence);
	

	Sprite^ Next;
};

