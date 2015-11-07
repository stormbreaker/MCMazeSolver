/*************************************************************************//**
* @file
*
* @mainpage Program 3 - Maze Solver
*
* @section course_section Course Information
*
* @author Benjamin Kaiser
*
* @date 11-19-2014
*
* @par Professor:
*         Roger Schrader
*
* @par Course:
*         CSC 250 - M002 -  3:00 PM
*
* @par Location:
*         McClaury - 316
*
* @section program_section Program Information
*
* @details This program reads mazes in from a file and using brute force
* permutation, finds the shortest path from beginning to end.  This is done
* primarily using the solve function.  
*
*
* @section compile_section Compiling and Usage
*
* @par Compiling Instructions:
*      VisualStudio 2013 was the IDE used to create this program.  
*
* @par Usage:
  @verbatim
  c:\> prog3.exe maze.maz
  d:\> c:\bin\<prog>.exe maze.maz
  @endverbatim
*
* @section todo_bugs_modification_section Todo, Bugs, and Modifications
*
* @bug None to knowledge
*
* @todo None to knowledge
*
* @par Modifications and Development Timeline:
  @verbatim
  Date          Modification
  ------------  --------------------------------------------------------------
  11-8-14       Started
  11-11-14      Created readInfo, createArray, createMaze, printMaze
  11-16-14      started solve
  11-18-14      wrote solve, storeSolve
  11-19-14		Doxygen
@endverbatim
*
*****************************************************************************/
#include <iostream>
#include <direct.h>
#include <io.h>
#include <string>
#include <fstream>
using namespace std;

void readInfo(ifstream& fin, int& row, int& col, int& startRow, int& startCol);
void createArray(char**& rows, int row, int col, char**& temp);
void createMaze(ifstream& fin, char**& rows, int row, int col);
void printMaze(char**& rows, int row, int col);
void storeSol(ofstream& fout, char** temp, int shortCount, int row, int col, int mazeNum, int startRow, int startCol);
void solve(int& count, char** rows, int currrow, int currcol, bool up, 
	bool right, bool down, bool left, int row, int col, char**& temp, int& shortCount);
