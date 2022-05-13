#include "World.h"
#include "PowerUps.h"
#include "BaseClass.h"

World::World(void)
{
	screenX = 0;
	screenY = 0;
}


World::~World(void)
{
}

bool World::loadLevel(char* path) {
	BG = create_bitmap(40*32, 32*32);
	clear_to_color(BG, 0x000000);
	for(int i = 0; i<BG->h; i++)
		vline(BG, i*32, 0, BG->h, 0xFF0000);
	for(int j = 0; j<BG->w; j++) 
		hline(BG, 0, j*32, BG->w, 0xFF0000);

		
	BITMAP* temp;
	temp = load_bitmap(path, 0);
	if(!temp) {
		destroy_bitmap(temp);
		temp = nullptr;
		return false;
	}
	else {
		for(int j = 0; j<temp->h; j++)
			for(int i = 0; i<temp->w; i++) {
				int col = getpixel(temp, i, j);
				switch(col) {

				case 0x000000:
					//rectfill(BG, i*32, j*32, (i*32)+32, (j*32)+32, 0x000000);
					mapArray[i][j] = 1;
					rectfill(BG, i*32, j*32, i*32+32, j*32+32, 0x00FF00);
					break;
				case 0xFF0000:
					startX = i*32;
					startY = j*32;
					break;
				case 0xFFFFFF:
					//Make a normal pellet here
						//pellet.loadSprite("Graphics//pellet1.bmp", 32, 32, 0, 1);
					//powerUps->powerUpsMain();
					mapArray[i][j] = 2;
					break;
				case 0x00FF00:
					//Make a power pellet here
						//powerPellet.loadSprite("Graphics//powerPellet.bmp", 32, 32, 0, 1);
					mapArray[i][j] = 3;
					break;
				case 0xFFFF00:
					//enemy spawn
					break;
				case 0x00FFFF:
					//Teleporter spawn
					break;
				} //End switch
			} //End for

			destroy_bitmap(temp);
			temp = nullptr;
			return true;
			
	} //End else
}

void World::followTarget(int targetx, int targety) {

	if(targetx > -screenX)
		if(-screenX < BG->w-SCREEN_W)
			screenX-=(targetx+screenX);

	if(targetx < -screenX)
		if(-screenX > 0)
			screenX+=(-screenX-targetx);

	if(-screenY > targety)
		if(-screenY >0)
			screenY+=(-screenY-targety);

	if(-screenY < targety)
		if(-screenY < BG->h-SCREEN_H)
			screenY-=(targety+screenY);

}