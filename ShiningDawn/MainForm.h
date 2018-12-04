// ----------------------------------------------------------------------------------------------------- //
//	PROJECT: SHININGDAWN IN628 2016 ASSIGNMENT ONE: 2D ROGUELIKE										 //
//	AUTHOR: ALEXANDER S R PHILLIPS																		 //
//	DATE: 9/10/2016																						 //
//	SUMMARY: ROGUELIKE VIDEO GAME WITH PROCEDURALLY GENERATED DUNGEONS, BATTLE, FOG AND MORE			 //
// ----------------------------------------------------------------------------------------------------- //
#pragma once

#include "Tile.h"
#include "TileList.h"
#include "TileMap.h"

#include "Sprite.h"
#include "SpriteList.h"

#include "Dungeon.h"
#include "Room.h"
#include "Cell.h"

#include "GameManager.h"

namespace ShiningDawn {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	protected:
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MainForm::timer1_Tick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(120, 650);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(74, 17);
			this->label1->TabIndex = 0;
			this->label1->Text = L"label1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(440, 650);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(74, 17);
			this->label2->TabIndex = 1;
			this->label2->Text = L"label2";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::Transparent;
			this->label3->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(760, 650);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(74, 17);
			this->label3->TabIndex = 2;
			this->label3->Text = L"label3";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::Transparent;
			this->label4->Font = (gcnew System::Drawing::Font(L"OCR A Extended", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(1080, 650);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(74, 17);
			this->label4->TabIndex = 3;
			this->label4->Text = L"label4";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1264, 681);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainForm";
			this->Text = L"ShiningDawn";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::MainForm_KeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::MainForm_KeyUp);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		// ------ FormWork ------ //

		GameManager^ gameManagerMain;

		// ------ Graphics ------ //

		Graphics^ graphics;
		Graphics^ bufferGraphics;
		Bitmap^ bufferImage;
		Brush^ backgroundBrush;

		// ------ MISC ------ //

		Random^ rGen;

	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		// ------ FormSet ------ //

		MainForm::Top = 0;
		MainForm::Left = 0;

		rGen = gcnew Random();

		graphics = CreateGraphics();
		bufferImage = gcnew Bitmap(Width, Height);
		backgroundBrush = gcnew SolidBrush(Color::White);
		bufferGraphics = Graphics::FromImage(bufferImage);

		timer1->Enabled = true;

		gameManagerMain = gcnew GameManager(bufferGraphics, rGen);

	}
	// Timer runs gameRun() which updates most of the game and updates labels on the form then draws the buffered picture
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
	{
		bufferGraphics->FillRectangle(backgroundBrush, 0, 0, Width, Height);

		gameManagerMain->gameRun();

		label1->Text = "Coins: " + gameManagerMain->getMainCharaterSprite()->getCoins();
		label2->Text = "Health: " + gameManagerMain->getMainCharaterSprite()->getHealth();
		label3->Text = "Attack: " + gameManagerMain->getMainCharaterSprite()->getAttack();
		label4->Text = "Defence: " + gameManagerMain->getMainCharaterSprite()->getDefence();

		graphics->DrawImage(bufferImage, 0, 0);
	}
	// Key input changes the direction of the sprite which changes how he walks within Move()
	private: System::Void MainForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
	{
		gameManagerMain->setIsWalking(true);
		switch (e->KeyData)
		{
		case Keys::Left:
			gameManagerMain->getMainCharaterSprite()->spriteDirection = EDirection::WEST;
			break;
		case Keys::Up:
			gameManagerMain->getMainCharaterSprite()->spriteDirection = EDirection::NORTH;
			break;
		case Keys::Down:
			gameManagerMain->getMainCharaterSprite()->spriteDirection = EDirection::SOUTH;
			break;
		case Keys::Right:
			gameManagerMain->getMainCharaterSprite()->spriteDirection = EDirection::EAST;
			break;
		default:
			gameManagerMain->getMainCharaterSprite()->spriteDirection = EDirection::EAST;
			break;
		}
	}
	private: System::Void MainForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
	{
		gameManagerMain->setIsWalking(false);
	}
};
}
