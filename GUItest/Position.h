#pragma once
#include <vector>
using namespace std;

//the position
class  Position
{
public:
	int x = 0;
	int y = 0;

	Position() {

	}

	Position(int inputX, int inputY) {
		x = inputX;
		y = inputY;
	}

	/**
	 * Intent: set the position
	 * Pre: x and y
	 * Post: void
	 */
	void setPosition(int inputX, int inputY) {
		x = inputX;
		y = inputY;
	}

	/**
	 * Intent: set the position
	 * Pre: the position
	 * Post: void
	 */
	void setPosition(Position input) {
		x = input.x;
		y = input.y;
	}

	/**
	 * Intent: operator equal =
	 * Pre: the position
	 * Post: void
	 */
	void operator= (const Position& equal) {
		this->x = equal.x;
		this->y = equal.y;
	}

	/**
	 * Intent: operator == (if two position the same)
	 * Pre: the position
	 * Post: bool
	 */
	bool operator== (const Position& compare) {
		if (this->x != compare.x) {
			return false;
		}
		if (this->y != compare.y) {
			return false;
		}
		return true;
	}

	/**
	 * Intent: operator != (if two position not the same)
	 * Pre: the position
	 * Post: bool
	 */
	bool operator!= (const Position& compare) {
		if (this->x == compare.x) {
			return false;
		}
		if (this->y == compare.y) {
			return false;
		}
		return true;
	}
};
