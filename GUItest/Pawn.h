#pragma once
#include "Position.h"
#include <vector>

// PAWN // §L(¦Y¹L¸ô§L) //Promoting (¤É¶¥)

class Pawn
{
private:
public:
	char icon = 'P';
	int color = 1; //b.w
	int moveTimes = 0; //to judge if first move
	bool enpassant = false; //move two units
	Position pos;
	vector<Position> canMovePos;

	Pawn() {
		this->pos.x = 0;
		this->pos.y = 0;
	}

	//init
	Pawn(int index, int color) {
		this->pos.x = index;

		if (color == 1) {
			this->pos.y = 1;
		}
		else if (color == -1) {
			this->pos.y = 6;
		}

		this->color = color;
	}

	//init
	Pawn(Position thePos) {
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
