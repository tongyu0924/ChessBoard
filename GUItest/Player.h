#pragma once
#include "Position.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h" //城堡
#include "Bishop.h" //主教
#include "Knight.h" //騎士
#include "Pawn.h" //兵
#include <vector>

//manage chess
class Player
{
private:
public:
	int color = 1; // 1:黑 -1:白
	vector<King> king;
	vector<Queen> queen;
	vector<Rook> rook;
	vector<Bishop> bishop;
	vector<Knight> knight;
	vector<Pawn> pawn;
	char** playerBoard;
	bool PMove = false; //if pawn move success
	bool haveEat = false; //if have eat chess


	/**
	 * Intent: init chess
	 * Pre: color(who first)
	 */
	Player(int turn)
	{
		//init playboard
		color = turn;
		playerBoard = new char* [8];
		for (int i = 0; i < 8; i++)
		{
			playerBoard[i] = new char[8];
			for (int j = 0; j < 8; j++)
			{
				playerBoard[i][j] = ' ';
			}
		}

		//init pawn
		for (int i = 0; i < 8; i++) {
			Pawn temp(i, turn);
			playerBoard[temp.pos.y][temp.pos.x] = temp.icon;
			pawn.push_back(temp);
		}

		//init R
		for (int i = 0; i < 2; i++) {
			Rook temp(i, turn);
			playerBoard[temp.pos.y][temp.pos.x] = temp.icon;
			rook.push_back(temp);
		}

		//init N
		for (int i = 0; i < 2; i++) {
			Knight temp(i, turn);
			playerBoard[temp.pos.y][temp.pos.x] = temp.icon;
			knight.push_back(temp);
		}

		//init B
		for (int i = 0; i < 2; i++) {
			Bishop temp(i, turn);
			playerBoard[temp.pos.y][temp.pos.x] = temp.icon;
			bishop.push_back(temp);
		}

		//init Q
		Queen tempQ(0, turn);
		playerBoard[tempQ.pos.y][tempQ.pos.x] = tempQ.icon;
		queen.push_back(tempQ);

		//init K
		King tempK(0, turn);
		playerBoard[tempK.pos.y][tempK.pos.x] = tempK.icon;
		king.push_back(tempK);
	}

	/**
	 * Intent: find the index of chess
	 * Pre: the chess and its position (from)
	 * Post: index
	 */
	int findTheChess(char chess, Position fromPos)
	{
		int index = 0;
		//find the chess
		switch (chess) {
		case 'K':
			index = 0;
			break;
		case 'Q':
			for (int i = 0; i < queen.size(); i++) {
				if (queen[i].pos == fromPos) {
					index = i;
					break;
				}
			}
			break;
		case 'R':
			for (int i = 0; i < rook.size(); i++) {
				if (rook[i].pos == fromPos) {
					index = i;
					break;
				}
			}
			break;
		case 'N':
			for (int i = 0; i < knight.size(); i++) {
				if (knight[i].pos == fromPos) {
					index = i;
					break;
				}
			}
			break;
		case 'B':
			for (int i = 0; i < bishop.size(); i++) {
				if (bishop[i].pos == fromPos) {
					index = i;
					break;
				}
			}
			break;
		case 'P':
			for (int i = 0; i < pawn.size(); i++) {
				if (pawn[i].pos == fromPos) {
					index = i;
					break;
				}
			}
			break;
		}

		return index;
	}

