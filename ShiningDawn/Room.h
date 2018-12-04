#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class Room
{
public:
	Room(int startleftCol, int starttopRow, int startwidthCells, int startheightCells);
	Rectangle^ getRoom();
private:
	Rectangle^ room;
};

