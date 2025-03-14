/***********************************************************************
 * File: main.cpp
 * Author: 蕭瑋智, 黃庭緯, 許良宏
 * Create Date: 2024/4/25
 * Editor: 蕭瑋智, 黃庭緯, 許良宏
 * Update Date: 2024/4/25
 * Description: This is the game of plumber
***********************************************************************/
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <vector>
#include <conio.h>
#include "Plumber.h"
#include "Position.h"
#include <random>
#include <thread>
#include <fstream>
#include <string>
#pragma comment(lib, "winmm.lib")
using namespace std;

// Enum for path direction
enum PathDirection {
	RIGHT,
	DOWN,
	LEFT,
	UP
};

// Array for movement directions
Position moveDirection[4] = {
	Position(1,0), Position(0,1),Position(-1,0),Position(0,-1)
};

// Function prototypes
void generateBoard(); //generate game board
void randomAllPlumber(); //randomize the type 
void randomPlumberDirection(); //randomize the direction
void plumberToBoard(vector<vector<char>>& tempBoard); //convert each plumber to character data type
Position randomPos(); //randomize the position for start/end
void keyUpdate(); //detect the key input
void update(); //update the display
void drawBoard(); //print the game board
void drawInfo(); //display game tutorial and essential info
void findPath(vector<vector<char>>& board, int startRow, int startCol, int endRow, int endCol); //find possible route
void findanswer(vector<vector<char>>& board, int y, int x, int endRow, int endCol, PathDirection direction); //find the answer route
void drawAnswer(); //print the answer in purple highlight
void playSound(); //for sound effect implementation
void save(); //save file
void load(); //load file
bool isVaild(string input); //check input size is vaild

// Global variables
vector<vector<Plumber>> plumber; //store board pipe
vector<vector<char>> gBoard; // Define a 2D vector to store characters for the game board
vector<vector<char>> answerBoard; // Define a 2D vector to store characters for the answer board
Position nowPos; // Define a Position object to store the current position
const double TIMELOG = 0.033; // Define a constant double for the time interval
int height = 0, width = 0;// Define integers to store the height and width of the map
Position start, mEnd;// Define Position objects for the starting position and the end position
bool hasInput = false, isFinish = false, hint = false;// Define boolean variables to track input status and game completion status

int main() {
	mciSendString("open ..\\src\\super-mario-bros-theme-song.wav alias backgroundMusic", 0, 0, 0);// Open and play background music
	mciSendString("play backgroundMusic", 0, 0, 0);

	srand(time(NULL));// Seed the random number generator
	string strHeight, strWidth; //store input

	// Prompt user for map height and width
	cout << "input map height: ";
	getline(cin, strHeight);
	cout << "input map weight: ";
	getline(cin, strWidth);

	do
	{
		if (isVaild(strHeight) == true && isVaild(strWidth) == true) { //check input vaild
			height = strtol(strHeight.c_str(), nullptr, 10);
			width = strtol(strWidth.c_str(), nullptr, 10);

			if (height >= 3 && width >= 3) {
				break;
			}
		}

		cout << "invaild size\n";
		cout << "input map height: ";
		getline(cin, strHeight);
		cout << "input map weight: ";
		getline(cin, strWidth);
	} while (true);

	//delete once randomPos() is in place
	start.x = 0;
	start.y = 1;
	mEnd.x = width * 4 - 2;
	mEnd.y = height * 3 - 2;
	cout << "loading\n";

	// Generate the game board, draw it, and draw game info
	generateBoard();
	drawBoard();
	drawInfo();

	// Variable for game loop
	clock_t startT, endT;
	startT = clock();
	endT = clock();

	do //main game loop
	{
		// Compute the time lap
		double timeFrame = (double)(endT - startT) / CLOCKS_PER_SEC;

		// Execute the game loop
		if (timeFrame >= TIMELOG) {
			update();

			if (gBoard[mEnd.y][mEnd.x] == 'O') { //is water arrive end position
				break;
			}

			char playResult[128];
			mciSendString("status backgroundMusic mode", playResult, 128, 0); //get bgm play status

			if (strcmp("playing", playResult) != 0) { //check if bgm is end, repeat
				mciSendString("close backgroundMusic", NULL, NULL, NULL);
				mciSendString("open ..\\src\\super-mario-bros-theme-song.wav alias backgroundMusic", 0, 0, 0);// Open and play background music
				mciSendString("play backgroundMusic", NULL, NULL, NULL);
			}

			startT = clock();
		}

		// Update the key
		keyUpdate();
		endT = clock();
	} while (isFinish == false);

	// Display stage clear message and wait for user input
	cout << "Stage Clear!\n";
	system("pause");
	return 0;
}