void deleteArray(char**& a, int row);
/**************************************************************************//**
* @author Benjamin Kaiser
*
* @par Description: int main
* This is the main computational function.  All function calls are made from
* this function.  
* 
*
* @param[in]     argc - number of arguments
* @param[in]     argv - vector storing program name  and file name to open
* 
* @returns 0 program ran successful.
* @returns 1 the program fail or there was an error.
*
*****************************************************************************/
int main(int argc, char* argv[])
{
	string file;
	int startRow = 0;
	int startCol = 0;
	int row = 0;
	int col = 0;
	int count = 0;
	int mazeNum = 0;
	int shortCount = 0;
	char** rows = nullptr;
	char** temp = nullptr;
	if (argc < 2 || argc > 2)
	{
		cout << "Program requires two parameters." << endl;
		cout << "<program> <file to analyze>";
		return 1;
	}
	
	
	
	file = argv[1];
	ifstream fin;
	ofstream fout;
	fin.open(file);
	if (!(fin.is_open()))
	{
		cout << "Failed to open " << file << endl;
		return 1;
	}
	fout.open("mazesol.maz");
	if (!(fout.is_open()))
	{
		cout << "Failed to open mazesol.maz" << endl;
		return 1;
	}
	readInfo(fin, row, col, startRow, startCol);
	mazeNum = 1;
	while (!fin.eof())
	{
		shortCount = row*col;
		createArray(rows, row, col+5, temp);
		createMaze(fin, rows, row, col);
		solve(count, rows, startRow, startCol, true, true, true, true, row, col, temp, shortCount);
		storeSol(fout, temp, shortCount, row, col, mazeNum, startRow, startCol);
		deleteArray(rows, row);
		deleteArray(temp, row);
		readInfo(fin, row, col, startRow, startCol);
		mazeNum++;
	}

	cout << endl << count << endl;
	cout << endl << shortCount << endl;
	return 0;
};
/**************************************************************************//**
* @author Benjamin Kaiser
*
* @par Description: void readInfo
* This is the function tha reads in the size values of the maze as well as the
* starting location indices.  
*
*
* @param[in]      fin - the file stream to read from the file
* @param[in,out]  row - number of rows
* @param[in,out]  col - number of rows
* @param[in,out]  startRow - starting rows index
* @param[in,out]  startCol - starting column index
*
* @returns nothing - is void function
*
*****************************************************************************/
void readInfo(ifstream& fin, int& row, int& col, int& startRow, int& startCol)
{
	fin >> row;
	fin >> col;
	fin >> startRow;
	fin >> startCol;
	return;
}
/**************************************************************************//**
* @author Benjamin Kaiser
*
* @par Description: createArray
* This function gets the rows and columns and dynamically allocates the memory
* for the arrayes which will be used by the solve function. 
*
* @param[in]      row - number of rows
* @param[in]	  col - number of columns
* @param[in,out]  rows - the main 2D array which will be used to solve stuff
* @param[in,out]  temp - the 2D array storing shortest path solution
*
* @returns nothing - this is a void function
*
*****************************************************************************/
void createArray(char**& rows, int row, int col, char**& temp)
{
	rows = new (nothrow) char*[row];
	temp = new (nothrow) char*[row];
	for (int i = 0; i < row; i++)
	{
		rows[i] = new (nothrow) char[col];
		temp[i] = new (nothrow) char[col];
	}
	return;
}
/**************************************************************************//**
* @author Benjamin Kaiser
*
* @par Description: createMaze
* A detailed description of the function.  It can
* extend to multiple lines
*
* @param[in]      row - number of rows
* @param[in]	  col - number of columns
* @param[in,out]  rows - the main 2D array which will be used to solve stuff
* @param[in,out]  temp - the 2D array storing shortest path solution
*
* @returns nothing - this is a void function
*
*****************************************************************************/
void createMaze(ifstream& fin, char**& rows, int row, int col)
{
	fin.ignore(256, '\n');
	for (int i = 0; i < row; i++)
	{
			fin.getline(rows[i], (col+1));
	}
	return;
}
/**************************************************************************//**
* @author Benjamin Kaiser
*
* @par Description: printMaze
* A function to print the maze or any other 2D array to the screen
*
* @param[in]      row - number of rows
* @param[in]	  col - number of columns
* @param[in,out]  rows - the main 2D array which will be used to solve stuff
*
* @returns nothing - this is a void function
*
*****************************************************************************/
void printMaze(char**& rows, int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			cout << rows[i][j];
		}
		cout << endl;
	}
	return;
}
/**************************************************************************//**
* @author Benjamin Kaiser
*
* @par Description: solve
* This is a recursive function which solves a maze using brute force
* permutations to attempt to go all four directions: up, right, down, left.
*
* @param[in]	  count - a running count of how many steps have been taken
* @param[in,out]  rows - the main 2D array which will be used to solve stuff
* @param[in]	  currrow - integer containing index of current row in the maze
* @param[in]	  currcol - index of current column in the maze
* @param[in]	  up - boolean value to test if up has already been tested
* @param[in]	  right - boolean value to see if right has already been tested
* @param[in]	  left - boolean value to see if left has already been tested
* @param[in]	  down - boolean value to see if down has already been tested
* @param[in]      row - number of rows
* @param[in]	  col - number of columns
* @param[in,out]  temp - the array which will be printed to the screen for
*		          shortest count
* @param[in,out]  shortCount - integer which contains number of steps for 
*				  shortest path
* 
*
* @returns nothing - this is a void function
*
*****************************************************************************/
void solve(int& count, char** rows, int currrow, int currcol, bool up,
	bool right, bool down, bool left, int row, int col, char**& temp, 
	int& shortCount)
{
	if ((rows[currrow][currcol] == 'L') || (rows[currrow][currcol] == '*') || 
		(rows[currrow][currcol] == '-') || (rows[currrow][currcol] == '=') ||
		(rows[currrow][currcol] == 'E'))
	{
		if (rows[currrow][currcol] == 'E')
		{
			if (count < shortCount)
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					temp[i][j] = rows[i][j];
					shortCount = count;
				}
			}
		}
		return;
	}
	if ((rows[currrow][currcol] == 'W'))
	{
		rows[currrow][currcol] = '=';
		count += 2;
	}
	else if (rows[currrow][currcol] == ' ')
	{
		rows[currrow][currcol] = '-';
		count++;
		// need a counter thingy mahbober
	}
	if (up = true)
	{
		solve(count, rows, (currrow - 1), currcol, up,
			right, down, left, row, col, temp, shortCount);
		up = false;
	}
	if (right = true)
	{
		solve(count, rows, currrow, (currcol + 1), up,
			right, down, left, row, col, temp, shortCount);
		right = false;
	}
	if (down = true)
	{
		solve(count, rows, (currrow + 1), currcol, up,
			right, down, left, row, col, temp, shortCount);
		down = false;
	}
	if (left = true)
	{
		solve(count, rows, currrow, (currcol - 1), up,
			right, down, left, row, col, temp, shortCount);
		left = false;
	}

	if (rows[currrow][currcol] == '=')
	{
		rows[currrow][currcol] = 'W';
		count += -2;
	}
	else if (rows[currrow][currcol] == '-')
	{
		rows[currrow][currcol] = ' ';
		count--;
	}
}
/**************************************************************************//**
* @author Benjamin Kaiser
*
* @par Description: storeSol
* A function to print the maze with shortest solution and count to a file
*
* @param[in]      fout - file stream which prints to file
* @param[in]	  temp - array which contains shortest path
* @param[in]	  shortCount - integer which contains number of short path
* @param[in]	  mazeNum - integer which contains the maze number 
*	              from a counter
* @param[in]	  startRow - contains starting row index
* @param[in]	  startCol - contains starting column index
*
* @returns nothing - this is a void function
*
*****************************************************************************/
void storeSol(ofstream& fout, char** temp, int shortCount, int row, int col, int mazeNum, int startRow, int startCol)
{
	fout << "Maze " << mazeNum << " :" << endl;
	fout << "     Size: " << row << "x" << col << endl;
	fout << "     Start loc: " << startRow << " " << startCol << endl;
	fout << "     Shortest path: " << shortCount << " steps" << endl;
	for (int i = 0; i < row; i++)
	{
		fout << "     ";
		for (int j = 0; j < col; j++)
		{
			fout << temp[i][j];
		}
		fout << endl;
	}
}
/**************************************************************************//**
* @author Benjamin Kaiser
*
* @par Description: deleteArray
* Deletes a dynamically allocated 2D array
*
* @param[in]      a - array to be deleted
* @param[in]	  row - the number of rows in the array
*
* @returns nothing - this is a void function
*
*****************************************************************************/
void deleteArray(char**& a, int row)
{
	if (a == nullptr)
	{
		return;
	}
	for (int i = 0; i < row; i++)
	{
		delete [] a[i];
	}
	delete [] a;
}