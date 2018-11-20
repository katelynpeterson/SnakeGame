#include "OpenGL.h"
#include "GL/glut.h"
#include "GL/GL.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <ctime>
#include <Windows.h>
#include <fstream>

using namespace std;
const int gridHeight = 50;
const int gridWidth = 60;
const int INITIAL_SIZE = 5;
const double PGenerateFruit = .01;
const double PGenerateBomb = .009;

bool bw = false;
int iter = 0;
int headX = 25;
int headY = 25;
int grow = INITIAL_SIZE;
//snake2
int head2X = 30;
int head2Y = 30;
int grow2 = INITIAL_SIZE;

list<int> snakeX;
list<int> snakeY;
//snake 2
list<int> snake2X;
list<int> snake2Y;

char board[gridWidth][gridHeight];
char direction = 'E';
char direction2 = 'W';
bool gameOver = true;
bool isPaused = false;

void DrawElement(double i, double j, char element) {
	double x = (i+.5) / gridWidth * 2.0 - 1.0;
	double y = (j+.5) / gridHeight * 2.0 - 1.0;
	double r = 1.0 / gridHeight;
	switch (element) {
	case 'S':
		glColor3f(0, bw*.5, !bw*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 's':
		glColor3f(0, !bw*.5, bw*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 'T':
		glColor3f(bw*.7, 0, !bw*.7);
		DrawCircle(x, y, r, 10);
		break;
	case 't':
		glColor3f(!bw*.7, 0, bw*.7);
		DrawCircle(x, y, r, 10);
		break;
	case 'A':
		glColor3f(1.0, 0.0, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(0, 1.0, 0);
		DrawCircle(x+r*.15, y+r*.9, r*.4, 10);
		break;
	case 'G':
		glColor3f(1.0, 1.0, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(0, 1.0, 0);
		DrawCircle(x + r * .15, y + r * .9, r*.4, 10);
		break;
	case 'B':
		glColor3f(0,0,0);
		DrawRectangle(x-r, y-r, x+r, y+r);
		break;
	case 'O':
		glColor3f(0, 0, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(1.0, 0, 0);
		DrawCircle(x + r * .15, y + r * .9, r*.4, 10);
		break;
	case ' ':
		break;
	default:
		break;
	}
}

void update() {
	
	bw = !bw;
	if (isPaused)
		return;
	//Make a wall
	for (int i = 20; i < 30; i++) {
		board[20][i] = 'B';
		board[40][i] = 'B';
	}
	//Generate Fruit...
	if (rand() / (double)RAND_MAX < PGenerateFruit) {
		int x = rand() % gridWidth;
		int y = rand() % gridHeight;
		if (board[x][y] != 'B' && board[x][y] != 'S' && board[x][y] != 's'&& board[x][y] != 'T' && board[x][y] != 't'&& board[x][y] != 'A') {
			board[x][y] = 'A';
		}
	}
	if (rand() / (double)RAND_MAX < PGenerateFruit) {
		int x = rand() % gridWidth;
		int y = rand() % gridHeight;
		if (board[x][y] != 'B' && board[x][y] != 'S' && board[x][y] != 's' && board[x][y] != 'T' && board[x][y] != 't'&& board[x][y] != 'A'&& board[x][y] != 'G') {
			board[x][y] = 'G';
		}
	}
	if (rand() / (double)RAND_MAX < PGenerateBomb) {
		int x = rand() % gridWidth;
		int y = rand() % gridHeight;
		if (board[x][y] != 'B' && board[x][y] != 'S' && board[x][y] != 's'&& board[x][y] != 'T' && board[x][y] != 't' && board[x][y] != 'A' && board[x][y]!='G' && board[x][y]!='O') {
			board[x][y] = 'O';
		}
	}
	
	//Find out where the head will be...
	switch (direction) {
		case 'N': headY++; break;
		case 'E': headX++; break;
		case 'S': headY--; break;
		case 'W': headX--; break;
	}
	//snake 2
	switch (direction2) {
	case 'N': head2Y++; break;
	case 'E': head2X++; break;
	case 'S': head2Y--; break;
	case 'W': head2X--; break;
	}
	if (headX < 0) headX = gridWidth - 1;
	if (headY < 0) headY = gridHeight - 1;
	if (headX >= gridWidth)  headX = 0;
	if (headY >= gridHeight) headY = 0;

	//snake 2
	if (head2X < 0) head2X = gridWidth - 1;
	if (head2Y < 0) head2Y = gridHeight - 1;
	if (head2X >= gridWidth)  head2X = 0;
	if (head2Y >= gridHeight) head2Y = 0;

	//Move the snake's tail..
	if (grow == 0) {
		int tailX = snakeX.back();
		int tailY = snakeY.back();
		snakeX.pop_back();
		snakeY.pop_back();
		board[tailX][tailY] = ' ';
	}
	else if (grow < 0) {
		for (int i = 0; i < 2; i++) {
			int tailX = snakeX.back();
			int tailY = snakeY.back();
			snakeX.pop_back();
			snakeY.pop_back();
			board[tailX][tailY] = ' ';
		}
		grow++;
	}
	else {
		grow--;
	}

	//snake 2 tail
	if (grow2 == 0) {
		int tail2X = snake2X.back();
		int tail2Y = snake2Y.back();
		snake2X.pop_back();
		snake2Y.pop_back();
		board[tail2X][tail2Y] = ' ';
	}
	else if (grow2 < 0) {
		snake2X.pop_back();
		snake2Y.pop_back();
		grow2++;
	}
	else {
		grow2--;
	}

	switch (board[headX][headY])
	{
		case 'A': grow += 4; break;
		case 'G': grow += 8; break;
		case 'O': {if (snakeX.size() <= 5) { gameOver = true; }
				  else grow -= 6;
			break; }
		case 'S': case 's': case 'B': case 'T': case 't': gameOver = true; break;
	}

	//snake 2
	switch (board[head2X][head2Y])
	{
	case 'A': grow2 += 4; break;
	case 'G': grow2 += 8; break;
	case 'O': grow -= 6; break;
	case 'S': case 's': case 'B': case 'T': case 't': gameOver = true; break;
	}

	//Move the snake's head.
	board[headX][headY] = bw ? 'S' : 's';
	snakeX.push_front(headX);
	snakeY.push_front(headY);

	//snake 2
	board[head2X][head2Y] = bw ? 'T' : 't';
	snake2X.push_front(head2X);
	snake2Y.push_front(head2Y);

	//move the apples away from snake in same direction	
	moveApple(headX, headY, direction);
	moveApple(head2X, head2Y, direction2);
	
	iter++;
}

void moveApple(int X, int Y, char dir) {
	char applePlaceholder;
	if (iter % 5 == 0) {
		for (int i = X - 5; i < X + 5; i++) {
			for (int j = Y - 5; j < Y + 5; j++) {
				if (board[i][j] == 'A' || board[i][j] == 'G') {
					applePlaceholder = board[i][j];
					board[i][j] = ' ';

					switch (dir) {
					case 'N': {
						if ((j+1) > gridHeight) {
							board[i][0] = applePlaceholder;
						}
						else {
							board[i][j - 1] = applePlaceholder;
						}
						break; }
					case 'E': {
						if ((i+1) > gridWidth) {
							board[0][j] = applePlaceholder;
						}
						else {
							board[i - 1][j] = applePlaceholder;
						}
						break; }
					case 'S': {
						if ((j-1) < 0) {
							board[i][gridHeight] = applePlaceholder;
						}
						else {
							board[i][j - 1] = applePlaceholder;
						}
						break; }
					case 'W': {
						if ((i-1) < 0) {
							board[gridWidth][j] = applePlaceholder;
						}
						else {
							board[i - 1][j] = applePlaceholder;
						}
						break; }
					}
				}
			}
		}
	}
}

void loadGame() {
	isPaused = true;
	ifstream snakeFile;
	string fileName;
	cout << "\nEnter the name of the file you want to load: ";
	cin >> fileName;
	snakeFile.open(fileName);
	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			snakeFile.get(board[i][j]);
		}
	}
	snakeFile.close();
}

void saveGame() {
	isPaused = true;
	ofstream snakeFile;
	string fileName;
	cout << "\nEnter the name you want your saved game to be called: ";
	cin >> fileName;
	snakeFile.open(fileName);
	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			snakeFile<<board[i][j];
		}
	}
	cout << "File " + fileName + " saved";
	snakeFile.close();
}

void draw() {
	
		for (int i = 0; i < gridWidth; i++) {
			for (int j = 0; j < gridHeight; j++) {
				DrawElement(i, j, board[i][j]);
			}
		}
		Sleep(100);
		if (!gameOver)
			update();	
}

void keyboard(int key) {
	switch (key) {
		case 'w': if (direction == 'E' || direction == 'W') direction = 'N'; break;
		case 'a': if (direction == 'N' || direction == 'S') direction = 'W'; break;
		case 's': if (direction == 'E' || direction == 'W') direction = 'S'; break;
		case 'd': if (direction == 'N' || direction == 'S') direction = 'E'; break;
		//snake 2
		case 'i': if (direction2 == 'E' || direction2 == 'W') direction2 = 'N'; break;
		case 'j': if (direction2 == 'N' || direction2 == 'S') direction2 = 'W'; break;
		case 'k': if (direction2 == 'E' || direction2 == 'W') direction2 = 'S'; break;
		case 'l': if (direction2 == 'N' || direction2 == 'S') direction2 = 'E'; break;
		case 'm': loadGame(); break;
		case 'n': saveGame(); break;
		case ' ': 
			if (gameOver) {
				gameOver = false;
				snakeX.clear();
				snakeY.clear();
				headX = gridWidth / 2.0-5;
				headY = gridHeight / 2.0-5;
				//snake 2
				snake2X.clear();
				snake2Y.clear();
				head2X = gridWidth / 2.0+4;
				head2Y = gridHeight / 2.0+4;
				for (int i = 0; i < gridWidth; i++) {
					for (int j = 0; j < gridHeight; j++) {
						board[i][j] = ' ';
					}
				}
				grow = grow2 = INITIAL_SIZE;
				direction = 'E';
				direction2 = 'W';
			}
			break;
		case 'p': isPaused=!isPaused; break;
	}
}

int main(int argc, char** argv) {
		RunOpenGL(argc, argv, draw, NULL, keyboard);
}