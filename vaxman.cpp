// Author: Patricia Terol
// Course: CSE 2050
// Project: assign10
//
// Modified by Omar Kalam (@Homshnogen)
// www.github.com/Homshnogen/Pacman-1

#include <stdlib.h>
#include <vector>
#include <list>
#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

struct Monster;
struct Point;

static bool replay = false; //check if starts a new game
static bool over = true; //check for the game to be over
static float tileSize = 50.0; //size of one square on the game
static float pacRadius = 16.0; // redius of pacman
static float pacX = 1.5; // x position of pacman
static float pacY = 1.5; // y position of pacman
static int pacRotation = 0; // orientation of pacman
static list<Monster> monsters;
static vector<int> border = { 0, 0, 15, 1, 15, 15, 14, 1, 0, 14, 15, 15, 1, 14, 0, 0 }; //coordinates of the border walls

//coordinates of the obstacles (divided into 3 for clarity)
static vector<int> obstaclesTop = { 2, 2, 3, 6, 3, 6, 4, 5, 4, 2, 5, 4, 5, 3, 6, 5, 6, 1, 9, 2, 7, 2, 8, 5, 9, 5, 10, 3, 10, 4, 11, 2, 11, 5, 12, 6, 12, 6, 13, 2 };
static vector<int> obstaclesMiddle = { 2, 9, 3, 7, 3, 7, 4, 8, 4, 9, 5, 11, 5, 6, 6, 10, 6, 10, 7, 8, 7, 8, 8, 9, 6, 7, 7, 6, 8, 6, 9, 7, 10, 6, 9, 10, 9, 10, 8, 8, 11, 9, 10, 11, 11, 8, 12, 7, 12, 7, 13, 9 };
static vector<int> obstaclesBottom = { 2, 10, 3, 13, 3, 13, 4, 12, 5, 12, 6, 13, 6, 13, 7, 11, 8, 11, 9, 13, 9, 13, 10, 12, 11, 12, 12, 13, 12, 13, 13, 10 };
static vector<float> foodPositions = { 1.5, 1.5, 1.5, 2.5, 1.5, 3.5, 1.5, 4.5, 1.5, 5.5, 1.5, 6.5, 1.5, 7.5, 1.5, 8.5, 1.5, 9.5, 1.5, 10.5, 1.5, 11.5, 1.5, 12.5, 1.5, 13.5, 2.5, 1.5, 2.5, 6.5, 2.5, 9.5, 2.5, 13.5, 3.5, 1.5, 3.5, 2.5, 3.5, 3.5, 3.5, 4.5, 3.5, 6.5, 3.5, 8.5, 3.5, 9.5, 3.5, 10.5, 3.5, 11.5, 3.5, 13.5, 4.5, 1.5, 4.5, 4.5, 4.5, 5.5, 4.5, 6.5, 4.5, 7.5, 4.5, 8.5, 4.5, 11.5, 4.5, 12.5, 4.5, 13.5, 5.5, 1.5, 5.5, 2.5, 5.5, 5.5, 5.5, 10.5, 5.5, 11.5, 5.5, 13.5, 6.5, 2.5, 6.5, 3.5, 6.5, 4.5, 6.5, 5.5, 6.5, 7.5, 6.5, 10.5, 6.5, 13.5, 7.5, 5.5, 7.5, 6.5, 7.5, 7.5, 7.5, 9.5, 7.5, 10.5, 7.5, 11.5, 7.5, 12.5, 7.5, 13.5, 8.5, 2.5, 8.5, 3.5, 8.5, 4.5, 8.5, 5.5, 8.5, 7.5, 8.5, 10.5, 8.5, 13.5, 9.5, 1.5, 9.5, 2.5, 9.5, 5.5, 9.5, 10.5, 9.5, 11.5, 9.5, 13.5, 10.5, 1.5, 10.5, 4.5, 10.5, 5.5, 10.5, 6.5, 10.5, 7.5, 10.5, 8.5, 10.5, 11.5, 10.5, 12.5, 10.5, 13.5, 11.5, 1.5, 11.5, 2.5, 11.5, 3.5, 11.5, 4.5, 11.5, 5.5, 11.5, 6.5, 11.5, 8.5, 11.5, 9.5, 11.5, 10.5, 11.5, 11.5, 11.5, 13.5, 12.5, 1.5, 12.5, 6.5, 12.5, 9.5, 12.5, 13.5, 13.5, 1.5, 13.5, 2.5, 13.5, 3.5, 13.5, 4.5, 13.5, 5.5, 13.5, 6.5, 13.5, 7.5, 13.5, 8.5, 13.5, 9.5, 13.5, 10.5, 13.5, 11.5, 13.5, 12.5, 13.5, 13.5 };
static list<Point> foods;
static vector<vector<bool>> bitmap; // 2d image of which squares are blocked and which are clear for pacman to move in 
bool* keyStates = new bool[256]; // record of all keys pressed 
int score = 0; // total points collected

