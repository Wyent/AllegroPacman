#include "Globals.h"
#include <cmath>
#include <sstream>
#define ENEMIES 1
short mapArray[40][32] = {-1};
int startX = 0;
int startY = 0;
int enemyStart[4][2] = {0};
int score = 0;
int newScore = 0;
int lives = 3;
int theLevel = 1;
BITMAP* BG;
BITMAP* numberArray[10];
void createNumbers() {
	for(int i = 0; i<10; i++) {
		std::stringstream s;
		s<<"Graphics//HudIcons//Number" << i <<".bmp";
		numberArray[i] = load_bitmap(s.str().c_str(), 0);
	}
}

double getDistance(double x1, double y1, double x2, double y2){
	return std::sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}