// Project3.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <istream>
#include <sstream>
#include <SFML/Graphics.hpp>

int TotalMines = 0;
bool Lost = false;
bool Win = false;
bool DebugMode = false;
int TileDisplay[16][25]; // Stores Sprite to Display
int TileMaster[16][25];	// Stores Actual Tile State Behind the Scenes ... Sneaky huh?

bool CheckWinCondition()
{
	// Assume game has been one uunless we find one
	bool GameWon = true;

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			// Check that every flagged tile on display has a mine
			/*if (TileDisplay[i][j] == 11 && TileMaster[i][j] != 0) {
				GameWon = false;
			}*/
			// Check that every non mine tile is not a hidden tile
			if (TileMaster[i][j] != 0 && TileDisplay[i][j] == 10)
			{
				GameWon = false;
			}
		}
	}
	return GameWon;
}

void ReadTestFile(std::string filename)
{
	TotalMines = 0;
	std::ifstream File;
	File.open(filename);
	if (File.is_open())
	{
		std::string line = "";
		int i = 0;
		while (getline(File, line))
		{
			int length = line.length();
			for (int j = 0; j < length; j++)
			{
				char temp = line[j];
				int tempInt = temp - 48;
				if (tempInt == 1)
				{ // 1 in the file means a mine
					TileMaster[i][j] = 0;
					TotalMines++; // make sure to increase totalmines!
				}
				if (tempInt == 0) // 0 in the file means an empty space
					TileMaster[i][j] = 9;
				if (j == 24)
					i++;
			}
		}
	}
	File.close();

	for (int i = 0; i <= 15; i++) // Row
	{
		for (int j = 0; j <= 24; j++) // Column
		{
			int n = 0;								 // Stores what Number Tile should have
			if (TileMaster[i][j] == 9) // If Tile is Revealed(9), go on.
			{
				if (TileMaster[i - 1][j - 1] == 0 && (i != 0) && (j != 0))
					n++; // Top Left
				if (TileMaster[i - 1][j] == 0 && (i != 0))
					n++; // Top
				if (TileMaster[i - 1][j + 1] == 0 && (i != 0) && (j != 24))
					n++; // Top Right
				if (TileMaster[i][j + 1] == 0 && (j != 24))
					n++; // Right
				if (TileMaster[i + 1][j + 1] == 0 && (i != 15) && (j != 24))
					n++; // Bottom Right
				if (TileMaster[i + 1][j] == 0 && (i != 15))
					n++; // Bottom
				if (TileMaster[i + 1][j - 1] == 0 && (i != 15) && (j != 0))
					n++; // BottomLeft
				if (TileMaster[i][j - 1] == 0 && (j != 0))
					n++; // Left
				if (n == 0)
				{
					n = 9; // if no mines nearby, assign TileMaster[i][j] as revealed tile.
				}
				TileMaster[i][j] = n;
			}
		}
	}
	// Hide the new tiles
	for (int i = 0; i <= 15; i++)
	{
		for (int j = 0; j <= 24; j++)
		{
			TileDisplay[i][j] = 10;
		}
	}
	// Print out mines int the console
	std::cout << "Total Mines: " << TotalMines << std::endl;
	for (int i = 0; i <= 15; i++)
	{
		for (int j = 0; j <= 24; j++)
		{
			if (TileMaster[i][j] == 0)
				std::cout << 1;
			else
				std::cout << 0;
		}
		std::cout << std::endl;
	}
}