struct Point {
	float x;
	float y;
};

struct Monster {
	float x;
	float y;
	int direction;
	float red;
	float green;
	float blue;

	//Method to draw the monster character through consecutive circles algorithm
	void draw() {
		glBegin(GL_LINES);
		glColor3f(red, green, blue);
		//draw the head
		int lineX, lineY;
		for (int k = 0; k < 32; k++){
			lineX = (float)k / 2.0 + (x*tileSize);
			lineY = (float)(y*tileSize);
			for (int i = 180; i <= 360; i++){
				glVertex2f(lineX, lineY);
				lineX = (float)k / 2.0 * cos(i * M_PI / 180.0) + (x*tileSize);
				lineY = (float)k / 2.0 * sin(i * M_PI / 180.0) + (y*tileSize);
				glVertex2f(lineX, lineY);
			}
		}
		glEnd();	
		//draw body
		glRectf((x*tileSize) - 17, y*tileSize, (x*tileSize) + 15, (y*tileSize) + 15);
		glBegin(GL_POINTS);
		glColor3f(0, 0.2, 0.4);
		//draw eyes and legs
		glVertex2f((x*tileSize) - 11, (y*tileSize) + 14); //legs
		glVertex2f((x*tileSize) - 1, (y*tileSize) + 14); //legs
		glVertex2f((x*tileSize) + 8, (y*tileSize) + 14); //legs
		glVertex2f((x*tileSize) + 4, (y*tileSize) - 3); //eyes
		glVertex2f((x*tileSize) - 7, (y*tileSize) - 3); //eyes 
		glEnd();
	}

	//Method to assign a new random direction
	void changeDirection() {
		int current = direction;
		do{
			direction = rand() % 4;
		} while (current == direction);
	}

	//Method to update the position of the monster
	void move() {
		//move him acording to its direction until he hits an obstacle
		float newX = x + (direction == 0) * (2/tileSize) - (direction == 2) * (2/tileSize);
		float newY = y + (direction == 3) * (2/tileSize) - (direction == 1) * (2/tileSize);
		int wallX = newX + (direction == 0) * (pacRadius/tileSize) - (direction == 2) * (pacRadius/tileSize);
		int wallY = newY + (direction == 3) * (pacRadius/tileSize) - (direction == 1) * (pacRadius/tileSize);
		if (!bitmap.at((int) wallX).at((int) wallY)){ 
				// no obstacle
				x = newX;
				y = newY;
			}else {
				changeDirection();
			}
	}
};


//Method to reset all the variables necessary to start the game again
void resetGame(){
	pacX = 1.5;
	pacY = 1.5; 
	pacRotation = 0;
	monsters = list<Monster>();
	monsters.push_back(Monster {13.5, 1.5, 0, 1.0, 0.0, 0.0});
	monsters.push_back(Monster {4.5, 6.5, 1, 1.0, 0.0, 0.6});
	monsters.push_back(Monster {10.5, 8.5, 2, 0.0, 1.0, 1.0});
	monsters.push_back(Monster {2.5, 13.5, 3, 1.0, 0.3, 0.0});
	score = 0;
	for (int i = 0; i < 256; i++){
		keyStates[i] = false;
	}
	
	foods = list<Point>();
	for (int i = 0; i < foodPositions.size(); i += 2) {
		foods.push_back(Point {foodPositions[i], foodPositions[i+1]});
	}
}