	/**
	 * Intent: if the chess move success
	 * Pre: the chess and its position (from and to)
	 * Post: boolean
	 */
	bool move(char chess, Position fromPos, Position toPos)
	{
		int index = findTheChess(chess, fromPos);

		switch (chess) {
		case 'K':
			if (king[index].move(toPos)) {
				int dx = toPos.x - king[index].pos.x;
				int dy = toPos.y - king[index].pos.y;
				king[index].pos.setPosition(toPos);

				//castle
				if (dy == 0) {
					int index;

					if (dx == 2) {
						for (index = 0; index < rook.size(); index++) {
							if (rook[index].pos.x == 7) {
								break;
							}
						}
						rook[index].pos.setPosition(toPos.x - 1, toPos.y);
						rook[index].isMove = true;
					}
					else if (dx == -2) {
						for (index = 0; index < rook.size(); index++) {
							if (rook[index].pos.x == 0) {
								break;
							}
						}
						rook[index].pos.setPosition(toPos.x + 1, toPos.y);
						rook[index].isMove = true;
					}
				}
				king[0].isMove = true;
				return true;
			}
			break;
		case 'Q':
			if (queen[index].move(toPos)) {
				queen[index].pos.setPosition(toPos);
				return true;
			}
			break;
		case 'R':
			if (rook[index].move(toPos)) {
				rook[index].pos.setPosition(toPos);
				rook[index].isMove = true;
				return true;
			}
			break;
		case 'N':
			if (knight[index].move(toPos)) {
				knight[index].pos.setPosition(toPos);
				return true;
			}
			break;
		case 'B':
			if (bishop[index].move(toPos)) {
				bishop[index].pos.setPosition(toPos);
				return true;
			}
			break;
		case 'P':
			if (pawn[index].move(toPos)) {
				int dy = toPos.y - fromPos.y;
				pawn[index].pos.setPosition(toPos);
				pawn[index].moveTimes++;
				this->PMove = true;

				if (abs(dy) == 2) {
					pawn[index].enpassant = true;
				}
				return true;
			}
			break;
		}

		return false;
	}

