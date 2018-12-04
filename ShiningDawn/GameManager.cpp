// ----------------------------------------------------------------------------------------------------- //
//	PROJECT: SHININGDAWN IN628 2016 ASSIGNMENT ONE: 2D ROGUELIKE										 //
//	AUTHOR: ALEXANDER S R PHILLIPS																		 //
//	DATE: 9/10/2016																						 //
//	SUMMARY: ROGUELIKE VIDEO GAME WITH PROCEDURALLY GENERATED DUNGEONS, BATTLE, FOG AND MORE			 //
// ----------------------------------------------------------------------------------------------------- //
#include "GameManager.h"

GameManager::GameManager(Graphics^ newGraphics, Random^ newRandom) // CONSTRUCTOR
{
	rGen = newRandom;
	graphics = newGraphics;

	generateTileMapMain();
	mainCharaterSprite = generateMainCharacter();
	generateCharacters();

	mainDungeon = gcnew Dungeon(NUMROOMS, rGen, mainCharaterSprite);
	mainDungeon->GenerateDungeon();
	tileMapMain->DrawMap(mainDungeon->ConvertCellToInt(mainDungeon->getCellArray()));

	bool isWalking = false;
}

void GameManager::gameRun() // Game run will run on every timer tick it moves and updates everything in the game
{
	mainDungeon->DrawDungeon();
	tileMapMain->DrawMap(mainDungeon->ConvertCellToInt(mainDungeon->getCellArray()));

	if (mainCharaterSprite->getFellInPitt() != true) // Once the map is created place the sprites in legal locations
	{
		generateCharacters();
		mainCharaterSprite->setFellInPitt(true);
		mainCharaterSprite->generateSpriteLocation();
		rogueSpriteList->updateLocationSprites();
		mageSpriteList->updateLocationSprites();
		itemSpriteList->updateLocationSprites();
	}
	mainDungeon->GenerateDungeonFog(); // Updates fog ontop of the map
	tileMapMain->DrawMap(mainDungeon->ConvertCellToInt(mainDungeon->getCellArray())); // Draws again
	mainDungeon->fellInPit();

	// MAIN CHARACTER

	if (isWalking == true && mainCharaterSprite->checkCollisionTerrain() == true) // Moves on keypress and terrain colloison
	{
		mainCharaterSprite->Move();
		mainCharaterSprite->UpdateFrame();
	}
	mainCharaterSprite->Draw();

	// ROGUE SPRITE

	rogueSpriteList->terrainCollisionSprites();
	rogueSpriteList->wanderSprites();
	rogueSpriteList->updateSprites();
	rogueSpriteList->drawSpritesFog();

	// ROGUE COMBAT

	Sprite^ rogueSprite = rogueSpriteList->checkCollectionSprites(mainCharaterSprite); 
	if (rogueSprite != nullptr)
	{

		Bitmap^ blood = gcnew Bitmap("Images/blood.png");
		graphics->DrawImage(blood, rogueSprite->collisionBounds);
		graphics->DrawImage(blood, mainCharaterSprite->collisionBounds);

		int miss = rGen->Next(DODGECHANCE); // Rogue has a special change to dodge an attack
		if (miss != 1)
		{
			rogueSprite->setHealth(rogueSprite->getHealth() - mainCharaterSprite->getAttack());
		}
		mainCharaterSprite->setHealth(mainCharaterSprite->getHealth() - rogueSprite->getAttack());

		if (rogueSprite->getHealth() <= 0)
		{
			rogueSprite->isAlive = false;
		}
	}

	// MAGE SPRITE

	mageSpriteList->terrainCollisionSprites();
	mageSpriteList->wanderSprites();
	mageSpriteList->updateSprites();
	mageSpriteList->drawSpritesFog();

	// MAGE COMBAT

	Sprite^ mageSprite = mageSpriteList->checkCollectionSprites(mainCharaterSprite);
	if (mageSprite != nullptr)
	{
		Bitmap^ blood = gcnew Bitmap("Images/blood.png");
		graphics->DrawImage(blood, mageSprite->collisionBounds);
		graphics->DrawImage(blood, mainCharaterSprite->collisionBounds);

		mageSprite->setHealth(mageSprite->getHealth() - mainCharaterSprite->getAttack());
		mainCharaterSprite->setHealth(mainCharaterSprite->getHealth() - mageSprite->getAttack());

		if (mageSprite->getHealth() <= 0)
		{
			mageSprite->isAlive = false;
		}
	}

	//itemSpriteList->drawSprites();
	itemSpriteList->drawSpritesFog();

	Sprite^ itemSprite = itemSpriteList->checkCollectionSprites(mainCharaterSprite); // Checks what item the player has collided with then updates the values accordingly
	if (itemSprite != nullptr)
	{
		switch (itemSprite->getItemId())
		{
		case(0) :
			mainCharaterSprite->setCoins(mainCharaterSprite->getCoins() + rGen->Next(MAXCOINS));
			break;
		case(1) :
			mainCharaterSprite->setHealth(mainCharaterSprite->getHealth() + HEALTHINCRASE);
			break;
		case(2) :
			mainCharaterSprite->setAttack(mainCharaterSprite->getAttack() + STATINCRASE);
			break;
		case(3) :
			mainCharaterSprite->setDefence(mainCharaterSprite->getDefence() + STATINCRASE);
			break;
		default:
			break;
		}
		itemSprite->isAlive = false;
	}

	// Removes the sprites from drawing once the are dead
	mageSpriteList->deleteAllDeadSprites();
	rogueSpriteList->deleteAllDeadSprites();
	itemSpriteList->deleteAllDeadSprites();
}