//Method to initialize the game with the appropiate information 
void init(void){
	//set initial game values
	resetGame();
	//clear screen
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	//reset all keys
	for (int i = 0; i < 256; i++){
		keyStates[i] = false;
	}
	//fill the bitmap with the obstacles
	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true});
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true});
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true});
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true});
	bitmap.push_back({ true, true, true, true, true, false, false, false, true, false, false, false, false, false, true});
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true });
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
}

//Method to draw the obstacle course and the walls
void drawMaze(){
	glColor3f(1.0, 1.0, 1.0);
	//Border
	for (int i = 0; i < border.size(); i = i + 4){
		glRectf(border.at(i) * tileSize, border.at(i + 1)*tileSize, border.at(i + 2)*tileSize, border.at(i + 3)*tileSize);
	}

	//Obstacles
	for (int j = 0; j < obstaclesBottom.size(); j = j + 4){
		glRectf(obstaclesBottom.at(j) * tileSize, obstaclesBottom.at(j + 1)*tileSize, obstaclesBottom.at(j + 2)*tileSize, obstaclesBottom.at(j + 3)*tileSize);
	}
	for (int k = 0; k < obstaclesMiddle.size(); k = k + 4){
		glRectf(obstaclesMiddle.at(k) * tileSize, obstaclesMiddle.at(k + 1)*tileSize, obstaclesMiddle.at(k + 2)*tileSize, obstaclesMiddle.at(k + 3)*tileSize);
	}
	for (int p = 0; p < obstaclesTop.size(); p = p + 4){
		glRectf(obstaclesTop.at(p) * tileSize, obstaclesTop.at(p + 1)*tileSize, obstaclesTop.at(p + 2)*tileSize, obstaclesTop.at(p + 3)*tileSize);
	}
}

//Method to check if the food has been eaten
bool foodEaten(Point &food){
	return (food.x >= pacX - pacRadius/tileSize && food.x <= pacX + pacRadius/tileSize) && (food.y >= pacY - pacRadius/tileSize && food.y <= pacY + pacRadius/tileSize);
}

//Method to draw all the food left and delete the ate one
void drawFoods(){
	// setup graphics drawing
	glPointSize(5.0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POINTS);
	for (auto food = foods.begin(); food != foods.end(); food++){
		if (foodEaten(*food)){
			// remove food and add score
			foods.erase(food);
			score++;
		} else {
			// draw food that has not been eaten
			glVertex2f(food->x * tileSize, food->y * tileSize);
		}
	}
	glEnd();
}

//Method to draw the pacman character through consicutive circle algorithm
void drawPacman(){
	int x, y;
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(pacX*tileSize, pacY*tileSize);
	for (int i = 30; i <= 330; i++){
		x = pacRadius * cos((i + 90*pacRotation) * M_PI / 180.0) + (pacX*tileSize);
		y = pacRadius * sin((i + 90*pacRotation) * M_PI / 180.0) + (pacY*tileSize);
		glVertex2f(x, y);
	}
	glEnd();
}

//Method to set the pressed key
void keyPressed(unsigned char key, int x, int y){
	keyStates[key] = true;
}

//Method to unset the released key
void keyUp(unsigned char key, int x, int y){
	keyStates[key] = false;
}

//Method to update the movement of the pacman according to the movement keys pressed
void movePacman(){
	//get current position
	float  x = (pacX) * tileSize;
	float y = (pacY) * tileSize;
	//update according to keys pressed
	if (keyStates['a']){
		x -= 2;
		int x1Quadrant = (int)((x - pacRadius) / tileSize);
		if (!bitmap.at(x1Quadrant).at((int)y/tileSize)){
			pacX -= 2 / tileSize;
			pacRotation = 2;
		}
	}
	else if (keyStates['d']){
		x += 2;
		int x2Quadrant = (int)((x + pacRadius) / tileSize);
		if (!bitmap.at(x2Quadrant).at((int)y / tileSize)){
			pacX += 2 / tileSize;
			pacRotation = 0;
		}
	}
	else if (keyStates['w']){
		y -= 2;
		int y1Quadrant = (int)((y - pacRadius) / tileSize);
		if (!bitmap.at((int)x/tileSize).at(y1Quadrant)){
			pacY -= 2 / tileSize;
			pacRotation = 3;
		}
	}
	else if (keyStates['s']){
		y += 2;
		int y2Quadrant = (int)((y + pacRadius) / tileSize);
		if (!bitmap.at((int)x / tileSize).at(y2Quadrant)){
			pacY += 2 / tileSize;
			pacRotation = 1;
		}
	}
}
//Method to check for key press to continue menu
void continueMenu(){
	if (keyStates[' ']){
		if (over){
			resetGame();
			replay = true;
			over = false;
		}
	}
}

