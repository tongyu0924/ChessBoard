#include <windows.h>
#include <mmsystem.h>
#pragma comment (lib,"Winmm.lib")
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "Position.h"
#include "Player.h"
#include "GameManager.h"
#include "ViewManager.h"

using namespace std;

bool moveChess(const char&, const Position&, Position);
int checkMate(int, Player&, Player&);

Player black(1), white(-1); //black player, white player
ViewManager myChess;
GameManager myGame;
//GLFWwindow* window;

int main(void)
{
	PlaySound(TEXT("./music/Croatian_Rhapsody.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Chess Game", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (gladLoadGL() == 0) {
		return -1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	bool show_menu_window = true;
	bool show_help_window = false;
	bool show_tool_window = false;
	bool show_music_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	myChess.black = &black;
	myChess.white = &white;
	//myGame.menu();
	black.update();
	white.update();
	black.updateCanMovePos(white);
	white.updateCanMovePos(black);

	Player tmpB(1);
	Player tmpW(1);
	Position end(-1, -1);
	Position fromPos(end), toPos(end);
	char chess;
	bool haveChessman = false;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//test if checkMate
		if (!haveChessman) { //00000000000
			myGame.checkKing = checkMate(myGame.turns, black, white);
			if (myGame.checkKing == 0) {
				if (myGame.turns == 1) {
					cout << "white win" << endl;
					cout << "black dead" << endl;
				}
				else {
					cout << "black win" << endl;
					cout << "white dead" << endl;
				}
				break;
			}
			// king be threatened
			else if (myGame.checkKing == 1) {
				cout << "King gonna dead!!Save him/her/them!!" << endl;
				cout << "Only move King!" << endl;
			}
		}

		//test if draw //0000000
		/*if (myGame.turns == 1) {
			if (myGame.testIfDraw(black, white)) {
				cout << "Draw" << endl;
				break;
			}
		}
		else {
			if (myGame.testIfDraw(white, black)) {
				cout << "Draw" << endl;
				break;
			}
		}*/

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Poll for and process events */
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Menu Window", &show_menu_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Music Window", &show_music_window);
			ImGui::Checkbox("Tools Window", &show_tool_window);
			ImGui::Checkbox("Help Window", &show_help_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		//show the menu window
		if (show_menu_window) {
			bool whiteFirst;
			bool blackFirst;
			if (myGame.turns == 1) {
				whiteFirst = false;
				blackFirst = true;
				//haveChessman = false;
			}
			else {
				whiteFirst = true;
				blackFirst = false;
				//haveChessman = false;
			}

			ImGui::Begin("Menu", &show_menu_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Who's first?");
			if (ImGui::RadioButton("White", whiteFirst)) myGame.turns = -1;

			ImGui::SameLine();
			if (ImGui::RadioButton("Black", blackFirst)) myGame.turns = 1;

			if (ImGui::Button("Close Me"))
				show_menu_window = false;
			ImGui::End();
		}

		//show the tool window
		if (show_tool_window) {
			ImGui::Begin("Tools", &show_tool_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

			//if (ImGui::BeginMenu("File")) {
			//	if (ImGui::MenuItem("Save", "Ctrl+s")) { //000000

			//	}
			//	ImGui::EndMenu();
			//}

			if (ImGui::Button("Restart")) {
				haveChessman = false;

				black.reset();
				white.reset();

				black.update();
				white.update();

				black.updateCanMovePos(white);
				white.updateCanMovePos(black);

				myGame.turns = -1; //白先

				//system("cls");
				cout << "restart successful" << endl;
			}

			if (ImGui::Button("Undo")) {
				if (myGame.turns == 1) myGame.turns = -1;
				else myGame.turns = 1;

				white = tmpW;
				black = tmpB;
				haveChessman = false;

				black.update();
				white.update();
				black.updateCanMovePos(white);
				white.updateCanMovePos(black);
				//system("cls");
				cout << "undo success" << endl;
				//continue;
			}

			//if (ImGui::Button("FEN")) //00000000

			if (ImGui::Button("Change Chessman")) {
				haveChessman = false;
			}

			if (ImGui::Button("Close Me"))
				show_tool_window = false;
			ImGui::End();
		}

		//show help window to learn the rules
		if (show_help_window)
		{
			ImGui::Begin("Chess Rule", &show_help_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("King : ....");
			ImGui::Text("Queen : ....");
			ImGui::Text("Bishop : ....");
			ImGui::Text("Knight : ....");
			ImGui::Text("Rook : ....");
			ImGui::Text("Pawn : ....");
			if (ImGui::Button("Close Me"))
				show_help_window = false;
			ImGui::End();
		}

		//show music window to set the music
		if (show_music_window)
		{
			ImGui::Begin("Music", &show_menu_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Choose the music︰");

			//init
			bool musics[6];
			for (int i = 0; i < 6; i++) {
				if (i == myGame.music) musics[i] = true;
				else musics[i] = false;
			}

			if (ImGui::RadioButton("Croatian_Rhapsody", musics[0])) {
				myGame.music = 0;
				PlaySound(NULL, NULL, SND_ASYNC | SND_PURGE);
				PlaySound(TEXT("./music/Croatian_Rhapsody.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			if (ImGui::RadioButton("happy song", musics[1])) {
				myGame.music = 1;
				PlaySound(NULL, NULL, SND_ASYNC | SND_PURGE);
				PlaySound(TEXT("./music/backround_music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			if (ImGui::RadioButton("Sonata", musics[2])) {
				myGame.music = 2;
				PlaySound(NULL, NULL, SND_ASYNC | SND_PURGE);
				PlaySound(TEXT("./music/Sonata.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			if (ImGui::RadioButton("cry_yourmom_dead", musics[3])) {
				myGame.music = 3;
				PlaySound(NULL, NULL, SND_ASYNC | SND_PURGE);
				PlaySound(TEXT("./music/cry_yourmom_dead.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			if (ImGui::RadioButton("Volume_Warning(guodong)", musics[4])) {
				myGame.music = 4;
				PlaySound(NULL, NULL, SND_ASYNC | SND_PURGE);
				PlaySound(TEXT("./music/Volume_Warning.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			if (ImGui::RadioButton("Stop music", musics[5])) {
				myGame.music = 5;
				PlaySound(NULL, NULL, SND_ASYNC | SND_PURGE);
			}

			if (ImGui::Button("Close Me"))
				show_music_window = false;
			ImGui::End();
		}

		//chess board
		{
			ImGui::Begin("Chess Board");

			if (!haveChessman) {
				//print board
				for (int i = 0; i < 8; i++) {
					//ImGui::Text(to_string(8 - i).c_str());
					//ImGui::SameLine();
					for (int j = 0; j < 8; j++) {
						string buttonName;
						bool ifCanMove = false;

						//set button name and its text color
						if (white.playerBoard[i][j] != ' ') {
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
							buttonName = white.playerBoard[i][j];
						}
						else if (black.playerBoard[i][j] != ' ') {
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
							buttonName = black.playerBoard[i][j];
						}
						else {
							buttonName = " ";
						}

						buttonName += "##" + to_string(i) + to_string(j);

						if (myGame.checkKing == 1) {
							if (buttonName[0] == 'K') ifCanMove = true;
						}
						else if ((myGame.turns == 1 && black.playerBoard[i][j] != ' ')
							|| (myGame.turns == -1 && white.playerBoard[i][j] != ' ')) {
							ifCanMove = true;
						}
						//canMovePos=0 //00000000000

						if (ifCanMove) {
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85f, 0.38f, 0.67f, 1.0f));
							if (ImGui::Button(buttonName.c_str())) {
								chess = buttonName[0];
								std::cout << "You choose " << chess << endl;
								fromPos.setPosition(j, i);
								haveChessman = true;
							}
						}
						else {
							ImGui::Button(buttonName.c_str());
						}

						ImGui::SameLine();
						ImGui::PopStyleColor();
					}
					ImGui::NewLine();
				}
			}
			else {//print canMovePos
				vector<Position> print;
				if (myGame.turns == 1) print = black.returnCanMovePos(chess, fromPos);
				else print = white.returnCanMovePos(chess, fromPos);

				toPos = myChess.guiPrintCanMove(print);

				if (toPos != end) {
					//archive
					tmpB = black;
					tmpW = white;

					moveChess(chess, fromPos, toPos);

					//doing promoting //0000000000
					if (myGame.turns == -1) {
						if (toPos.y == 0 && chess == 'P') {
							white.promoting(toPos);
						}
					}
					else {
						if (toPos.y == 7 && chess == 'P') {
							black.promoting(toPos);
						}
					}

					black.update();
					white.update();
					black.updateCanMovePos(white);
					white.updateCanMovePos(black);

					//change the player
					if (myGame.turns == 1) myGame.turns = -1;
					else myGame.turns = 1;

					haveChessman = false;
				}

			}

			ImGui::PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::End();
		}



		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	PlaySound(NULL, NULL, SND_ASYNC | SND_PURGE);
	return 0;
}

/**
 * Intent: move the chess
 * Pre: the chess, fromPos and toPos
 * Post: boolean
 */
bool moveChess(const char& chess, const Position& fromPos, Position toPos)
{
	//move or eat
	if (myGame.turns == 1) {
		if (black.playerBoard[toPos.y][toPos.x] != ' ') {
			system("cls");
			cout << "invalid position (位置上已經有棋子)" << endl;
			return false;
		}
		else {
			//move
			if (black.move(chess, fromPos, toPos)) {
				system("cls");
				cout << "success!" << endl;
			}
			else {
				system("cls");
				cout << "invalid move way" << endl;
				return false;
			}

			//pawn is not enpassant
			for (int i = 0; i < white.pawn.size(); i++) {
				white.pawn[i].enpassant = false;
			}

			//eat
			if (white.playerBoard[toPos.y][toPos.x] != ' ') {
				white.beEat(toPos);
			}
			//eat enpassant
			else  if (chess == 'P') {
				int dx = toPos.x - fromPos.x;
				int dy = toPos.y - fromPos.y;
				if (abs(dx) == 1 && abs(dy) == 1) {
					toPos.y -= 1;
					white.beEat(toPos);
				}
			}
		}
	}
	else if (myGame.turns == -1) {
		if (white.playerBoard[toPos.y][toPos.x] != ' ') {
			system("cls");
			cout << "invalid position (位置上已經有棋子)" << endl;
			return false;
		}
		else {
			//move
			if (white.move(chess, fromPos, toPos)) {
				system("cls");
				cout << "success" << endl;
			}
			else {
				system("cls");
				cout << "invalid move way" << endl;
				return false;
			}

			for (int i = 0; i < black.pawn.size(); i++) {
				black.pawn[i].enpassant = false;
			}

			//eat
			if (black.playerBoard[toPos.y][toPos.x] != ' ') {
				black.beEat(toPos);
			}
			//eat enpassant
			else  if (chess == 'P') {
				int dx = toPos.x - fromPos.x;
				int dy = toPos.y - fromPos.y;
				if (abs(dx) == 1 && abs(dy) == 1) {
					toPos.y += 1;
					black.beEat(toPos);
				}
			}
		}
	}
	return true;
}

int checkMate(int index, Player& black, Player& white) {
	//check if threaten
	int checkKing = 0;
	int checCheck = 0;
	//check if all chess can eat it
	//judge king (1:black/-1:white)
	if (index == 1) {
		//king can move range
		for (int i = 0; i < black.king[0].canMovePos.size(); i++) {
			checkKing = 0;
			//Q
			for (int j = 0; j < white.queen.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < white.queen[j].canMovePos.size(); k++) {
					if (black.king[0].canMovePos[i] == white.queen[j].canMovePos[k]) {
						auto item = black.king[0].canMovePos.erase(black.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}

			//R
			for (int j = 0; j < white.rook.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < white.rook[j].canMovePos.size(); k++) {
					if (black.king[0].canMovePos[i] == white.rook[j].canMovePos[k]) {
						auto item = black.king[0].canMovePos.erase(black.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}

			//N
			for (int j = 0; j < white.knight.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < white.knight[j].canMovePos.size(); k++) {
					if (black.king[0].canMovePos[i] == white.knight[j].canMovePos[k]) {
						auto item = black.king[0].canMovePos.erase(black.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}


			//B
			for (int j = 0; j < white.bishop.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < white.bishop[j].canMovePos.size(); k++) {
					if (black.king[0].canMovePos[i] == white.bishop[j].canMovePos[k]) {
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}

			//P
			for (int j = 0; j < white.pawn.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < white.pawn[j].canMovePos.size(); k++) {
					if (black.king[0].canMovePos[i].x == white.pawn[j].canMovePos[k].x - 1 && black.king[0].canMovePos[i].y == white.pawn[j].canMovePos[k].y) {
						auto item = black.king[0].canMovePos.erase(black.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
					if (black.king[0].canMovePos[i].x == white.pawn[j].canMovePos[k].x + 1 && black.king[0].canMovePos[i].y == white.pawn[j].canMovePos[k].y) {
						auto item = black.king[0].canMovePos.erase(black.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}
		}
	}
	else if (index == -1) {
		//king can move range in outer circle
		for (int i = 0; i < white.king[0].canMovePos.size(); i++) {
			checkKing = 0;
			//Q
			for (int j = 0; j < black.queen.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < black.queen[j].canMovePos.size(); k++) {
					if (white.king[0].canMovePos[i] == black.queen[j].canMovePos[k]) {
						auto item = white.king[0].canMovePos.erase(white.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}

			//R
			for (int j = 0; j < black.rook.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < black.rook[j].canMovePos.size(); k++) {
					if (white.king[0].canMovePos[i] == black.rook[j].canMovePos[k]) {
						auto item = white.king[0].canMovePos.erase(white.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}

			//N
			for (int j = 0; j < black.knight.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < black.knight[j].canMovePos.size(); k++) {
					if (white.king[0].canMovePos[i] == black.knight[j].canMovePos[k]) {
						auto item = white.king[0].canMovePos.erase(white.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}

			//B
			for (int j = 0; j < black.bishop.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < black.bishop[j].canMovePos.size(); k++) {
					if (white.king[0].canMovePos[i] == black.bishop[j].canMovePos[k]) {
						auto item = white.king[0].canMovePos.erase(white.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}

			//P
			for (int j = 0; j < black.pawn.size(); j++) {
				if (checkKing == 1) { break; }
				for (int k = 0; k < black.pawn[j].canMovePos.size(); k++) {
					if (white.king[0].canMovePos[i].x == black.pawn[j].canMovePos[k].x - 1 && white.king[0].canMovePos[i].y == black.pawn[j].canMovePos[k].y) {
						auto item = white.king[0].canMovePos.erase(white.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
					if (white.king[0].canMovePos[i].x == black.pawn[j].canMovePos[k].x + 1 && white.king[0].canMovePos[i].y == black.pawn[j].canMovePos[k].y) {
						auto item = white.king[0].canMovePos.erase(white.king[0].canMovePos.begin() + i);
						checkKing = 1;
						checCheck = 1;
						break;
					}
				}
			}
		}
	}

	if (index == 1) {
		if (black.king[0].canMovePos.size() == 0 && checCheck == 1) {
			return 0; //checkMate : game over
		}
		else if (checCheck == 1) {
			return 1; //checkMate : be threaten
		}
		else {
			return 2; //no checkmate and does not be threaten
		}
	}
	else if (index == -1) {
		if (white.king[0].canMovePos.size() == 0 && checCheck == 1) {
			return 0; //checkMate : game over
		}
		else if (checCheck == 1) {
			return 1; //checkMate : be threaten
		}
		else {
			return 2; //no checkmate and does not be threaten
		}
	}
}