// Intent: check user input size is all digit
// Pre: user input
// Post: return true when input vaild
bool isVaild(string input) {
	for (size_t i = 0; i < input.length(); i++)
	{
		if (isdigit(input[i]) == false) {
			return false;
		}
	}

	return true;
}

// Intent: Generate the game board
// Pre: nothing
// Post: now the board is randomized and ready to test
void generateBoard() {
	//initialize
	vector<vector<char>> board(height * 3, vector<char>(width * 4, ' '));
	int randDirectTimes = 0;
	randomAllPlumber();

	do
	{
		//if impossible to solve, re-generate
		if (randDirectTimes >= 500) {
			randomAllPlumber();
			randDirectTimes = 0;
		}

		randDirectTimes++;
		vector<vector<char>> tempBoard(height * 3, vector<char>(width * 4, ' '));
		randomPlumberDirection();//random plumber direction
		plumberToBoard(tempBoard);
		board = tempBoard;
		findPath(tempBoard, start.y, start.x, height * 3 - 1, width * 4 - 1); //travese to find answer route

		if (tempBoard[mEnd.y][mEnd.x] == 'O') {//end is reached, Feasible solution
			break;
		}
		else {
			tempBoard.clear();
		}

	} while (true);

	answerBoard = board;
	findanswer(answerBoard, start.y, start.x, height * 3 - 2, width * 4 - 2, RIGHT); //set an extra answer board for possible route detection

	do {
		randomPlumberDirection();//random plumber direction
		drawBoard(); //update
	} while (gBoard[mEnd.y][mEnd.x] == 'O');
}

// Intent: Randomly decide the type of tubes
// Pre: all same type of tubes
// Post: now increase RNG factor to make the board more interesting and traversable
void randomAllPlumber() {
	plumber.clear(); //reset

	for (int i = 0; i < height; i++) {
		vector<Plumber> tempVecPlu; //new row pipe

		for (int j = 0; j < width; j++) { //random each pipe
			Plumber tempPlu;
			tempPlu.randomPlumb();
			tempVecPlu.push_back(tempPlu);
		}

		plumber.push_back(tempVecPlu);
	}
}

// Intent: Randomly turn the tubes
// Pre: randomly generated tubes with the same direction
// Post: now increase RNG factor to make the board less straight forward
void randomPlumberDirection() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			plumber[i][j].randomDir(); //random pipe direction
		}
	}
}

// Intent: Convert the 3x3 plumber into char data type
// Pre: the vector stores plumber as a class member
// Post: newly assigned tempBoard now stores each characters individually
void plumberToBoard(vector<vector<char>>& tempBoard) {
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			vector<vector<char>> tempBlock = plumber[i][j].getPlumber(); //get pipe char block

			for (size_t k = 0; k < 3; k++) //fill pipe char block to tempBoard
			{
				for (size_t f = 0; f < 3; f++)
				{
					//assignment
					tempBoard[i * 3 + k][j * 4 + f] = tempBlock[k][f];
				}
			}
		}
	}
}