void RevealAdjacentTiles(int x, int y)
{
	if (TileMaster[y][x] == 9)
	{ // Tile must be revealed and not contain a number
		/********** If an adjacent tile is empty of has a number, reveal it **********/
		if ((y != 0) && (x != 0))
		{ // Top Left
			int temp = TileMaster[y - 1][x - 1];
			if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8)
				if (TileDisplay[y - 1][x - 1] != 11)
					TileDisplay[y - 1][x - 1] = TileMaster[y - 1][x - 1];
		}
		if (y != 0)
		{ // Top
			int temp = TileMaster[y - 1][x];
			if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8 || temp == 9)
				if (TileDisplay[y - 1][x] != 11)
					TileDisplay[y - 1][x] = TileMaster[y - 1][x];
		}
		if ((y != 0) && (x != 24))
		{ // Top Right
			int temp = TileMaster[y - 1][x + 1];
			if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8)
				if (TileDisplay[y - 1][x + 1] != 11)
					TileDisplay[y - 1][x + 1] = TileMaster[y - 1][x + 1];
		}
		if (x != 24)
		{ // Right
			int temp = TileMaster[y][x + 1];
			if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8 || temp == 9)
				if (TileDisplay[y][x + 1] != 11)
					TileDisplay[y][x + 1] = TileMaster[y][x + 1];
		}
		if ((y != 15) && (x != 24))
		{ // Bottom Right
			int temp = TileMaster[y + 1][x + 1];
			if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8)
				if (TileDisplay[y + 1][x + 1] != 11)
					TileDisplay[y + 1][x + 1] = TileMaster[y + 1][x + 1];
		}
		if (y != 15)
		{ // Bottom
			int temp = TileMaster[y + 1][x];
			if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8 || temp == 9)
				if (TileDisplay[y + 1][x] != 11)
					TileDisplay[y + 1][x] = TileMaster[y + 1][x];
		}
		if ((y != 15) && (x != 0))
		{ // Bottom Left
			int temp = TileMaster[y + 1][x - 1];
			if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8)
				if (TileDisplay[y + 1][x - 1] != 11)
					TileDisplay[y + 1][x - 1] = TileMaster[y + 1][x - 1];
		}
		if (x != 0)
		{ // Left
			int temp = TileMaster[y][x - 1];
			if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8 || temp == 9)
				if (TileDisplay[y][x - 1] != 11)
					TileDisplay[y][x - 1] = TileMaster[y][x - 1];
		}

		TileMaster[y][x] = 12; // Set Revealed Tile as a Tile that has already been checked

		/********** If a top, right, bottom, or left tile is empty, reveal it  **********/
		if ((y != 15) && TileMaster[y + 1][x] == 9)
		{ // Bottom
			int newY = y + 1;
			RevealAdjacentTiles(x, newY);
		}
		if ((x != 0) && TileMaster[y][x - 1] == 9)
		{ // Left
			int newX = x - 1;
			RevealAdjacentTiles(newX, y);
		}
		if ((x != 24) && TileMaster[y][x + 1] == 9)
		{ // Right
			int newX = x + 1;
			RevealAdjacentTiles(newX, y);
		}
		if ((y != 0) && TileMaster[y - 1][x] == 9)
		{ // Top
			int newY = y - 1;
			RevealAdjacentTiles(x, newY);
		}
	}
}

