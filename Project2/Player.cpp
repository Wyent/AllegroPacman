#include "Player.h"
#include "Globals.h"
#include <sstream>

Player::Player(void)
{
	speed = 2;
	findWater = true;
}


Player::~Player(void)
{
}

void Player::movement()
{

	if(state!=2) {
		row = currentMove-1;
	}
	if(row==2)
		maxFrame = 5;
	else
		maxFrame = 3;

	if(row<0)
		row = 2;

	if(key[KEY_LEFT]){
		if(canMove(LEFT)){
			currentMove = LEFT;
			queuedMove = LEFT;
		}
		else
			queuedMove = LEFT;
	}
	else if(key[KEY_RIGHT]){
		if(canMove(RIGHT)){
			currentMove = RIGHT;
			queuedMove = RIGHT;
		}
		else
			queuedMove = RIGHT;
	}
	else if(key[KEY_UP]){
		if(canMove(UP)){
			currentMove = UP;
			queuedMove = UP;
		}
		else
			queuedMove = UP;
	}
	else if(key[KEY_DOWN]){
		if(canMove(DOWN)){
			currentMove = DOWN;
			queuedMove = DOWN;
		}
		else
			queuedMove = DOWN;
	}

	if(key[KEY_Q]){
		if(ability[0][0]){
			ability[0][1] = 1;
		}
	}
	else if(key[KEY_W]){
		if(ability[1][0]){
			ability[1][1] = 1;
		}
	}
	else if(key[KEY_E]){
		if(ability[2][0]){
			ability[2][1] = 1;
		}
	}
	else if(key[KEY_R]){
		if(ability[3][0]){
			ability[3][1] = 1;
		}
	}

	for(int destination = 0; destination < speed; destination++){
		if(canMove(queuedMove) && queuedMove != currentMove)
			currentMove = queuedMove;

		switch(currentMove){
		case UP:
			if(canMove(UP)) {
				y += -1;
				updateSprite();
			}
			break;
		case DOWN:
			if(canMove(DOWN)) {
				y += 1;
				updateSprite();
			}
			break;
		case LEFT:
			if(canMove(LEFT)) {
				x += -1;
				updateSprite();
			}
			break;
		case RIGHT:
			if(canMove(RIGHT)) {
				x += 1;
				updateSprite();
			}
			break;
		default:
			break;
		}

		//Check for the rock spell, if we hit it, destroy it.
		if(mapArray[(int)(x+16)/32][(int)(y+16)/32]==11) {
			destroyBlock((int)(x+16)/32,(int)(y+16)/32);
		}

		//Check for teleports
		if(mapArray[(int)x/32][(int)y/32]==6) {
			if(y<0) 
				y = BG->h-32;
			else if (y>SCREEN_H)
				y = 0;

			else if(x<0)
				x = BG->w-32;
			else if (x>BG->w-32)
				x = 0;

		}

	//Death, temporary
		if(state==2) {
			x = startX;
			y = startY;
			frameDelay = 20;
			currentMove = 0;
			queuedMove = 0;
			row = 2;
		}


	}
}

void Player::checkSpell(Enemies elemental[4]){
	for(int hotkey = 0; hotkey < 4; hotkey++)
		if(ability[hotkey][STATUS] == 1){
			if(ability[hotkey][TIMER] > -1)
				doAbility(ability[hotkey][TYPE], elemental, ability[hotkey][TIMER]);
			else{
				ability[hotkey][STATUS] = 0;
				ability[hotkey][TYPE] = 0;
				ability[hotkey][TIMER] = 0;
			}

			ability[hotkey][TIMER]--;
		}
}

