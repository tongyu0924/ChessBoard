#pragma once
#include "Position.h"
// KNIGHT, // 騎士(左右(一格)上下(兩格)OR上下(一格)左右(兩格))


class Knight
{
private:
public:
	int color;
	char icon = 'N';
	Position pos;
	vector<Position> canMovePos;

	pair<int, int> step[8] = {
		{1, 2},
		{2, 1},
		{2, -1},
		{1, -2},
		{-1, -2},
		{-2, -1},
		{-2, 1},
		{-1, 2},
	};

	//init
	Knight() {
		this->pos.x = 0;
		this->pos.y = 0;
	}

	//init
	Knight(int index, int color) {
		if (index == 0) {
			this->pos.x = 1;
		}
		else if (index == 1) {
			this->pos.x = 6;
		}

		if (color == 1) {
			this->pos.y = 0;
		}
		else if (color == -1) {
			this->pos.y = 7;
		}

		this->color = color;
	}

	//init
	Knight(Position thePos) {
		this->pos = thePos;
	}

	//move the chessman
	bool move(Position to)
	{
		for (int i = 0; i < canMovePos.size(); i++) {
			if (to == canMovePos[i]) {
				return true;
			}
		}

		cout << "failed to move\n";
		return false;
	}
};
