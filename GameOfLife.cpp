// GameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "olcConsoleGameEngine.h"
#include <iostream>
using namespace std;



class GameOfLife : public olcConsoleGameEngine
{
public:
	GameOfLife() // initializes the engine with the name of the program in this case game of life
	{
		m_sAppName = L"Game Of Life";
	}

	int m_state;

private: // creating member variables for the two 2D memeber arrays one that stores the data to be output through the engine to the user and one to store the current memory of the cell
	int *m_EngineOutput; 
	int *m_CellMemory;
	
protected:
	virtual bool OnUserCreate() // called by start function that can be overridden to create the resources needed
	{
		m_EngineOutput = new int[ScreenHeight() * ScreenWidth()]; // creating the array for the output using the screen height and width from the olcConsoleGameEngine
		m_CellMemory = new int[ScreenHeight() * ScreenWidth()]; // creating the array for the cell memorys using the screen height and width from the olcConsoleGameEngine

		memset(m_EngineOutput, 0, ScreenHeight() * ScreenWidth() * sizeof(int)); // initializing memory to 0
		memset(m_CellMemory, 0, ScreenHeight() * ScreenWidth() * sizeof(int)); // initializing memory to 0

		auto set = [&](int x, int y, wstring s) // lambda function that sets the value of the cell based off # in order to create patterns easily without extensive lines of code
		{
			int p = 0;
			for (auto c : s)
			{
				m_CellMemory[y * ScreenWidth() + x + p] = c == L'#' ? 1 : 0; // itterates through the string and specifies 1 in the location where a '#' is located
				p++;
			}
		};

		if (m_state == 1) // takes user input to determine which pattern to display
		{
			// R-Pentomino
			set(80, 50, L"  ## ");
			set(80, 51, L" ##  ");
			set(80, 52, L"  #  ");
		}
		else if (m_state == 2)
		{
			// Gosper Glider Gun
			set(60, 45, L"........................#............");
			set(60, 46, L"......................#.#............");
			set(60, 47, L"............##......##............##.");
			set(60, 48, L"...........#...#....##............##.");
			set(60, 49, L"##........#.....#...##...............");
			set(60, 50, L"##........#...#.##....#.#............");
			set(60, 51, L"..........#.....#.......#............");
			set(60, 52, L"...........#...#.....................");
			set(60, 53, L"............##.......................");
		}
		else if (m_state == 3) 
		{
			// Infinite Growth
			set(20, 50, L"########.#####...###......#######.#####");
		}
		else // if no program is selected then randomly seed the array at the start with random values of either 0 or 1 to randomly make cells alive or dead
		{
			for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
				m_CellMemory[i] = rand() % 2;
		}
		return true;
	}
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		auto cell = [&](int x, int y) // lambda function in order to work with 2d cordiante values for the array
		{
			return m_EngineOutput[y * ScreenWidth() + x];
		};

		// Store output state in cell memory in order to interogate the entire array before chaning through once cycle of life
		for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
			m_EngineOutput[i] = m_CellMemory[i];

		for (int x = 1; x < ScreenWidth() - 1; x++)
			for (int y = 1; y < ScreenHeight() - 1; y++)
			{
				// setting up cell co-ordiantes for easy visual identification and reusability
				int NW = cell(x - 1, y + 1);
				int N = cell(x, y + 1);
				int NE = cell(x + 1, y + 1);
				int E = cell(x + 1, y);
				int SE = cell(x + 1, y - 1);
				int S = cell(x, y - 1);
				int SW = cell(x - 1, y - 1);
				int W = cell(x - 1, y);

				// number of neighbours each cell has made by checking each direction
				int numAliveNeighbours = NW + N + NE + E + SE + S + SW + W;

				if (cell(x, y) == 1)
				{
					m_CellMemory[y * ScreenWidth() + x] = numAliveNeighbours == 2 || numAliveNeighbours == 3; // checks if cell is alive and then checks if it has 2 or 3 neighbors if not it will be set as = false and therefore 0
				}
				else 
				{
					m_CellMemory[y * ScreenWidth() + x] = numAliveNeighbours == 3; // asking does the current cell have 3 neighbores true or false if it is true it is set as 1 and 'comes alive' if false it remains 0 and stays dead
				}


				if (cell(x, y) == 1) // if the cell is alive (=1) then draw a white pixel to show it as alive
				{
					Draw(x, y, PIXEL_SOLID, FG_WHITE);
				}
				else // if the cell is alive(=! 1) then draw a Black pixel to show it as dead
				{
					Draw(x, y, PIXEL_SOLID, FG_BLACK);
				}
					
			}

		return true;
	}
};

int main()
{
	// Seed random number generator
	srand(clock());

	// creating text information for scenario choice
	cout << "Please enter program choice:" << endl;
	cout << "1: R-Pentomino" << endl;
	cout << "2: Gosper Glider Gun" << endl;
	cout << "3: Infinite Growth" << endl;
	cout << "Default: Random Seeding" << endl;

	// creating and initializing a state for the game to execute in order to run different scenarios, if anything other than 1,2 or 3 is entered it will default to a random seeding
	int state = 0;
	cin >> state;

	// creating a derived application using the olcConsoleGameEngine package to generate an output
	GameOfLife game;

	// creating the console window size specifing how many pixels wide and how many high (240*150) as well as the size of the pixels in this case 8x8
	game.ConstructConsole(240, 150, 8, 8);

	game.m_state = state;

	// starts the olcConsoleGameEngine allowing an output to be generated starts a thread that allows the running of the engine
	game.Start();
}


