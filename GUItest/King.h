#pragma once
#include "Position.h"
// KING, // 國王(橫、直、斜走，一格)

class King
{
public:
	int color = 1; //1:黑 -1:白
	char icon = 'K';
	bool isMove = false;
	Position pos;
	vector<Position> canMovePos;

	King() {
		this->pos.x = 0;
		this->pos.y = 0;
	}

	//init
	King(int index, int color) {
		this->pos.x = 4;
		if (color == 1) {
			this->pos.y = 0;
		}
		else if (color == -1) {
			this->pos.y = 7;
		}

		this->color = color;
	}

	//init
	King(Position thePos) {
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