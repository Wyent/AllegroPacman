#include "World.h"
#include "PowerUps.h"
#include "BaseClass.h"
#include <sstream>
#define ENEMIES 4
World::World(void)
{
	numKilled = 0;
	invulnTime = 0;
	screenX = 0;
	screenY = 0;
	bonusTimer = 0;
	numBonuses = 0;
	LOCK_VARIABLE(ticker);
	LOCK_FUNCTION(invul_timer);
}

World::~World(void)
{
	destroy_bitmap(pellet);
	pellet = nullptr;
	destroy_bitmap(powerPellet);
	powerPellet = nullptr;
	destroy_bitmap(bonus);
	bonus = nullptr;
}

bool World::loadLevel(int level, Enemies elemental[ENEMIES]) {
	//Clear out the world array
	for(int i = 0; i<32; i++) 
		for(int j = 0; j<40; j++)
			mapArray[i][j] = 0;

	//set all enemies to chase
	for(int i = 0; i<ENEMIES; i++)
		elemental[i].setState(1);
	//Set all enemy move delays
	elemental[0].moveTimer = 50;
	elemental[1].moveTimer = 150;
	elemental[2].moveTimer = 100;
	elemental[3].moveTimer = 200;

	numPellets = 0; //Set number of pellets to zero
	pelletsLeft = 0; //Set number of pellets to go to zero
	numBonuses = 0; //Reset the number of bonuses
	//Set level to zero if you don't want to use bitmaps (debug)
	std::stringstream levelString;
	levelString << "Graphics//Level" << level << "//Level.bmp";
	srand(time(NULL));
	this->level = level;
	BITMAP* wall;
	BITMAP* floor;
	BITMAP* warp;
	int counter = 0;
	warp = load_bitmap("Graphics//warping tunnel.bmp", 0);
	std::stringstream pelletString;
	pelletString << "Graphics//Level" << level << "//pacdot" << ".bmp";
	pellet = load_bitmap(pelletString.str().c_str(), 0);

	std::stringstream pPelletString;
	pPelletString << "Graphics//Level" << level << "//powerPellet" << ".bmp";
	powerPellet = load_bitmap(pPelletString.str().c_str(), 0);

	std::stringstream bonusString;
	bonusString << "Graphics//Level" << level << "//Bonus" << ".bmp";
	bonus = load_bitmap(bonusString.str().c_str(), 0);

	BG = create_bitmap(40*32, 32*32);
	clear_to_color(BG, 0xFFFFFF);
	//for(int i = 0; i<BG->h; i++)
	//	vline(BG, i*32, 0, BG->h, 0xFF0000);
	//for(int j = 0; j<BG->w; j++) 
	//	hline(BG, 0, j*32, BG->w, 0xFF0000);

	//This will handle loading the assets for each level
	//automatically, just be sure the folders stay the same.
	BITMAP* temp;
	if(level==0) {
		wall = create_bitmap(32, 32);
		clear_to_color(wall, 0x000000);
		floor = create_bitmap(32, 32);
		clear_to_color(wall, 0xFFFFFF);
	}

	temp = load_bitmap(levelString.str().c_str(), 0);
	if(!temp) {
		destroy_bitmap(temp);
		temp = nullptr;
		return false;
	}
	else {
		for(int j = 0; j<temp->h; j++)
			for(int i = 0; i<temp->w; i++) {
				if(level!=0) {
					std::stringstream myString;
					std::stringstream myString2;

					int num = rand()%2+1;
					myString << "Graphics//Level" << level << "//Tile" << num << ".bmp";

					int num2 = rand()%2+1;
					myString2 << "Graphics//Level" << level << "//FloorTile" << num2 << ".bmp";

					wall = load_bitmap(myString.str().c_str(), 0);
					floor = load_bitmap(myString2.str().c_str(), 0);
				}
				masked_blit(floor, BG, 0, 0, i*32, j*32, wall->w, wall->h);

				int col = getpixel(temp, i, j);
				switch(col) {
				case 0x000000: //black
					//rectfill(BG, i*32, j*32, (i*32)+32, (j*32)+32, 0x000000);
					mapArray[i][j] = 1;
					masked_blit(wall, BG, 0, 0, i*32, j*32, wall->w, wall->h);
					break;
				case 0xFF0000: //red
					startX = i*32;
					startY = j*32;
					break;
				case 0xFFFFFF: //white
					//Make a normal pellet here
					mapArray[i][j] = 2;
					numPellets++;
					pelletsLeft++;
					break;
				case 0x00FF00: //green
					//Make a power pellet here
					mapArray[i][j] = 3;
					break;
				case 0x00FFFF: //cyan
					mapArray[i][j] = 6;
					masked_blit(warp, BG, 0, 0, i*32, j*32, warp->w, warp->h);
					break;

				case 0xFFFF00: //gold
					enemyStart[counter][0] = i*32;
					enemyStart[counter][1] = j*32;
					mapArray[i][j] = -9;
					++counter;
					break;
				} //End switch
			} //End for

			destroy_bitmap(temp);
			temp = nullptr;
			destroy_bitmap(wall);
			wall = nullptr;
			destroy_bitmap(floor);
			floor = nullptr;
			destroy_bitmap(warp);
			warp = nullptr;
			return true;

	} //End else

}