//Method to check if the game is over
void gameOver(){
	for (Monster monster : monsters) {
		// lose the game (og pacman)
		if ((int)(pacX) == (int) monster.x && (int)(pacY) == (int) monster.y){
			over = true;
		}
	}
	if (score == 106){
		// win the game (og pacman)
		over = true;
	}
}

//Method to display the results of the game at the ends
void resultsDisplay(){
	if (score == 106){
		//Won
		const char* message = "*************************************";
		glRasterPos2f(170, 250);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
		message = "CONGRATULATIONS, YOU WON! ";
		glColor3f(1, 1, 1);
		glRasterPos2f(200, 300);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
		message = "*************************************";
		glRasterPos2f(170, 350);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
		message = "To start or restart the game, press the space key.";
		glRasterPos2f(170, 550);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch);
	}else {
		//Lost
		const char* message = "*************************";
		glRasterPos2f(210, 250);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
		message = "SORRY, YOU LOST ... ";
		glColor3f(1, 1, 1);
		glRasterPos2f(250, 300);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
		message = "*************************";
		glRasterPos2f(210, 350);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
		message = "You got: ";
		glRasterPos2f(260, 400);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
		string result = to_string(score);
		message = (char*)result.c_str();
		glRasterPos2f(350, 400);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
		message = " points!";
		glRasterPos2f(385, 400);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
		message = "To start or restart the game, press the space key.";
		glRasterPos2f(170, 550);
		for (const char* ch = message; *ch; ch++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch);
	}
}

//Method to display the starting instructions
void welcomeScreen(){
	glClearColor(0, 0.2, 0.4, 1.0);
	const char* message = "*************************************";
	glRasterPos2f(150, 200);
	for (const char* ch = message; *ch; ch++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
	message = "VAXMAN - modified by Omar Kalam";
	glColor3f(1, 1, 1);
	glRasterPos2f(225, 250);
	for (const char* ch = message; *ch; ch++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
	message = "*************************************";
	glRasterPos2f(150, 300);
	for (const char* ch = message; *ch; ch++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
	message = "To control Pacman use A to go right, D to go left, W to go up and S to go down.";
	glRasterPos2f(50, 400);
	for (const char* ch = message; *ch; ch++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch);
	message = "To start or restart the game, press the space key.";
	glRasterPos2f(170, 450);
	for (const char* ch = message; *ch; ch++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch);
}

//Method to update monsters and player position
void tick(){
	if (replay && !over) {
		for (Monster &monster : monsters) {
			monster.move();
		}
	}
}
//Method to display the screen and its elements
void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	gameOver();
	if (replay){
		if (!over){
			movePacman();
			tick();
			drawMaze();
			drawFoods();
			drawPacman();
			for (Monster &monster : monsters) {
				monster.draw();
			}
		}
		else {
			continueMenu();
			resultsDisplay();
		}
	}
	else {
		continueMenu();
		welcomeScreen();
	}
	glutSwapBuffers();
}

//Methdo to reshape the game is the screen size changes
void reshape(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glOrtho(0, 750, 750, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//Main functions that controls the running of the game
int main(int argc, char** argv){
	//initialize and create the screen
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(750, 750);
	glutInitWindowPosition(500, 50);
	glutCreateWindow("VAXMAN - modified by Omar Kalam");

	//define all the control functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);

	//run the game
	init();
	glutMainLoop();
	return 0;
}