// Intent: Display both the main plumber board and reference answer board 
// Pre: nothing
// Post: main plumber board and reference answer board will be displayed and updated
void drawBoard() {
	system("CLS");// Clear the console screen
	vector<vector<char>> board(height * 3, vector<char>(width * 4, ' ')); //initialize
	plumberToBoard(board); //convert the 3x3 plumber into char data type
	findPath(board, start.y, start.x, height * 3 - 1, width * 4 - 1); //traverse
	gBoard = board;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (size_t i = 0; i < height * 3; i++) //output board
	{
		for (size_t j = 0; j < width * 4; j++)
		{
			if ((board[i][j] == 'O') && //element is O and is selected, print with blue and red background
				(nowPos.y * 3 == i || nowPos.y * 3 + 1 == i || nowPos.y * 3 + 2 == i) &&
				(nowPos.x * 4 == j || nowPos.x * 4 + 1 == j || nowPos.x * 4 + 2 == j))
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_RED); //make the color of console into blue words and red background
			}
			else if ((i == start.y && j == start.x) || (i == mEnd.y && j == mEnd.x))//element is start,end position, print with yellow
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); //make the color of console into yello words
			}
			else if (board[i][j] == 'O')//element is O, print with blue
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE); //make the color of console into blue words
			}
			else if ((nowPos.y * 3 == i || nowPos.y * 3 + 1 == i || nowPos.y * 3 + 2 == i) &&
				(nowPos.x * 4 == j || nowPos.x * 4 + 1 == j || nowPos.x * 4 + 2 == j)) //element is selected, print with red background
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | BACKGROUND_RED); //make the color of console into green words and red background
			}
			else {
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); //make the color of console into white words
			}

			printf("%c", board[i][j]);
		}

		printf("\n");
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); //make the color of console into white words
}

// Intent: Show your current locations and input tutorial
// Pre: No indication
// Post: Needed info to play the game is now displayed
void drawInfo() {
	cout << "nowpos x:" << nowPos.x << " y:" << nowPos.y << "\n";
	cout << "(ESC) Exit (1) Save (2) Load." << "\n";
	cout << "W,A,S,D to select target, Q and E to rotate selected block." << "\n";
	cout << "You need an answer? Press K." << "\n";
}

// Intent: Detect input and update accordingly
// Pre: nothing changed
// Post: after input the game and relevant data will be update via functions
void keyUpdate() {
	char input = _getch(); //get input char without enter
	hasInput = false;

	switch (input) {//w,a,s,d are for selecting targets
	case 'w'://move up
		if (nowPos.y == 0) { //prevent select out of range
			std::cout << "Out of range" << std::endl;
			break;
		}

		nowPos.y--;
		hasInput = true;
		break;
	case 's'://move down
		if (nowPos.y == height - 1) {//prevent select out of range
			std::cout << "Out of range" << std::endl;
			break;
		}

		nowPos.y++;
		hasInput = true;
		break;
	case 'a'://move left
		if (nowPos.x == 0) {//prevent select out of range
			std::cout << "Out of range" << std::endl;
			break;
		}

		nowPos.x--;
		hasInput = true;
		break;
	case 'd'://move right
		if (nowPos.x == width - 1) {//prevent select out of range
			std::cout << "Out of range" << std::endl;
			break;
		}

		nowPos.x++;
		hasInput = true;
		break;
	case 'q'://rotate anticlockwise
		hasInput = true;
		plumber[nowPos.y][nowPos.x].rotateAnticlock(); //rotate
		playSound();
		break;
	case 'e'://rotate clockwise
		hasInput = true;
		plumber[nowPos.y][nowPos.x].rotateClockwise();//rotate
		playSound();
		break;
	case 'k'://print answer board as a hint
		hasInput = true;
		hint = !hint;
		break;
	case '1'://save
		hasInput = true;
		save();
		break;
	case '2'://load
		hasInput = true;
		load();
		break;
	case 27://esc is to exit the program
		isFinish = true;
		hasInput = true;
		break;
	default:
		hasInput = false;
		break;
	}
}

// Intent: After key input, we update the board and display it
// Pre: the previous iteration of the game
// Post: this function will update and display the newly updated data
void update() {
	//call both functions to update and display to the console
	drawBoard();
	drawInfo();

	if (hint == true) {
		printf("\nanswer:\n");
		drawAnswer();
		return;
	}

	if (hasInput == false) {
		cout << "Invaild input\n";
		return;
	}
}

