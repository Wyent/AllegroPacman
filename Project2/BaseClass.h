#pragma once


#include <allegro.h>
#include "Globals.h"
class BaseClass
{
public:
	double x, y;
	int excessX, excessY;
	int height, width;
	int trueH, trueW;
	short state;// states of life 0 = killable 1 = invincible 2 = dead
	//animations
	BITMAP* sprite;
	int animStartX, animStartY;
	int animColumns; // number of collumns in the sprite sheet
	int frameCount;
	int frameDelay;
	int curFrame;
	int maxFrame;
	int row;

	BaseClass(void);
	~BaseClass(void);

	void loadSprite(const char* filename, int newW, int newH, int newMaxFrame, int newAnimColumns);
	void updateSprite();
	void drawFrame(BITMAP* dest);
};

