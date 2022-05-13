#include "BaseClass.h"


BaseClass::BaseClass(void)
{
}


BaseClass::~BaseClass(void)
{
	animStartX = 0;
	animStartY = 0;
	animColumns = 1;
	frameCount = 0;
	frameDelay = 50;
	curFrame = 0;
	maxFrame = 0;
	row = 0;

}

void BaseClass::loadSprite (const char* filename, int newW, int newH, int newMaxFrame, int newAnimColumns)
{
	
		sprite = load_bitmap(filename, 0);

		if (sprite != 0)
		{

			width  = newW;
			height = newH;
			if (maxFrame != -1)
				maxFrame = newMaxFrame;
			if (animColumns != -1)
				animColumns = newAnimColumns;

		}

		//set to glitch sprite
		else
			allegro_message("eRRROR loadind platform sprite");
}

void BaseClass::updateSprite() {
	frameCount--;
	if(frameCount<0){
		frameCount = frameDelay;
		curFrame++;
		if(curFrame>maxFrame)
			curFrame = 0;
	}

	
}
void BaseClass::drawFrame(BITMAP* dest)
{

     int fX = animStartX + (curFrame % animColumns) * width;
     int fY = animStartY + (row) * height;

	 masked_blit( sprite, dest, fX, fY, x, 
                 y, width, height);

}