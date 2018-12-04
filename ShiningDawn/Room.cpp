#include "Room.h"


Room::Room(int startleftCol, int starttopRow, int startwidthCells, int startheightCells)
{
	room = gcnew Rectangle(startleftCol, starttopRow, startwidthCells, startheightCells);
}
Rectangle^ Room::getRoom()
{
	return room;
}