void World::followTarget(int targetx, int targety, int speedFactor) {

	if(-screenX < BG->w-SCREEN_W) {
		if(targetx > -screenX)
			screenX-=(targetx+screenX)/speedFactor;
	} else
		screenX = -(BG->w-SCREEN_W);

	if(-screenX > 0) {
		if(targetx < -screenX)		
			screenX+=(-screenX-targetx)/speedFactor;
	} else
		screenX = 0;

	if(-screenY >0) {
		if(-screenY > targety)
			screenY+=(-screenY-targety)/speedFactor;
	} else
		screenY = 0;

	if(-screenY < BG->h-SCREEN_H+80) {
		if(-screenY < targety)
			screenY-=(targety+screenY)/speedFactor;
	} else
		screenY = -(BG->h-SCREEN_H+80);
}



/*This handles pellets, scoring, player/enemy collisions, and other various
routines in the level. Cleans up the main quite a bit*/
void World::updateWorld(Player* thePlayer, Enemies elemental[ENEMIES]) {

	//David, forgot this little bit of code, enjoy eating pellets.
	if(thePlayer->getValueAtPosition()==2) { //If there is a pellet at player's position,
		newScore+=10; //Up the score by 10
		pelletsLeft--; //Decrement the number of pellets remaining
		thePlayer->setValueAtPosition(0); //Get rid of the pellet
	}
	//Power Pellets
	if(thePlayer->getValueAtPosition()==3) { //If we hit a pellet,
		newScore+=50;	//Up the score
		thePlayer->setValueAtPosition(0);	//Get rid of the pellet
		thePlayer->setState(-1); //Trip the player's invulnerable state
	}

	//Bonus Items
	if(thePlayer->getValueAtPosition()==7) { //If there is a bonus at player's position,
		newScore+=1600; //Up the score by 1600
		thePlayer->setValueAtPosition(0); //Get rid of the bonus
		bonusTimer = 0; //Set bonus timer to zero for safe measure
	}

	//Invulnerability handling
	if(thePlayer->getState()==-1) { //If we ate a power pellet, use a temp state
		setInvuln(); //Start invulerability timer
		thePlayer->setState(1); //We can eat enemies
		for(int i = 0; i<ENEMIES; i++) { //Go thru each enemy,
			elemental[i].Flee(); //and make them turn around
			elemental[i].flashing = false;
		}
	} else if(thePlayer->getState() == 1) //If we can eat enemies,
		invul_timer(); //Decrement invulerability timer

	//Flashing enemies when timer is low
	if(isInvuln()) { //If player is invulnerable,
		if(invulnTime<150) { //If invunerability timer is less than 150,
		for(int i = 0; i<ENEMIES; i++) //For each enemy,
			if(invulnTime%15==0) { //Every time the timer mod 15 is zero,
				elemental[i].flashing = true; //Use the normal animation
			} else if(invulnTime%15==7) { //And every time the timer mod 7 is zero,
				elemental[i].flashing = false; //Use vulnerable animation
			}
		}
	}

	else if(!isInvuln()) { //If the power pellet timer is no longer active,
		for(int i = 0; i<ENEMIES; i++) //For each enemy,
			elemental[i].flee = false; //Don't flee
		numKilled=0; //Reset number of enemies killed
		thePlayer->setState(0); //Make player vulnerable again
	}

	//Check Collisions
	for(int i  = 0; i<ENEMIES; i++) { //For each enemy,
		if(getDistance(thePlayer->x, thePlayer->y, elemental[i].x, elemental[i].y) <16) { //If player collided with us,
			if(elemental[i].getState() < thePlayer->getState()) { //If enemy's state was less than wizard's
				numKilled++; //Increment number of enemies eaten
				newScore+=pow(2.0, numKilled-1)*100; //Adjust score accordingly
				elemental[i].setState(2); //Enemy is now dead
				elemental[i].speed = 8; //Go to the box at a(much) faster speed
				for(int spell = 0; spell < 4; spell++){
					if(thePlayer->ability[spell][0] == 0){
						switch(elemental[i].type){
						case WIND:
							thePlayer->ability[spell][2] = 120;
							break;
						case ROCK:
							thePlayer->ability[spell][2] = 1;
							break;
						case WATER:
							thePlayer->ability[spell][2] = 120;
							break;
						case LIGHT:
							thePlayer->ability[spell][2] = 120;
							break;
						case METAL:
							thePlayer->ability[spell][2] = 1;
							break;
						case FIRE:
							thePlayer->ability[spell][2] = 120;
							break;
						case DARK:
							thePlayer->ability[spell][2] = 120;
							break;
						case SAND:
							thePlayer->ability[spell][2] = 120;
							break;
						case ELECTRIC:
							thePlayer->ability[spell][2] = 120;
							break;
						case EARTH:
							thePlayer->ability[spell][2] = 120;
							break;
						case POISON:
							thePlayer->ability[spell][2] = 120;
							break;
						case ICE:
							thePlayer->ability[spell][2] = 120;
							break;
						case NATURE:
							thePlayer->ability[spell][2] = 120;
							break;
						}
						thePlayer->ability[spell][0] = elemental[i].type;
						break;
					}
				}
			} else if(elemental[i].getState() > thePlayer->getState() && elemental[i].getState()!=2) { //If enemy's state was greater than wizard's state AND we aren't dead,
				thePlayer->setState(2); //Kill wizard
				for(int i = 0; i<ENEMIES; i++)
					elemental[i].speed = 0;
				resetWorld(thePlayer, elemental);
			}
		}
	}
}
//Draws the game world
void World::drawWorld(BITMAP* dest) {
	bonusTimer--; //Decrement the bonus' timer
	//Okay, this handles showing the level's bonus item at the correct time
	if(((pelletsLeft == (int)(numPellets*.3)) && numBonuses < 3)) { //Bonus at 1/3 dots left
		numBonuses++;
		mapArray[startX/32][startY/32] = 7; //Set the starting position to a bonus code.
		bonusTimer = 600; //Set bonus timer to 10 seconds
	} else if((pelletsLeft ==  (int)(numPellets*.6) && numBonuses < 2)) { //Bonus at 2/3 dots left
		numBonuses++;
		mapArray[startX/32][startY/32] = 7; //Set the starting position to a bonus code.
		bonusTimer = 600; //Set bonus timer to 10 seconds
	}
	if(bonusTimer < 0) //Don't show the bonus if the bonus timer runs out
		mapArray[startX/32][startY/32] = 0;

				//Put pellets on-buffer
			for(int i = 0; i < 40; i++)
			{
				for(int j = 0; j < 32; j++)
				{
					if(mapArray[i][j] == 2)
						//draw pellet
					masked_blit(pellet, dest, 0, 0, i*32, j*32, pellet->w, pellet->h);
					else if(mapArray[i][j] == 3)
						//draw power pellet
					masked_blit(powerPellet, dest, 0, 0, i*32, j*32, powerPellet->w, powerPellet->h);
					else if(mapArray[i][j] == 7) {
							masked_blit(bonus, dest, 0, 0, startX, startY, bonus->w, bonus->h);
					}
				} 

			}

}

void World::resetWorld(Player* thePlayer, Enemies elemental[]) {
	for(int i = 0; i<ENEMIES; i++)
		elemental[i].speed = elemental[i].aliveSpeed;

	thePlayer->x = startX;
	thePlayer->y = startY;
	for(int enemy = 0; enemy < ENEMIES; enemy++){
		elemental[enemy].ID = enemy; //Give each enemy an ID
		//elemental[enemy].x = enemyStartX-(32/**enemy*/);	//(enemyStartX -32) * 2 -32 -32;
		elemental[enemy].x = enemyStart[enemy][0];	//(enemyStartX -32) * 2 -32 -32;
		elemental[enemy].y = enemyStart[enemy][1];	//(enemyStartX -32) * 2 -32 -32;
	}
	elemental[0].moveTimer = 50;
	elemental[1].moveTimer = 150;
	elemental[2].moveTimer = 100;
	elemental[3].moveTimer = 200;

}