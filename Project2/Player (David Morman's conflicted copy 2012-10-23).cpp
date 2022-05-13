#include "Player.h"
#include "Globals.h"


Player::Player(void)
{
	speed = 3;
}


Player::~Player(void)
{
}

void Player::movement()
{
	//Limit off-map movement
	//if(x < 32)
	//	x = 32;
	//
	//if(y < 32)
	//	y = 32;
	//	
	//if(x > BG->w-64)
	//	x = BG->w-64;
	//
	//if(y > BG->h-64)
	//	y = BG->h-64;

	if(key[KEY_LEFT] || key[KEY_A])
	{
		// -X SPEED
			x += -speed;
			if(mapArray[x/32][y/32]==1) {
				x+=speed;
			}
	}

	else if(key[KEY_RIGHT] || key[KEY_D])
	{
		// +X SPEED
			x += speed;
			if(mapArray[x/32+1][y/32]==1) {
				x-=speed;
			}
	}

	else if(key[KEY_UP] || key[KEY_W])
	{
		// -Y SPEED
			y += -speed;
			if(mapArray[x/32][y/32]==1) {
				y+=speed;
			}
	}

	else if(key[KEY_DOWN] || key[KEY_S])
	{
		// +Y SPEED
			y += speed;
			if(mapArray[x/32][y/32+1]==1) {
				y-=speed;
			}
	}
}