void GameManager::generateTileMapMain() // Sets up all the graphics the tileMap needs
{
	tilelistMain = gcnew TileList();
	tileMapMain = gcnew TileMap(tilelistMain, graphics);

	otherTiles = gcnew array<Tile^>(MAXTILES);
	otherTileBitmaps = gcnew array<Bitmap^>(MAXTILES);

	otherTileBitmaps[0] = gcnew Bitmap("Images/dirtFloor.png"); // assigns a bitmap to the bitmap array
	otherTileBitmaps[1] = gcnew Bitmap("Images/dungeonWall.png");
	otherTileBitmaps[2] = gcnew Bitmap("Images/fog.png");
	otherTileBitmaps[3] = gcnew Bitmap("Images/antifog.png");
	otherTileBitmaps[4] = gcnew Bitmap("Images/door.png");
	otherTileBitmaps[4] = gcnew Bitmap("Images/pit.png");

	for (int i = 0; i < NUMTILES; i++)
	{
		otherTiles[i] = gcnew Tile(otherTileBitmaps[i]); // uses those bitmaps to create tiles
	}

	for (int i = 0; i < TILEENTRY; i++)
	{
		tilelistMain->SetTileArrayEntry(i + 1, otherTiles[i]); // sets there ertry to be used later with that key
	}

	// Setting tiles walkable
	tilelistMain->SetTileWalkable(1, true);
	tilelistMain->SetTileWalkable(2, false);
	tilelistMain->SetTileWalkable(3, false);
	tilelistMain->SetTileWalkable(4, true);
	tilelistMain->SetTileWalkable(6, true);
}
void GameManager::generateCharacters() // Creates and sets up mage, rogue and items.
{
	mageSpriteList = gcnew SpriteList();
	rogueSpriteList = gcnew SpriteList();
	itemSpriteList = gcnew SpriteList();

	mageSpriteList = generateMageSprite();
	rogueSpriteList = generateRogueSprite();
	itemSpriteList = generateItems();
}

