/***********************************************************************
 * File: Plumber.h
 * Author: 蕭瑋智 黃庭緯 許良宏
 * Create Date: 2024/4/25
 * Editor: 蕭瑋智 黃庭緯 許良宏
 * Update Date: 2024/4/25
 * Description: header file of plumber, represent each plumber block
***********************************************************************/
#pragma once
#include <iostream>
#include <vector>
#include <random>
#include "Position.h"
using namespace std;

class Plumber
{
private:
	vector<vector<vector<vector<char>>>> block //store each type of pipes
	{
		{// T
			{//0 0 UP
				{'#','P','#'},
				{'P','P','P'},
				{'#','#','#'}
			},
			{//0 1 RIGHT
				{'#','P','#'},
				{'#','P','P'},
				{'#','P','#'}
			},
			{//0 2 DOWN
				{'#','#','#'},
				{'P','P','P'},
				{'#','P','#'}
			},
			{//0 3 LEFT
				{'#','P','#'},
				{'P','P','#'},
				{'#','P','#'}
			}
		},
		//second row
		{// +
			{//1 0 
				{'#','P','#'},
				{'P','P','P'},
				{'#','P','#'}
			},
			{//1 1
				{'#','P','#'},
				{'P','P','P'},
				{'#','P','#'}
			},
			{//1 2
				{'#','P','#'},
				{'P','P','P'},
				{'#','P','#'}
			},
			{//1 3
				{'#','P','#'},
				{'P','P','P'},
				{'#','P','#'}
			}
		},

		{// |
			{//2 0 |
				{'#','P','#'},
				{'#','P','#'},
				{'#','P','#'}
			},
			{//2 1 一
				{'#','#','#'},
				{'P','P','P'},
				{'#','#','#'}
			},
			{//2 2 |
				{'#','P','#'},
				{'#','P','#'},
				{'#','P','#'}
			},
			{//2 3 一
				{'#','#','#'},
				{'P','P','P'},
				{'#','#','#'}
			}
		},

		{// L
			{//3 0
				{'#','P','#'},
				{'#','P','P'},
				{'#','#','#'}
			},
			{//3 1
				{'#','#','#'},
				{'#','P','P'},
				{'#','P','#'}
			},
			{//3 2
				{'#','#','#'},
				{'P','P','#'},
				{'#','P','#'}
			},
			{//3 3
				{'#','P','#'},
				{'P','P','#'},
				{'#','#','#'}
			}
		}
	};

	enum Direction { UP, RIGHT, DOWN, LEFT }; //use number to represent the four directions
	enum Type { T_TYPE, INTERSECTION_TYPE, LINE_TYPE, L_TYPE }; //use number to represent the four type of pipe
	Position pos; //pipe's position
	Type type; //represent the type of pipe
	Direction direct; //represent the pipe's direction

public:
	// Intent: Default constructor for Plumber class
	// Pre: None
	// Post: Initializes a Plumber object with default values
	Plumber() {
		type = T_TYPE;
		direct = UP;
		block[type][direct];
	}

	// Intent: Parameterized constructor for Plumber class
	// Pre: intger to repersent pipe type and direction
	// Post: Initializes a Plumber object with specified pipe type and direction
	Plumber(int pipeType, int pipeDir) {
		type = (Type)pipeType;
		direct = (Direction)pipeDir;
		block[type][direct];
	}

	// Intent: Rotate the Plumber object clockwise
	// Pre: None
	// Post: Changes the direction of the Plumber object to the next clockwise direction
	void rotateClockwise() {
		// Increment direction by 1
		this->direct = (Direction)(direct + 1);

		// If direction is greater than 3, reset to 0
		if (this->direct > 3) {
			this->direct = (Direction)0;
		}
}


	// Intent: Rotate the Plumber object anticlockwise
	// Pre: None
	// Post: Changes the direction of the Plumber object to the next anticlockwise direction
	void rotateAnticlock() {
		// Decrement direction by 1
		this->direct = (Direction)(direct - 1);

		// If direction is less than 0, set to 3
		if (this->direct < 0) {
			this->direct = (Direction)3;
		}
	}

	// Intent: Set the position of the Plumber object
	// Pre: intger represesnt position of pipe
	// Post: Sets the position of the Plumber object to the specified coordinates
	void setPos(int x, int y) {
		this->pos.x = x;
		this->pos.y = y;
	}

	// Intent: Randomize the type and direction of the Plumber object
	// Pre: None
	// Post: Sets the type and direction of the Plumber object to random values
	void randomPlumb() {
		int randType = rand() % 4;
		int randDir = rand() % 4;
		direct = (Direction)randDir;
		type = (Type)randType;
	}

	// Intent: Randomize the direction of the Plumber object
	// Pre: None
	// Post: Sets the direction of the Plumber object to a random value
	void randomDir() {
		int randDir = rand() % 4;
		direct = (Direction)randDir;
	}

	// Intent: Get the direction of the Plumber object
	// Pre: None
	// Post: Returns the current direction of the Plumber object
	Direction getDir() {
		return this->direct;
	}

	// Intent: Get the type of the Plumber object
	// Pre: None
	// Post: Returns the current type of the Plumber object
	Type getType() {
		return this->type;
	}

	// Intent: Get the position of the Plumber object
	// Pre: None
	// Post: Returns the current position of the Plumber object
	Position getPos() {
		return this->pos;
	}

	// Intent: Get the 2D vector representing the Plumber object
	// Pre: None
	// Post: Returns the 2D vector representing the current state of the Plumber object
	vector<vector<char>> getPlumber() {
		return block[type][direct];
	}
};
