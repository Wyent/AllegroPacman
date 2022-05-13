#pragma once
#include <allegro.h>
extern short mapArray[40][32];
extern int startX;
extern int startY;
extern int enemyStart[4][2];
extern int score;
extern int newScore;
extern int lives;
extern int theLevel;
extern BITMAP* BG;
extern BITMAP* numberArray[10];
void createNumbers();

double getDistance(double, double, double, double);