// Intent: This will recursively find the route to finish
// Pre: the original randomly genetated plumber board
// Post: this function will navigate through the route and mark 'O' if the route is accessable
void findPath(vector<vector<char>>& board, int startRow, int startCol, int endRow, int endCol) {
	//if out of range, return
	if (startRow < 0 || startCol < 0 || startRow > endRow || startCol > endCol) {
		return;
	}

	//if a wall is hit, return
	if (board[startRow][startCol] == '#' || board[startRow][startCol] == 'O') {
		return;
	}

	//if the current block is blank space that needs to be skipped
	if (board[startRow][startCol] == ' ') {
		findPath(board, startRow, startCol + 1, endRow, endCol);
		findPath(board, startRow, startCol - 1, endRow, endCol);
	}
	else {
		board[startRow][startCol] = 'O';

		//recursively search all other directions
		findPath(board, startRow + 1, startCol, endRow, endCol);
		findPath(board, startRow - 1, startCol, endRow, endCol);
		findPath(board, startRow, startCol + 1, endRow, endCol);
		findPath(board, startRow, startCol - 1, endRow, endCol);
	}

	return;
}

// Intent: This will recursively find the route to finish (for the answer board)
// Pre: the original randomly genetated plumber board
// Post: this function will navigate through the route and mark 'A' if the route is accessable
void findanswer(vector<vector<char>>& board, int y, int x, int endRow, int endCol, PathDirection direction) {
	//marked A, route has been accessed already
	if (board[endRow][endCol] == 'A') {
		return;
	}

	//if out of valid range, return
	if (y < 0 || x < 0 || y >(height * 3 - 1) || x >(width * 4 - 1)) {
		return;
	}

	//ignore the spaces in between line
	if (board[y][x] == ' ' && (direction == UP || direction == DOWN)) {
		return;
	}

	//if wall is meet, return
	if (board[y][x] == '#') {
		return;
	}

	//turn and search all other directions
	PathDirection turnRight = PathDirection((direction + 1) % 4);
	PathDirection turnLeft = PathDirection((direction + 3) % 4);
	PathDirection oppsite = PathDirection((direction + 2) % 4);

	//if we hit a dead end, we go back 
	if (board[y][x] == 'A' && board[y - moveDirection[direction].y][x - moveDirection[direction].x] == 'A') {
		board[y - moveDirection[direction].y][x - moveDirection[direction].x] = 'P';
	}
	else if (board[y][x] == 'A' && board[y - moveDirection[direction].y][x - moveDirection[direction].x] == ' ' && board[y - moveDirection[direction].y * 2][x - moveDirection[direction].x * 2] == 'A') {
		board[y - moveDirection[direction].y * 2][x - moveDirection[direction].x * 2] = 'P';
	}

	//route has not been accessed, go back
	if (board[y][x] == 'P') {
		board[y][x] = 'A';
	}

	//update the traversed route
	if (height <= 8 && width <= 8) {
		system("CLS");
		drawAnswer();
		Sleep(150);
	}

	//recursively find all 4 directions around the current block
	findanswer(board, y + moveDirection[turnRight].y, x + moveDirection[turnRight].x, endRow, endCol, turnRight);
	findanswer(board, y + moveDirection[direction].y, x + moveDirection[direction].x, endRow, endCol, direction);
	findanswer(board, y + moveDirection[turnLeft].y, x + moveDirection[turnLeft].x, endRow, endCol, turnLeft);
	findanswer(board, y + moveDirection[oppsite].y, x + moveDirection[oppsite].x, endRow, endCol, oppsite);
	return;
}

// Intent: This will print the answer route with purple 'A's 
// Pre: a possible route to finish
// Post: now the route will be highlighted in purple for reference
void drawAnswer() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string tempLine; //register for each line

	for (size_t i = 0; i < answerBoard.size(); i++)
	{
		tempLine = ""; //reset

		for (size_t j = 0; j < answerBoard[i].size(); j++)
		{
			if (answerBoard[i][j] == 'A') { //if need print A, output tempLine, change console color, print A
				printf("%s", tempLine.c_str());
				tempLine = "";
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				printf("A");
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); //make the color of console into white words
			}
			else {
				tempLine += answerBoard[i][j]; //add current char to tempLine
			}
		}

		printf("%s\n", tempLine.c_str()); //print each line
	}
}

