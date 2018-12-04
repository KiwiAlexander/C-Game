#include "Cell.h"


#include "Cell.h"


Cell::Cell(int startCellType)
{
	cellType = startCellType;
}

int Cell::GetCellType()
{
	return cellType;
}

void Cell::SetCellType(int newCellType)
{
	cellType = newCellType;
}