	/**
	 * Intent: update can move pos
	 * Pre: the opponent player
	 * Post: void
	 */
	void updateCanMovePos(Player& opponent)
	{
		Position temp;
		// K
		king[0].canMovePos.clear();
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				temp.x = king[0].pos.x + i;
				temp.y = king[0].pos.y + j;
				if (temp.x <= 7 && temp.x >= 0 && temp.y <= 7 && temp.y >= 0) {
					if (playerBoard[temp.y][temp.x] == ' ') {
						king[0].canMovePos.push_back(temp);
					}
				}
			}
		}

		//castle
		if (!king[0].isMove) {
			temp.y = king[0].pos.y;
			int index;
			for (index = 0; index < rook.size(); index++) {
				if (rook[index].pos.x == 7) {
					break;
				}
			}
			if (!rook[index].isMove) {
				bool canCastle = true;
				temp.x = king[0].pos.x + 2;
				for (int i = king[0].pos.x + 1; i < 7; i++) {
					if (this->playerBoard[temp.y][i] != ' ' || opponent.playerBoard[temp.y][i] != ' ') {
						canCastle = false;
						break;
					}
				}
				if (canCastle) king[0].canMovePos.push_back(temp);
			}

			for (index = 0; index < rook.size(); index++) {
				if (rook[index].pos.x == 0) {
					break;
				}
			}
			if (!rook[index].isMove) {
				bool canCastle = true;
				temp.x = king[0].pos.x - 2;
				for (int i = king[0].pos.x - 1; i >= 1; i--) {
					if (this->playerBoard[temp.y][i] != ' ' || opponent.playerBoard[temp.y][i] != ' ') {
						canCastle = false;
						break;
					}
				}
				if (canCastle) king[0].canMovePos.push_back(temp);
			}
		}

		//Q
		for (int index = 0; index < queen.size(); index++) {
			queen[index].canMovePos.clear();
			//to left up
			for (int i = 1; i < 8; i++) {
				temp.x = queen[index].pos.x - i;
				temp.y = queen[index].pos.y - i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					queen[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//to right down
			for (int i = 1; i < 8; i++) {
				temp.x = queen[index].pos.x + i;
				temp.y = queen[index].pos.y + i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					queen[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//to left down
			for (int i = 1; i < 8; i++) {
				temp.x = queen[index].pos.x - i;
				temp.y = queen[index].pos.y + i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					queen[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//to right up
			for (int i = 1; i < 8; i++) {
				temp.x = queen[index].pos.x + i;
				temp.y = queen[index].pos.y - i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					queen[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//go up
			for (int i = 1; i < 8; i++) {
				temp.x = queen[index].pos.x;
				temp.y = queen[index].pos.y - i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					queen[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//go down
			for (int i = 1; i < 8; i++) {
				temp.x = queen[index].pos.x;
				temp.y = queen[index].pos.y + i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					queen[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//go left
			for (int i = 1; i < 8; i++) {
				temp.x = queen[index].pos.x - i;
				temp.y = queen[index].pos.y;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					queen[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//go right
			for (int i = 1; i < 8; i++) {
				temp.x = queen[index].pos.x + i;
				temp.y = queen[index].pos.y;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					if (playerBoard[temp.y][temp.x] == ' ' && temp.x <= 7 && temp.x >= 0 && temp.y <= 7 && temp.y >= 0) {
						queen[index].canMovePos.push_back(temp);
						if (opponent.playerBoard[temp.y][temp.x] != ' ') {
							break;
						}
					}
					else {
						break;
					}
				}
			}
		}

		//R
		for (int index = 0; index < rook.size(); index++) {
			rook[index].canMovePos.clear();
			//go up
			for (int i = 1; i < 8; i++) {
				temp.x = rook[index].pos.x;
				temp.y = rook[index].pos.y - i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					rook[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//go down
			for (int i = 1; i < 8; i++) {
				temp.x = rook[index].pos.x;
				temp.y = rook[index].pos.y + i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					rook[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//go left
			for (int i = 1; i < 8; i++) {
				temp.x = rook[index].pos.x - i;
				temp.y = rook[index].pos.y;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					rook[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//go right
			for (int i = 1; i < 8; i++) {
				temp.x = rook[index].pos.x + i;
				temp.y = rook[index].pos.y;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					rook[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}
		}

		//N
		for (int index = 0; index < knight.size(); index++) {
			knight[index].canMovePos.clear();
			for (int i = 0; i < 8; i++) {
				temp.x = knight[index].pos.x + knight[index].step[i].first;
				temp.y = knight[index].pos.y + knight[index].step[i].second;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) continue;
				if (playerBoard[temp.y][temp.x] == ' ') {
					knight[index].canMovePos.push_back(temp);
				}
			}
		}

		//B
		for (int index = 0; index < bishop.size(); index++) {
			bishop[index].canMovePos.clear();
			//to left up
			for (int i = 1; i < 8; i++) {
				temp.x = bishop[index].pos.x - i;
				temp.y = bishop[index].pos.y - i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					bishop[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//to right down
			for (int i = 1; i < 8; i++) {
				temp.x = bishop[index].pos.x + i;
				temp.y = bishop[index].pos.y + i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					bishop[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//to left down
			for (int i = 1; i < 8; i++) {
				temp.x = bishop[index].pos.x - i;
				temp.y = bishop[index].pos.y + i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					bishop[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}

			//to right up
			for (int i = 1; i < 8; i++) {
				temp.x = bishop[index].pos.x + i;
				temp.y = bishop[index].pos.y - i;
				if (temp.x > 7 || temp.x < 0 || temp.y > 7 || temp.y < 0) break;
				if (playerBoard[temp.y][temp.x] == ' ') {
					bishop[index].canMovePos.push_back(temp);
					if (opponent.playerBoard[temp.y][temp.x] != ' ') {
						break;
					}
				}
				else {
					break;
				}
			}
		}

		//P
		for (int index = 0; index < pawn.size(); index++) {
			pawn[index].canMovePos.clear();
			temp.x = pawn[index].pos.x;
			temp.y = color == 1 ? pawn[index].pos.y + 1 : pawn[index].pos.y - 1;
			if (temp.y > 7 || temp.y < 0) continue;
			if (playerBoard[temp.y][temp.x] == ' ' && opponent.playerBoard[temp.y][temp.x] == ' ') {
				pawn[index].canMovePos.push_back(temp);

				//first time can move two units
				if (pawn[index].moveTimes == 0) {
					temp.y = color == 1 ? pawn[index].pos.y + 2 : pawn[index].pos.y - 2;
					if (temp.y <= 7 && temp.y >= 0) {
						if (playerBoard[temp.y][temp.x] == ' ' && opponent.playerBoard[temp.y][temp.x] == ' ') {
							pawn[index].canMovePos.push_back(temp);
						}
					}
				}
			}

			//eat chessman & Enpassant
			temp.y = color == 1 ? pawn[index].pos.y + 1 : pawn[index].pos.y - 1;
			//right
			temp.x = pawn[index].pos.x + 1;
			if (temp.x <= 7) {
				int opponentP = 0;
				for (opponentP = 0; opponentP < opponent.pawn.size(); opponentP++) {
					if (opponent.pawn[opponentP].pos.x == temp.x) {
						break;
					}
				}
				if (opponent.playerBoard[pawn[index].pos.y][temp.x] == 'P' && opponent.pawn[opponentP].enpassant) {
					pawn[index].canMovePos.push_back(temp);
				}
				else if (opponent.playerBoard[temp.y][temp.x] != ' ') {
					pawn[index].canMovePos.push_back(temp);
				}
			}

			//left
			temp.x = pawn[index].pos.x - 1;
			if (temp.x >= 0) {
				int opponentP = 0;
				for (opponentP = 0; opponentP < opponent.pawn.size(); opponentP++) {
					if (opponent.pawn[opponentP].pos.x == temp.x) {
						break;
					}
				}
				if (opponent.playerBoard[pawn[index].pos.y][temp.x] == 'P' && opponent.pawn[opponentP].enpassant) {
					pawn[index].canMovePos.push_back(temp);
				}
				else if (opponent.playerBoard[temp.y][temp.x] != ' ') {
					pawn[index].canMovePos.push_back(temp);
				}
			}
		}
	}

	/**
	 * Intent: return can move pos (print canMovePos playerboard)
	 * Pre: the chess and its position (from)
	 * Post: position vector
	 */
	vector<Position> returnCanMovePos(char chess, const Position& fromPos)
	{
		int index = findTheChess(chess, fromPos);
		vector<Position> answer;

		switch (chess) {
		case 'K':
			answer = king[index].canMovePos;
			break;
		case 'Q':
			answer = queen[index].canMovePos;
			break;
		case 'R':
			answer = rook[index].canMovePos;
			break;
		case 'N':
			answer = knight[index].canMovePos;
			break;
		case 'B':
			answer = bishop[index].canMovePos;
			break;
		case 'P':
			answer = pawn[index].canMovePos;
			break;
		}

		return answer;
	}

	/**
	 * Intent: pawn promoting
	 * Pre: the chess position (to)
	 * Post: void
	 */
	void promoting(const Position& toPos)
	{
		char symbol;
		cout << "choose Q or B or N or R" << '\n';
		while (cin >> symbol) {
			if (symbol == 'Q') {
				Queen temp(0, color);
				temp.pos.setPosition(toPos);
				queen.push_back(temp);
				break;
			}
			else if (symbol == 'B') {
				Bishop temp(0, color);
				temp.pos.setPosition(toPos);
				bishop.push_back(temp);
				break;
			}
			else if (symbol == 'N') {
				Knight temp(0, color);
				temp.pos.setPosition(toPos);
				knight.push_back(temp);
				break;
			}
			else if (symbol == 'R') {
				Rook temp(0, color);
				temp.pos.setPosition(toPos);
				temp.isMove = true;
				rook.push_back(temp);
				break;
			}

			else cout << "input error. Please choose again : ";
		}

		//delete the promotion pawn
		for (int i = 0; i < pawn.size(); i++) {
			if (pawn[i].pos.x == toPos.x && pawn[i].pos.y == toPos.y) {
				pawn.erase(pawn.begin() + i);
				break;
			}
		}
		system("cls");
		cout << "you choose to promotion to " << symbol << '\n';
	}

	/**
	 * Intent: eat chessman
	 * Pre: the chess  position (to)
	 * Post: void
	 */
	void beEat(Position chessPos)
	{
		this->haveEat = true;
		char theChess = playerBoard[chessPos.y][chessPos.x];
		//find the chess
		int i;
		switch (theChess) {
		case 'K':
			//lose？！！！
			cout << "you lose" << endl;
			break;
		case 'Q':
			for (i = 0; i < queen.size(); i++) {
				if (queen[i].pos == chessPos) {
					break;
				}
			}
			queen.erase(queen.begin() + i);
			break;
		case 'R':
			for (i = 0; i < rook.size(); i++) {
				if (rook[i].pos == chessPos) {
					break;
				}
			}
			rook.erase(rook.begin() + i);
			break;
		case 'N':
			for (i = 0; i < knight.size(); i++) {
				if (knight[i].pos == chessPos) {
					break;
				}
			}
			knight.erase(knight.begin() + i);
			break;
		case 'B':
			for (i = 0; i < bishop.size(); i++) {
				if (bishop[i].pos == chessPos) {
					break;
				}
			}
			bishop.erase(bishop.begin() + i);
			break;
		case 'P':
			for (i = 0; i < pawn.size(); i++) {
				if (pawn[i].pos == chessPos) {
					break;
				}
			}
			pawn.erase(pawn.begin() + i);
			break;
		}
	}

	/**
	 * Intent: update playboard
	 * Pre: void
	 * Post: void
	 */
	void update(void) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				playerBoard[i][j] = ' ';
			}
		}

		//update king
		playerBoard[king[0].pos.y][king[0].pos.x] = king[0].icon;

		//update queen
		for (int i = 0; i < queen.size(); i++) {
			playerBoard[queen[i].pos.y][queen[i].pos.x] = queen[i].icon;
		}

		//update bishop
		for (int i = 0; i < bishop.size(); i++) {
			playerBoard[bishop[i].pos.y][bishop[i].pos.x] = bishop[i].icon;
		}

		//update knight
		for (int i = 0; i < knight.size(); i++) {
			playerBoard[knight[i].pos.y][knight[i].pos.x] = knight[i].icon;
		}

		//update pawn
		for (int i = 0; i < pawn.size(); i++) {
			playerBoard[pawn[i].pos.y][pawn[i].pos.x] = pawn[i].icon;
		}

		//update rook
		for (int i = 0; i < rook.size(); i++) {
			playerBoard[rook[i].pos.y][rook[i].pos.x] = rook[i].icon;
		}

	}

	/**
	 * Intent: reset the chessman (after promoting)
	 * Pre: void
	 * Post: void
	 */
	void reset(void)
	{
		//P
		pawn.clear();
		for (int i = 0; i < 8; i++) {
			Pawn temp(i, color);
			pawn.push_back(temp);
		}

		//R
		rook.clear();
		for (int i = 0; i < 2; i++) {
			Rook temp(i, color);
			rook.push_back(temp);
		}

		//N
		knight.clear();
		for (int i = 0; i < 2; i++) {
			Knight temp(i, color);
			knight.push_back(temp);
		}

		//B
		bishop.clear();
		for (int i = 0; i < 2; i++) {
			Bishop temp(i, color);
			bishop.push_back(temp);
		}

		//Q
		queen.clear();
		Queen tempQ(0, color);
		queen.push_back(tempQ);

		//K
		king.clear();
		King tempK(0, color);
		king.push_back(tempK);

		PMove = false;
		haveEat = false;
	}
};