void Player::doAbility(int ability, Enemies elemental[4], int &abilityTimer){
	switch(ability){
//////////////////////////////////////////////////////////
	case WIND:
		if(abilityTimer>0)
			speed = 8;
		else
			speed = 3;
		break;
//////////////////////////////////////////////////////////
	case ROCK:
		if(abilityTimer>0)
			switch(currentMove) {
			case UP:
				if(mapArray[(int)(x+16)/32][(int)(y+16)/32+1] != 1)
					putRock((int)(x+16)/32, (int)(y+16)/32+1);
				break;
			case LEFT:
				if(mapArray[(int)(x+16)/32+1][(int)(y+16)/32] != 1)
					putRock((int)(x+16)/32+1, (int)(y+16)/32);
				break;
			case DOWN:
				if(mapArray[(int)(x+16)/32][(int)(y+16)/32-1] != 1)
					putRock((int)(x+16)/32, (int)(y+16)/32-1);
				break;
			case RIGHT:
				if(mapArray[(int)(x+16)/32-1][(int)(y+16)/32] != 1)
					putRock((int)(x+16)/32-1, (int)(y+16)/32);
				break;
		}
		break;
//////////////////////////////////////////////////////////
	case WATER:
		int checkX, checkY;
		if(findWater){
			abilityTimer++;
			if((int)x % 32 == 0){
				for(checkX = x - 32; checkX <= x + 32; checkX += 32){
					if(findWater){
						for(checkY = y - 32; checkY <= y + 32; checkY += 32){
							if(mapArray[checkX/32][checkY/32] != 1 || mapArray[checkX/32][checkY/32] != 11){
								if(findIntersection(checkX, checkY)){
									mapArray[checkX/32][checkY/32] = 12;
									findWater = false;
									break;
								}
							}
						}
					}
				}
			}
		}
		else
			if(abilityTimer < 0){
				mapArray[checkX/32][checkY/32] = 0;
				findWater = true;
			}
			else
				putWater(checkX/32, checkY/32);
		break;
//////////////////////////////////////////////////////////
	case LIGHT:
		if(abilityTimer > 0){
			if(state == 2)
				state = 3;
		}
		else
			if(state == 3)
				state = 2;
			
		break;
//////////////////////////////////////////////////////////
	case METAL:
		if(abilityTimer>0)
			switch(currentMove) {
			case UP:
				destroyBlock((int)(x+16)/32, (int)(y+16)/32-1);
				break;
			case LEFT:
				destroyBlock((int)(x+16)/32-1, (int)(y+16)/32);
				break;
			case DOWN:
				destroyBlock((int)(x+16)/32, (int)(y+16)/32+1);
				break;
			case RIGHT:
				destroyBlock((int)(x+16)/32+1, (int)(y+16)/32);
				break;
		}
		break;
//////////////////////////////////////////////////////////
	case FIRE:
		break;
//////////////////////////////////////////////////////////
	case DARK:

		if(abilityTimer > 0){
			for(int i = 0; i < 4; i++)
				if(elemental[i].getState() < 2)
					elemental[i].vision = 256;
		}
		else
			for(int i = 0; i < 4; i++)
				if(elemental[i].getState() < 2)
					elemental[i].vision = 512;
				
		break;
//////////////////////////////////////////////////////////
	case SAND:
		break;
//////////////////////////////////////////////////////////
	case ELECTRIC:
		break;
//////////////////////////////////////////////////////////
	case EARTH:
		break;
//////////////////////////////////////////////////////////
	case POISON:
		break;
//////////////////////////////////////////////////////////
	case ICE:
		break;
//////////////////////////////////////////////////////////
	case NATURE:
		
		if(abilityTimer > 0){
			for(int i = 0; i < 4; i++)
				if(elemental[i].getState() < 2)
					elemental[i].speed = 0;
		}
		else
			for(int i = 0; i < 4; i++)
				if(elemental[i].getState() < 2)
					elemental[i].speed = elemental[i].aliveSpeed;
		break;
//////////////////////////////////////////////////////////
	}
}

bool Player::canMove(int direction, int x, int y)
{
	if(x < 0)
		x = this->x;
	if(y < 0)
		y = this->y;
	bool move = true;
	switch(direction){
	case(UP): 
		if((mapArray[(int)(x)/32][(int)(y-1)/32] == 1 || mapArray[(int)(x+32-1)/32][(int)(y-1)/32] == 1)) //adjust top side: left Y - 1, right Y - 1, right x - 1
			move = false;
		break;
	case(DOWN):
		if((mapArray[(int)(x)/32][(int)(y+32)/32] == 1 || mapArray[(int)(x+32-1)/32][(int)(y+32)/32] == 1)) //adjust bottom side: right X - 1
			move = false;
		break;
	case(LEFT):
		if((mapArray[(int)(x-1)/32][(int)(y)/32] == 1 || mapArray[(int)(x-1)/32][(int)(y+32-1)/32] == 1)) //adjust left side: X - 1, bottom Y - 1
			move = false;
		break;
	case(RIGHT):
		if((mapArray[((int)x+32)/32][(int)(y)/32] == 1 || mapArray[(int)(x+32)/32][(int)(y+32-1)/32] == 1)) //adjust right side: botom Y - 1
			move = false;
		break;
	}
	return move;
}
void Player::destroyBlock(int x, int y) {

	mapArray[x][y] = 0;
	std::stringstream floor;

	BITMAP* Floor;
	int num2 = rand()%2+1;
	floor << "Graphics//Level" << theLevel << "//FloorTile" << num2 << ".bmp";
	Floor = load_bitmap(floor.str().c_str(), 0);

	masked_blit(Floor, BG, 0, 0, x*32, y*32, Floor->w, Floor->h);
	destroy_bitmap(Floor);
	Floor = nullptr;
}

void Player::putRock(int x, int y) {

	mapArray[x][y] = 11;
	std::stringstream rock;

	BITMAP* Rock;
	int num2 = rand()%2+1;
	rock << "Graphics//spells//Rock.bmp";
	Rock = load_bitmap(rock.str().c_str(), 0);

	masked_blit(Rock, BG, 0, 0, x*32, y*32, Rock->w, Rock->h);
	destroy_bitmap(Rock);
	Rock = nullptr;
}
void Player::putWater(int x, int y) {

	mapArray[x][y] = 12;
	std::stringstream water;

	BITMAP* Water;
	int num2 = rand()%2+1;
	water << "Graphics//spells//Water.bmp";
	Water = load_bitmap(water.str().c_str(), 0);

	masked_blit(Water, BG, 0, 0, x*32, y*32, Water->w, Water->h);
	destroy_bitmap(Water);
	Water = nullptr;
}
bool Player::findIntersection(int checkX, int checkY){
	int intersection = 0;
	int tempDirection = 0;
	/*if(!(int)x % 32){
		for(int checkX = x - 32; checkX <= x + 32; checkX += 32){
			for(int checkY = y - 32; checkY <= y + 32; checkY += 32){
				if(mapArray[checkX/32][checkY/32] != 1 || mapArray[checkX/32][checkY/32] != 11){*/
					for(int direction = UP; direction <= RIGHT; direction++){
						if(abs(direction - tempDirection) % 2 != 0 || tempDirection == 0){
							if(canMove(direction, checkX, checkY)){
								intersection++;
								if(intersection > 1){
									return true;
								}
								tempDirection = direction;
							}
						}
					}
				/*}
			}
		}
	}*/
					return false;
}