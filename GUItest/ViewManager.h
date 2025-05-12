#pragma once
#define RESET   "\033[0m" //reset
#define BLACK_C     "\033[30m" //black letter
#define BLUE_B    "\033[46m" //blue background
#define YELLOW_B  "\033[43m" //yellow background
#define RED_B     "\033[41m" //red background
#include <sstream>
using namespace std;

//to view the board
class ViewManager
{
private:
	string castle = "\0";
public:
	Player* black, * white;

	Position guiPrintCanMove(vector<Position> print)
	{
		Position tempPos;
		for (int i = 0; i < 8; i++) {
			//ImGui::Text(to_string(8 - i).c_str());
			//ImGui::SameLine();
			for (int j = 0; j < 8; j++) {
				string buttonName;
				bool ifCanMove = false;
				tempPos.setPosition(j, i);

				if (white->playerBoard[i][j] != ' ') {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					buttonName = white->playerBoard[i][j];
				}
				else if (black->playerBoard[i][j] != ' ') {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
					buttonName = black->playerBoard[i][j];
				}
				else {
					buttonName = " ";
				}
				buttonName += "##" + to_string(i) + to_string(j);

				for (int k = 0; k < print.size(); k++) {
					if (tempPos == print[k]) {
						ifCanMove = true;
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.38f, 0.67f, 1.0f));
						break;
					}
				}

				if (ImGui::Button(buttonName.c_str()) && ifCanMove) {
					std::cout << "Move " << buttonName[0] << endl;
					return tempPos;
				}

				ImGui::SameLine();
				ImGui::PopStyleColor();
			}
			ImGui::NewLine();
		}
		ImGui::Text("second");
		return Position(-1, -1);
	}


	/**
	 * Intent: print the player board
	 * Pre: void
	 * Post: void
	 */
	void printBoard(void)
	{
		for (int i = 0; i < 8; i++) {
			cout << 8 - i;
			for (int j = 0; j < 8; j++) {
				if ((i % 2 == 0 && j % 2 == 0) || ((i % 2 == 1 && j % 2 == 1))) {
					cout << YELLOW_B; //yellow background
				}
				else {
					cout << BLUE_B; //blue background
				}

				if (white->playerBoard[i][j] != ' ') {
					cout << white->playerBoard[i][j];
				}
				else if (black->playerBoard[i][j] != ' ') {
					cout << BLACK_C << black->playerBoard[i][j]; //black letter
				}
				else {
					cout << " ";
				}
				cout << RESET; //reset
			}
			cout << endl;
		}

		cout << " abcdefgh" << endl;
	}

	/**
	 * Intent: print can move position
	 * Pre: the chess can move position
	 * Post: void
	 */
	void printCanMove(vector<Position> print)
	{
		for (int i = 0; i < 8; i++) {
			cout << 8 - i;
			for (int j = 0; j < 8; j++) {

				if ((i % 2 == 0 && j % 2 == 0) || ((i % 2 == 1 && j % 2 == 1))) {
					cout << YELLOW_B;
				}
				else {
					cout << BLUE_B;
				}

				//change the canMovePos backgroung to red
				for (int k = 0; k < print.size(); k++) {
					Position locate(j, i);
					if (locate == print[k]) {
						cout << RESET;
						cout << RED_B;
						print.erase(print.begin() + k);
						break;
					}
				}

				if (white->playerBoard[i][j] != ' ') {
					cout << white->playerBoard[i][j];
				}
				else if (black->playerBoard[i][j] != ' ') {
					cout << BLACK_C << black->playerBoard[i][j];
				}
				else {
					cout << " ";
				}
				cout << RESET;
			}
			cout << endl;
		}

		cout << " abcdefgh" << endl;
	}

	/**
	 * Intent: read FEE (doea not success)
	 * Pre: string
	 * Post: who first
	 */
	int readFEN(string input)
	{
		vector<string> tokens;
		string token;
		int turn = 0;

		istringstream tokenStream(input);

		for (int i = 0; i < 7; i++) {
			getline(tokenStream, token, '/');
			tokens.push_back(token);
		}
		getline(tokenStream, token, ' ');
		tokens.push_back(token);

		if (tokenStream.str().size() != 0) {
			getline(tokenStream, token, ' '); //who first
			if (token[0] == 'b') turn = 1;
			else if (token[0] == 'w') turn = -1;
			else turn = 0;

			if (tokenStream.str().size() != 0) {
				getline(tokenStream, castle, ' '); //castle
			}
		}


		//init
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				white->playerBoard[i][j] = ' ';
				black->playerBoard[i][j] = ' ';
			}
		}

		//divide
		int i = 0;
		for (string str : tokens) {
			int j = 0;
			for (char c : str) {
				if (isalpha(c)) {
					if (isupper(c)) white->playerBoard[i][j] = c;
					else black->playerBoard[i][j] = toupper(c);
					j++;
				}
				else {
					j += c - '0';
				}

			}
			i++;
		}

		readPlayBoard();
		return turn;
	}

	/**
	 * Intent: read playBoard and set chess (not success)
	 * Pre: void
	 * Post: void
	 */
	void readPlayBoard(void)
	{
		//clean
		black->king.clear();
		black->queen.clear();
		black->rook.clear();
		black->knight.clear();
		black->bishop.clear();
		black->pawn.clear();

		//set black chess
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				char temp = black->playerBoard[i][j];
				if (temp == ' ') continue;
				King tempK; //ismove
				Queen tempQ;
				Rook tempR; //ismove
				Knight tempN;
				Bishop tempB;
				Pawn tempP; //movetime enpassant
				switch (temp) {
				case 'K':
					tempK.color = 1;
					tempK.isMove = true;
					tempK.pos.setPosition(i, j);
					black->king.push_back(tempK);
					break;
				case 'Q':
					tempQ.color = 1;
					tempQ.pos.setPosition(i, j);
					black->queen.push_back(tempQ);
					break;
				case 'R':
					tempR.color = 1;
					tempR.isMove = true;
					tempR.pos.setPosition(i, j);
					black->rook.push_back(tempR);
					break;
				case 'N':
					tempN.color = 1;
					tempN.pos.setPosition(i, j);
					black->knight.push_back(tempN);
				case 'B':
					tempB.color = 1;
					tempB.pos.setPosition(i, j);
					black->bishop.push_back(tempB);
					break;
				case 'P':
					tempP.color = 1;
					tempP.pos.setPosition(i, j);
					black->pawn.push_back(tempP);
					break;
				}
			}
		}

		for (int i = 0; i < black->pawn.size(); i++) {
			if (black->pawn[i].pos.y != 6) {
				black->pawn[i].moveTimes = 1;
			}
		}

		//set white chess
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				char temp = white->playerBoard[i][j];
				if (temp == ' ') continue;
				King tempK; //ismove
				Queen tempQ;
				Rook tempR; //ismove
				Knight tempN;
				Bishop tempB;
				Pawn tempP; //movetime enpassant
				switch (temp) {
				case 'K':
					tempK.color = 1;
					tempK.isMove = true;
					tempK.pos.setPosition(i, j);
					white->king.push_back(tempK);
					break;
				case 'Q':
					tempQ.color = 1;
					tempQ.pos.setPosition(i, j);
					white->queen.push_back(tempQ);
					break;
				case 'R':
					tempR.color = 1;
					tempR.isMove = true;
					tempR.pos.setPosition(i, j);
					white->rook.push_back(tempR);
					break;
				case 'N':
					tempN.color = 1;
					tempN.pos.setPosition(i, j);
					white->knight.push_back(tempN);
				case 'B':
					tempB.color = 1;
					tempB.pos.setPosition(i, j);
					white->bishop.push_back(tempB);
					break;
				case 'P':
					tempP.color = 1;
					tempP.pos.setPosition(i, j);
					white->pawn.push_back(tempP);
					break;
				}
			}
		}

		for (int i = 0; i < white->pawn.size(); i++) {
			if (white->pawn[i].pos.y != 6) {
				white->pawn[i].moveTimes = 1;
			}
		}

		//set castle
		for (int i = 0; i < this->castle.size(); i++) {
			int index;
			switch (this->castle[i]) {
			case 'K':
				for (index = 0; index < white->rook.size(); index++) {
					if (white->rook[i].pos.x == 7 && white->rook[i].pos.y == 7) break;
				}
				white->rook[index].isMove = false;
				white->king[0].isMove = false;
				break;
			case 'Q':
				for (index = 0; index < white->rook.size(); index++) {
					if (white->rook[i].pos.x == 0 && white->rook[i].pos.y == 7) break;
				}
				white->rook[index].isMove = false;
				white->king[0].isMove = false;
				break;
			case 'k':
				for (index = 0; index < black->rook.size(); index++) {
					if (black->rook[i].pos.x == 7 && black->rook[i].pos.y == 7) break;
				}
				black->rook[index].isMove = false;
				black->king[0].isMove = false;
				break;
			case 'q':
				for (index = 0; index < black->rook.size(); index++) {
					if (black->rook[i].pos.x == 7 && black->rook[i].pos.y == 7) break;
				}
				black->rook[index].isMove = false;
				black->king[0].isMove = false;
				break;
			}
		}

		black->updateCanMovePos(*white);
		white->updateCanMovePos(*black);
	}
};
