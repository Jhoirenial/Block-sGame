/////////////////////////
// Include libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>


/////////////////////////
// Game settings 
const int rowsCount = 10;
const int columnsCount = 15;

const unsigned char symbolHero = 2;
const unsigned char symbolWall = 177;
const unsigned char symbolBox = 254;
const unsigned char symbolExit = 176;

// Make a second level
const unsigned char symbolBomb = 15;

const unsigned char levelData0[rowsCount][columnsCount + 1] =  {
																	"#####2#########",
																	"#  X   #   X ##",
																	"# X ### X  #  #",
																	"#X X  X  ## X #",
																	"# X    ##  #  #",
																	"#######    # X#",
																	"#B  X   XX #X #",
																	"#XXX # # X   ##",
																	"#1 X #   X X  #",
																	"###############",
																};

// Second level
const unsigned char levelData1[rowsCount][columnsCount + 1] =  {
																	"###############",
																	"#X X    X  X  #",
																	"# X ## X   X ##",
																	"##  X###XXX  ##",
																	"## X #   #2 X #",
																	"#XX###  X### X#",
																	"#B X   # X #  #",
																	"#X #X X#   XXX#",
																	"#1X#   ### X  #",
																	"###############",
																};


/////////////////////////
// Logics variables
HANDLE consoleHandle = 0;
bool isGameActive = true;

unsigned char levelData[rowsCount][columnsCount];
int heroRow = 0;
int heroColumn = 0;
int currentLevel = 0;

/////////////////////////
// Functions
void SetupSystem()
{
	srand( time ( 0 ) );

	consoleHandle = GetStdHandle( STD_OUTPUT_HANDLE );
}

void Initialise()
{
	if (currentLevel == 0)
	{
		// Load level
		for (int r = 0; r < rowsCount; r++)
		{
			for (int c = 0; c < columnsCount; c++)
			{
				unsigned char symbol = levelData0[r][c];

				switch (symbol)
				{
					// Wall
				case '#':
				{
					levelData[r][c] = symbolWall;
					break;
				}

				// Hero
				case '1':
				{
					levelData[r][c] = symbolHero;

					// Catch hero position
					heroRow = r;
					heroColumn = c;
					break;
				}

				// Exit
				case '2':
				{
					levelData[r][c] = symbolExit;
					break;
				}

				// Box
				case 'X':
				{
					levelData[r][c] = symbolBox;
					break;
				}

				// Bomb 
				case 'B':
				{
					levelData[r][c] = symbolBomb;
					break;
				}

				// Other symbols (like spaces)
				default:
				{
					levelData[r][c] = symbol;
					break;
				}
				}
			}
		}
	}
	else
	{
		// Load level
		for (int r = 0; r < rowsCount; r++)
		{
			for (int c = 0; c < columnsCount; c++)
			{
				unsigned char symbol = levelData1[r][c];

				switch (symbol)
				{
					// Wall
				case '#':
				{
					levelData[r][c] = symbolWall;
					break;
				}

				// Hero
				case '1':
				{
					levelData[r][c] = symbolHero;

					// Catch hero position
					heroRow = r;
					heroColumn = c;
					break;
				}

				// Exit
				case '2':
				{
					levelData[r][c] = symbolExit;
					break;
				}

				// Box
				case 'X':
				{
					levelData[r][c] = symbolBox;
					break;
				}

				// Bomb 
				case 'B':
				{
					levelData[r][c] = symbolBomb;
					break;
				}

				// Other symbols (like spaces)
				default:
				{
					levelData[r][c] = symbol;
					break;
				}
				}
			}
		}
	}
}

