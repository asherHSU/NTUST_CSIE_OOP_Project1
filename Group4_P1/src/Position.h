/***********************************************************************
 * File: Position.h
 * Author: ��޳�� ���x�n �\�}��
 * Create Date: 2024/4/25
 * Editor: ��޳�� ���x�n �\�}��
 * Update Date: 2024/4/25
 * Description: Position class for data saving
***********************************************************************/

#pragma once
class Position
{
public:
	//default constructor
	Position()
	{
		x = 0;
		y = 0;
	}

	//constructor with init position
	Position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int x, y; //position
};