// Intent: This will determine random start and end
// Pre: start is from upper left and end is in bottom right
// Post: both will now be randomized around the outer ring
Position randomPos() {
	Position newRandomPos;
	int determine = rand() % 4;

	if (determine == 0) {
		//on top
		newRandomPos.y = 0;
		newRandomPos.x = rand() % (width - 1);
	}
	else if (determine == 1) {
		//right side
		newRandomPos.y = rand() % (height - 1);
		newRandomPos.x = width - 1;
	}
	else if (determine == 2) {
		//bottom
		newRandomPos.y = height - 1;
		newRandomPos.x = rand() % (width - 1);
	}
	else if (determine == 3) {
		//left side
		newRandomPos.y = rand() % (height - 1);
		newRandomPos.x = 0;
	}

	return newRandomPos;
}

// Intent: Saving the game saves
// Pre: the current running game
// Post: the game will now be saved into save.txt
void save() {
	//save file name: save.txt
	std::ofstream oStream("save.txt");

	//height x width
	oStream << height << " " << width << " " << start.x << " " << start.y << " " << mEnd.x << " " << mEnd.y << " "
		<< nowPos.x << " " << nowPos.y << "\n";

	//the type and direction of each individual tube
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int type = plumber[i][j].getType();
			int dir = plumber[i][j].getDir();
			oStream << type << " " << dir << " ";
		}

		oStream << "\n";
	}

	//saving the answer route 
	for (int i = 0; i < answerBoard.size(); i++) {
		for (int j = 0; j < answerBoard[i].size(); j++) {
			oStream << answerBoard[i][j];
		}

		oStream << "\n";
	}

	oStream.close();
	cout << "save success\n";
	return;
}

// Intent: Loading the game saves
// Pre: previous version of the game
// Post: the game will now be refreshed and updated according to the loading file 
void load() {
	std::ifstream iStream("save.txt");

	if (iStream.fail()) {
		return;
	}

	//read parameters
	int newH, newW;
	Position newStart, newEnd, currentPos;
	iStream >> newH >> newW >> newStart.x >> newStart.y >> newEnd.x >> newEnd.y >> currentPos.x >> currentPos.y;

	//assign the global variables to the newly read variables
	width = newW;
	height = newH;
	start = newStart;
	mEnd = newEnd;
	nowPos = currentPos;

	plumber.clear();//reset plumber
	answerBoard.clear();//reset answer board

	//reading the direction and the type of tubes
	vector<vector<Plumber>> newPlumber;

	for (int i = 0; i < height; i++) {
		//read by row
		vector<Plumber> tempPlumber;

		for (int j = 0; j < width; j++) {
			int type, dir;
			iStream >> type >> dir;
			//push back and save
			Plumber temp(type, dir);
			tempPlumber.push_back(temp);
		}

		newPlumber.push_back(tempPlumber);//save by row
	}

	plumber = newPlumber;//assign the loaded saves to plumber vector

	//read the answer route
	vector<vector<char>> newAnswer;
	string tempLine = "";

	while (getline(iStream, tempLine)) {
		vector<char> tempAns;

		for (int j = 0; j < tempLine.length(); j++) {
			tempAns.push_back(tempLine[j]);
		}

		newAnswer.push_back(tempAns);
	}

	answerBoard = newAnswer;
	iStream.close();
	return;
}

// Intent: this is for the audio effect
// Pre: movement won't have any audio effect
// Post: background, rotating a block will now have audio effect implemented
void playSound() {
	mciSendString("close effectSound", NULL, NULL, NULL); //close effectSound
	mciSendString("open ..\\src\\sound-effect.wav alias effectSound", NULL, NULL, NULL); //open new effectSound
	mciSendString("pause backgroundMusic", NULL, NULL, NULL);
	mciSendString("play effectSound", NULL, NULL, NULL);
	mciSendString("resume backgroundMusic", NULL, NULL, NULL);
}