void Render()
{
	// Move console cursor to (0,0)
	COORD cursorCoord;
	cursorCoord.X = 0;
	cursorCoord.Y = 0;
	SetConsoleCursorPosition( consoleHandle, cursorCoord );

	printf("\n\n\t");
	for ( int r = 0; r < rowsCount; r++ )
	{
		for ( int c = 0; c < columnsCount; c++ )
		{
			unsigned char symbol = levelData[r][c];
			
			switch ( symbol )
			{
				// Walls - white
				case symbolWall:
				{
					SetConsoleTextAttribute( consoleHandle, 15 );
					break;
				}

				// Hero - green
				case symbolHero:
				{
					SetConsoleTextAttribute( consoleHandle, 10 );
					break;
				}

				// Boxes - yellow
				case symbolBox:
				{
					SetConsoleTextAttribute( consoleHandle, 14);
					break;
				}

				// Bombs - purple
				case symbolBomb:
				{
					SetConsoleTextAttribute( consoleHandle, 5 );
					break;
				}

				// Exit - red
				case symbolExit:
				{
					SetConsoleTextAttribute( consoleHandle, 12 );
					break;
				}
			}

			printf("%c", symbol);
		}
		printf("\n\t");
	}

	// Make a colour game
	SetConsoleTextAttribute( consoleHandle, 7 );
	printf("\n\n\tUse ");
	
	SetConsoleTextAttribute( consoleHandle, 14 );
	printf("AWSD ");
	
	SetConsoleTextAttribute( consoleHandle, 7 );
	printf("to move ");
	
	SetConsoleTextAttribute( consoleHandle, 10 );
	printf("Hero");
	
	SetConsoleTextAttribute( consoleHandle, 7 );
	printf(". Catch the ");

	SetConsoleTextAttribute( consoleHandle, 5 );
	printf("Bombs");

	SetConsoleTextAttribute( consoleHandle, 7 );
	printf(". ");

	SetConsoleTextAttribute( consoleHandle, 14 );
	printf("R ");
	
	SetConsoleTextAttribute( consoleHandle, 7 );
	printf("- restart level.");
}

void MoveHeroTo( int row, int column )
{
	unsigned char destinationCell = levelData[row][column];
	bool canMoveToCell = false;


	switch ( destinationCell )
	{
		// EmptyCell
		case ' ':
		{
			canMoveToCell = true;
			break;
		}

		// ExitCell
		case symbolExit:
		{
			isGameActive = false;
			currentLevel++;
			break;
		}

		// Box
		case symbolBox:
		{	
			// Calculate hero move direction
			int heroDirectionR = row - heroRow;
			int heroDirectionC = column - heroColumn;

			// Check for empty space behind the box
			if ( levelData[row + heroDirectionR][column + heroDirectionC] == ' ' )
			{
				canMoveToCell = true;

				// Remove box symbol from previous position
				levelData[row][column] = ' ';

				// Set box symbol to new position
				levelData[row + heroDirectionR][column + heroDirectionC] = symbolBox;
			}
			
			// Check for bomb behind the box
			else if (levelData[row + heroDirectionR][column + heroDirectionC] == symbolBomb)	
				{
					canMoveToCell = true;

					// Remove box symbol from previous position
					levelData[row][column] = ' ';

					// Set box symbol to new position
					levelData[row + heroDirectionR][column + heroDirectionC] = symbolBox;
				}
			break;
		}

		// Bomb
		case symbolBomb:
		{
			Initialise();
			break;
		}
	}


	if ( canMoveToCell )
	{
		// Remove hero symbol from previous position
		levelData[heroRow][heroColumn] = ' ';

		// Set new hero position
		heroRow = row;
		heroColumn = column;

		// Set hero symbol on new position
		levelData[heroRow][heroColumn] = symbolHero;
	}
}

void Update()
{
	unsigned char inputChar = _getch();
	inputChar = tolower( inputChar );

	switch ( inputChar )
	{
		// Up
		case 'w':
		{
			MoveHeroTo( heroRow - 1, heroColumn );
			break;
		}

		// Down
		case 's':
		{
			MoveHeroTo( heroRow + 1, heroColumn );
			break;
		}

		// Left
		case 'a':
		{
			MoveHeroTo( heroRow, heroColumn - 1 );
			break;
		}

		// Right
		case 'd':
		{
			MoveHeroTo( heroRow, heroColumn + 1 );
			break;
		}

		// Restart level
		case 'r':
		{
			Initialise();
			break;
		}
	}
}

void ShutDown()
{
	system("cls");
	
	printf("\n\n\n\tProgramming - ");
	SetConsoleTextAttribute( consoleHandle, 10 );
	printf("Vadym Sultanov\n");
	
	SetConsoleTextAttribute( consoleHandle, 7 );
	printf("\n\n\tThanks for your support ");

	SetConsoleTextAttribute( consoleHandle, 10 );
	printf("Anastasia Elsukova");

	SetConsoleTextAttribute( consoleHandle, 7 );
	printf("\n\n\n\n\n\n\n\tPress anykey to continue...");
	
	_getch();
}



int main()
{
	SetupSystem();
	Initialise();

	do
	{
		Render();
		Update();
	} 
	while ( isGameActive );

	isGameActive = true;
	Initialise();

	do
	{
		Render();
		Update();
	} while (isGameActive);

	ShutDown();

	// No errors
	return 0;
}



