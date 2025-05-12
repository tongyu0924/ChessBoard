#pragma once
#include "Position.h"
// ROOK // 城堡(橫、直走，不限制) // Castle (入堡) 

class Rook
{
private:
public:
	int color;
	char icon = 'R';
	bool isMove = false;
	Position pos;
	vector<Position> canMovePos;

	Rook() {
		this->pos.x = 0;
		this->pos.y = 0;
	}

	//init
	Rook(int index, int color) {
		if (index == 0) {
			this->pos.x = 0;
		}
		else if (index == 1) {
			this->pos.x = 7;
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
	Rook(Position thePos) {
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