Sprite^ GameManager::generateMainCharacter() // sets up the 4 strings to bitmaps that will be sent to the sprite constructor to make a working sprite
{
	int framesPerSheet = FRAMESPERSHEET;
	int directions = DIRECTIONS;

	String^ east = gcnew String("Images/Characters/Human/humanEast.png");
	String^ south = gcnew String("Images/Characters/Human/humanSouth.png");
	String^ west = gcnew String("Images/Characters/Human/humanWest.png");
	String^ north = gcnew String("Images/Characters/Human/humanNorth.png");

	array<String^>^ humanImages = gcnew array<String^>(directions);

	humanImages[EAST] = east;
	humanImages[SOUTH] = south;
	humanImages[WEST] = west;
	humanImages[NORTH] = north;

	return gcnew Sprite(graphics, humanImages, framesPerSheet, rGen, tileMapMain);
}
SpriteList^ GameManager::generateMageSprite() // Same thing as mainCharacter
{
	int framesPerSheet = FRAMESPERSHEET;
	int directions = DIRECTIONS;

	String^ east = gcnew String("Images/Characters/Mage/mageEast.png");
	String^ south = gcnew String("Images/Characters/Mage/mageSouth.png");
	String^ west = gcnew String("Images/Characters/Mage/mageWest.png");
	String^ north = gcnew String("Images/Characters/Mage/mageNorth.png");

	array<String^>^ mageImages = gcnew array<String^>(directions);

	mageImages[EAST] = east;
	mageImages[SOUTH] = south;
	mageImages[WEST] = west;
	mageImages[NORTH] = north;

	for (int i = 0; i < NUMENEMIES; i++)
	{
		Sprite^ newSprite = gcnew Sprite(graphics, mageImages, framesPerSheet, rGen, tileMapMain);
		mageSpriteList->addSprite(newSprite);
	}
	return mageSpriteList;
}
SpriteList^ GameManager::generateRogueSprite() // same here for rogue
{
	int framesPerSheet = FRAMESPERSHEET;
	int directions = DIRECTIONS;

	String^ east = gcnew String("Images/Characters/Rogue/rogueEast.png");
	String^ south = gcnew String("Images/Characters/Rogue/rogueSouth.png");
	String^ west = gcnew String("Images/Characters/Rogue/rogueWest.png");
	String^ north = gcnew String("Images/Characters/Rogue/rogueNorth.png");

	array<String^>^ rogueImages = gcnew array<String^>(directions);

	rogueImages[0] = east;
	rogueImages[1] = south;
	rogueImages[2] = west;
	rogueImages[3] = north;

	for (int i = 0; i < NUMENEMIES; i++)
	{
		Sprite^ newSprite = gcnew Sprite(graphics, rogueImages, framesPerSheet, rGen, tileMapMain);
		rogueSpriteList->addSprite(newSprite);
	}
	return rogueSpriteList;
}

SpriteList^ GameManager::generateItems() // items are similar but have a item ID
{
	String^ coins = gcnew String("Images/Items/coins.png");
	String^ potion = gcnew String("Images/Items/potion.png");
	String^ sword = gcnew String("Images/Items/sword.png");
	String^ shield = gcnew String("Images/Items/shield.png");

	array<String^>^ itemImages = gcnew array<String^>(4);

	itemImages[0] = coins;
	itemImages[1] = potion;
	itemImages[2] = sword;
	itemImages[3] = shield;

	for (int i = 0; i < itemImages->Length; i++) // for loop i creates the item id
	{
		Sprite^ newSprite = gcnew Sprite(graphics, itemImages[i], rGen, tileMapMain, i);
		itemSpriteList->addSprite(newSprite);
	}
	return itemSpriteList;
}

// ACCESSORS

Sprite^ GameManager::getMainCharaterSprite()
{
	return mainCharaterSprite;
}

void GameManager::setIsWalking(bool newIsWalking)
{
	isWalking = newIsWalking;
}
bool GameManager::getIsWalking()
{
	return isWalking;
}
void GameManager::setgenLocation(bool newGenLocation)
{
	genLocation = newGenLocation;
}

Rectangle^ GameManager::fogRec()
{
	return mainDungeon->getFogRectangle();
}