void ShuffleBoard()
{
	TotalMines = 0;
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> distribute8(1, 8);

	// Populate Master with random data Mine or Not
	while (TotalMines < 50)
	{
		TotalMines = 0;
		for (int i = 0; i <= 15; i++)
		{
			for (int j = 0; j <= 24; j++)
			{
				TileDisplay[i][j] = 10; // Assign all TileDisplay objects to hidden
				if (distribute8(rng) == 8 && TotalMines < 50)
				{
					TileMaster[i][j] = 0; // Mine is 0
					TotalMines++;
				}
				else
				{
					TileMaster[i][j] = 9; // Assign all non mine tiles as revealed(9), this will be changed later
				}
			}
		}
	}
	std::cout << "Total Mines: " << TotalMines << std::endl; // Print out mines int the console
	for (int i = 0; i <= 15; i++)
	{
		for (int j = 0; j <= 24; j++)
		{
			if (TileMaster[i][j] == 0)
				std::cout << 1;
			else
				std::cout << 0;
		}
		std::cout << std::endl;
	}

	// ********** Assign Number Values to Adjacent Tiles with Mines **********
	for (int i = 0; i <= 15; i++) // Row
	{
		for (int j = 0; j <= 24; j++) // Column
		{
			int n = 0;								 // Stores what Number Tile should have
			if (TileMaster[i][j] == 9) // If Tile is Revealed(9), go on.
			{
				if (TileMaster[i - 1][j - 1] == 0 && (i != 0) && (j != 0))
					n++; // Top Left
				if (TileMaster[i - 1][j] == 0 && (i != 0))
					n++; // Top
				if (TileMaster[i - 1][j + 1] == 0 && (i != 0) && (j != 24))
					n++; // Top Right
				if (TileMaster[i][j + 1] == 0 && (j != 24))
					n++; // Right
				if (TileMaster[i + 1][j + 1] == 0 && (i != 15) && (j != 24))
					n++; // Bottom Right
				if (TileMaster[i + 1][j] == 0 && (i != 15))
					n++; // Bottom
				if (TileMaster[i + 1][j - 1] == 0 && (i != 15) && (j != 0))
					n++; // BottomLeft
				if (TileMaster[i][j - 1] == 0 && (j != 0))
					n++; // Left
				if (n == 0)
				{
					n = 9; // if no mines nearby, assign TileMaster[i][j] as revealed tile.
				}
				TileMaster[i][j] = n;
			}
		}
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

	sf::Texture DebugText;
	sf::Texture DigitsText;
	sf::Texture HappyFaceText;
	sf::Texture LostFaceText;
	sf::Texture WinFaceText;
	sf::Texture FlagText;
	sf::Texture MineText;
	sf::Texture OneText;
	sf::Texture TwoText;
	sf::Texture ThreeText;
	sf::Texture FourText;
	sf::Texture FiveText;
	sf::Texture SixText;
	sf::Texture SevenText;
	sf::Texture EightText;
	sf::Texture TestOneText;
	sf::Texture TestTwoText;
	sf::Texture HiddenTileText;
	sf::Texture RevealedTileText;

	int TileWidth = 32;

	if (!DebugText.loadFromFile("images/debug.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!DigitsText.loadFromFile("images/digits.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!HappyFaceText.loadFromFile("images/face_happy.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!LostFaceText.loadFromFile("images/face_lose.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!WinFaceText.loadFromFile("images/face_win.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!FlagText.loadFromFile("images/flag.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!MineText.loadFromFile("images/mine.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!OneText.loadFromFile("images/number_1.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!TwoText.loadFromFile("images/number_2.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!ThreeText.loadFromFile("images/number_3.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!FourText.loadFromFile("images/number_4.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!FiveText.loadFromFile("images/number_5.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!SixText.loadFromFile("images/number_6.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!SevenText.loadFromFile("images/number_7.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!EightText.loadFromFile("images/number_8.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!TestOneText.loadFromFile("images/test_1.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!TestTwoText.loadFromFile("images/test_2.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!HiddenTileText.loadFromFile("images/tile_hidden.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}
	if (!RevealedTileText.loadFromFile("images/tile_revealed.png"))
	{
		std::cout << "Error loading file!" << std::endl;
	}

	sf::Sprite Debug(DebugText);
	sf::Sprite Digits(DigitsText);
	sf::Sprite HappyFace(HappyFaceText);
	sf::Sprite LostFace(LostFaceText);
	sf::Sprite WinFace(WinFaceText);
	sf::Sprite Flag(FlagText);
	sf::Sprite Mine(MineText);
	sf::Sprite One(OneText);
	sf::Sprite Two(TwoText);
	sf::Sprite Three(ThreeText);
	sf::Sprite Four(FourText);
	sf::Sprite Five(FiveText);
	sf::Sprite Six(SixText);
	sf::Sprite Seven(SevenText);
	sf::Sprite Eight(EightText);
	sf::Sprite TestOne(TestOneText);
	sf::Sprite TestTwo(TestTwoText);
	sf::Sprite RevealedTile;
	RevealedTile.setTexture(RevealedTileText);
	//RevealedTile.move(64, 64);
	sf::Sprite HiddenTile(HiddenTileText);
	sf::Sprite DudSprite; // WHY DOES THIS WORK !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// ^^^^^^^^^^^^^^^^^^  DO NOT REMOVE THIS SPRITE, DON"T YOU DARE TOUCH IT!!!!!!!!!!!!

	// Add mines randomly and insert numbers where needed
	ShuffleBoard();

	while (window.isOpen())
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Gives a constant Pos for the mouse
		int x = mousePos.x / TileWidth;
		int y = mousePos.y / TileWidth;

		// Message Loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			/********** Mouse Events **********/
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left) // Left Click
				{
					int temp = TileDisplay[y][x];
					/******************** If game hasn't been lost ********************/
					if (Lost == false && Win == true)
					{
						// If win face is pressed, reshuffle board, with hidden tiles displayed
						if ((mousePos.x >= 368 && mousePos.x < 432) && (mousePos.y >= 512 && mousePos.y <= 576))
						{
							std::cout << "Win Face Pressed!" << std::endl;
							std::cout << "Board Reshuffled" << std::endl;
							ShuffleBoard();
							Win = false;
						}
					}
					else if (Lost == false)
					{
						// If smiley face is pressed, reshuffle board, with hidden tiles displayed
						if ((mousePos.x >= 368 && mousePos.x < 432) && (mousePos.y >= 512 && mousePos.y <= 576))
						{
							std::cout << "Smiley Face Pressed!" << std::endl;
							std::cout << "Board Reshuffled" << std::endl;
							ShuffleBoard();
							Win = false;
						}
						// If DEBUG button is pressed, reveal all tiles
						else if ((x >= 15 && x < 17) && (y >= 16 && y < 18))
						{ //Debug.setPosition(480, 512);
							std::cout << "Debug Botton Pressed" << std::endl;
							if (DebugMode == false)
							{
								DebugMode = true;
								for (int i = 0; i <= 15; i++)
								{
									for (int j = 0; j <= 24; j++)
									{
										TileDisplay[i][j] = 10;
										if (TileMaster[i][j] == 0)
										{
											TileDisplay[i][j] = 0;
										}
									}
								}
							}
							else if (DebugMode == true)
							{
								DebugMode = false;
								for (int i = 0; i <= 15; i++)
								{
									for (int j = 0; j <= 24; j++)
									{
										TileDisplay[i][j] = 10;
									}
								}
							}
						}
						// If TEST ONE is pressed, change TileMaster
						else if ((x >= 17 && x < 19) && (y >= 16 && y < 18))
						{
							//TestOne.setPosition(544, 512);
							ReadTestFile("boards/testboard.brd");
							std::cout << "Test One Succesfully Loaded!" << std::endl;
						}
						// If TEST TWO is pressed, change TileMaster
						else if ((x >= 19 && x < 21) && (y >= 16 && y < 18))
						{
							//TestTwo.setPosition(608, 512);
							ReadTestFile("boards/testboard2.brd");
							std::cout << "Test Two Succesfully Loaded!" << std::endl;
						}
						/***** Only reveal tiles if they are within the tile set area *****/
						else if ((x >= 0 && x <= 25) && (y >= 0 & y < 16))
						{
							if (temp == 13)
								break;
							TileDisplay[y][x] = TileMaster[y][x]; // REVEAL TILE
							if (TileMaster[y][x] == 0)
							{ // REVEALING MINE should stop game
								for (int i = 0; i <= 15; i++)
								{
									for (int j = 0; j <= 24; j++)
									{
										TileDisplay[i][j] = TileMaster[i][j];
									}
								}
								Lost = true;
							}
							// If revealed tile is hidden, reveal other tiles next to it.
							else if (TileMaster[y][x] == 9)
							{
								RevealAdjacentTiles(x, y);
							}
							// If it's a flag or revealed don't do anything
							else if (temp == 11 || temp == 9)
								break;
							// If it's a number, don't do anything
							else if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8)
								break;
							if (TotalMines == 0)
							{
								Win = CheckWinCondition();
							}
							Win = CheckWinCondition();
						}
					}
					/******************** If game is lost ********************/
					else if (Lost == true)
					{
						// If Lost face is pressed, reshuffle board, with hidden tiles displayed
						// HappyFace.setPosition(368, 512);
						if ((mousePos.x >= 368 && mousePos.x < 432) && (mousePos.y >= 512 && mousePos.y <= 576))
						{
							std::cout << "Lost Face Pressed!" << std::endl;
							std::cout << "Board Reshuffled" << std::endl;
							ShuffleBoard();
							Lost = false;
							Win = false;
						}
					}
				}
				else if (event.key.code == sf::Mouse::Right) // Right Click
				{
					/***** Only flag tiles within the tile board area *****/
					if ((x >= 0 && x <= 25) && (y >= 0 & y < 16))
					{
						if (Lost == false && Win == false)
						{
							int temp = TileDisplay[y][x];
							// If tile is hidden, flag it
							if (TileDisplay[y][x] == 10)
							{
								TileDisplay[y][x] = 11;
								TotalMines--;
								if (TotalMines == 0)
									Win = CheckWinCondition();
							}
							else if (TileDisplay[y][x] == 11)
							{
								TileDisplay[y][x] = 10;
								TotalMines++;
							}
							// if tile haas been revealed (and/or is a number), don't flag it
							else if (temp == 1 || temp == 2 || temp == 3 || temp == 4 || temp == 5 || temp == 6 || temp == 7 || temp == 8 || temp == 9)
								break;

							else if (TileDisplay[y][x] == 0)
							{
								TileDisplay[y][x] = 13;
							}
							else if (TileDisplay[y][x] == 13)
							{
								TileDisplay[y][x] = 0;
							}
							else
							{
								TileDisplay[y][x] = 10;
							}
						}
					}
				}
			}
		}
		// 1. Update()
		// 2. Draw()
		window.clear();

		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 25; j++)
			{
				if (TileDisplay[i][j] == 0) // Mine
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					Mine.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(Mine);
				}
				if (TileDisplay[i][j] == 1) // One
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					One.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(One);
				}
				if (TileDisplay[i][j] == 2) // Two
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					Two.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(Two);
				}
				if (TileDisplay[i][j] == 3) // Three
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					Three.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(Three);
				}
				if (TileDisplay[i][j] == 4) // Four
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					Four.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(Four);
				}
				if (TileDisplay[i][j] == 5) // Five
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					Five.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(Five);
				}
				if (TileDisplay[i][j] == 6) // Six
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					Six.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(Six);
				}
				if (TileDisplay[i][j] == 7) // Seven
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					Seven.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(Seven);
				}
				if (TileDisplay[i][j] == 8) // Eight
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					Eight.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(Eight);
				}
				if (TileDisplay[i][j] == 9) // Revealed Tile
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
				}
				if (TileDisplay[i][j] == 10) // Hidden Tile
				{
					HiddenTile.setPosition(j * TileWidth, i * TileWidth);
					window.draw(HiddenTile);
				}
				if (TileDisplay[i][j] == 11) // Flag
				{
					HiddenTile.setPosition(j * TileWidth, i * TileWidth);
					window.draw(HiddenTile);
					Flag.setPosition(j * TileWidth, i * TileWidth);
					window.draw(Flag);
				}
				if (TileDisplay[i][j] == 12) // Revealed Tile that has been revealed already, used to stope recursive func
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
				}
				if (TileDisplay[i][j] == 13) // Mine with a flag, used only in debug mode
				{
					RevealedTile.setPosition(j * TileWidth, i * TileWidth);
					Mine.setPosition(j * TileWidth, i * TileWidth);
					window.draw(RevealedTile);
					window.draw(Mine);
					Flag.setPosition(j * TileWidth, i * TileWidth);
					window.draw(Flag);
				}
			}
		}
		if (Lost == false && Win == false)
		{
			HappyFace.setPosition(368, 512);
			window.draw(HappyFace);
		}
		if (Lost == true)
		{
			LostFace.setPosition(368, 512);
			window.draw(LostFace);
		}
		if (Win == true && Lost == false)
		{
			WinFace.setPosition(368, 512);
			window.draw(WinFace);
		}
		Debug.setPosition(480, 512);
		window.draw(Debug);

		TestOne.setPosition(544, 512);
		window.draw(TestOne);

		TestTwo.setPosition(608, 512);
		window.draw(TestTwo);

		// Draw Digits representing Total Mine count
		int hundreds = TotalMines / 100;
		int tens = TotalMines / 10;
		int ones = TotalMines % 10;

		if (hundreds == 0)
		{
			Digits.setPosition(0, 512);
			Digits.setTextureRect(sf::IntRect(0, 0, 21, 32));
			window.draw(Digits);
		}
		else
		{
			Digits.setPosition(0, 512);
			Digits.setTextureRect(sf::IntRect(21 * hundreds, 0, 21, 32));
			window.draw(Digits);
		}
		if (tens == 0)
		{
			Digits.setPosition(21, 512);
			Digits.setTextureRect(sf::IntRect(0, 0, 21, 32));
			window.draw(Digits);
		}
		else
		{
			Digits.setPosition(21, 512);
			Digits.setTextureRect(sf::IntRect(21 * tens, 0, 21, 32));
			window.draw(Digits);
		}
		if (ones == 0)
		{
			Digits.setPosition(42, 512);
			Digits.setTextureRect(sf::IntRect(0, 0, 21, 32));
			window.draw(Digits);
		}
		else
		{
			Digits.setPosition(42, 512);
			Digits.setTextureRect(sf::IntRect(21 * ones, 0, 21, 32));
			window.draw(Digits);
		}
		window.display();
	}
	return 0;
}