#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class Cell
{
public:
	Cell(int cellType);
	int GetCellType();
	void SetCellType(int newCellType);
private:
	int cellType;
};

