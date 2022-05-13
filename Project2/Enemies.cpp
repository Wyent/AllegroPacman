#include "Enemies.h"
#include "Globals.h"
#define ENEMIES 4
Enemies::Enemies(void)
{
	Movers();
	//srand(time(NULL));
	state = 1;
	aliveSpeed=2;
	speed = aliveSpeed;
	currentMove = UP;
	queuedMove =  rand()%4+1;
	flee = false;
	turnAround = false;
	moveTimer = 0;
	flashing = false;
	vision = 512;
	type = WATER;
}


Enemies::~Enemies(void)
{
}

void Enemies::movement(int targetX, int targetY, Enemies elementals[4])
{
		
	this->targetX = targetX;
	this->targetY = targetY;
	for(int destination = 0; destination < speed; destination++){
		//////////////////////////////////////////////////////////////////////////////////////
		if(getDistance(x, y, targetX, targetY) < vision || state==2) {
			if(!flee)
				roam = false;
		}
		else
			roam = true;
		if(!roam){
			if((int)x%32==0 && (int)y%32==0) //Problem was here, kept missing the checkpoint
				queuedMove = findSmallestDist(elementals);
			if(!canMove(currentMove, elementals) && !canMove(queuedMove, elementals) && !roam)
				roam=true;
		}
		//////////////////////////////////////////////////////////////////////////////////////
		//Fleeing from player
		if(state!=2) { //If not eaten
			if(flee) { //If can flee
				if(turnAround) { //If can turn around
					if(canMove(currentMove+2, elementals)) { //If we can turn around without trouble
						currentMove+=2; //do a U-turn
						if(currentMove>4) //Handles invalid direction
							currentMove = 1;
						queuedMove=currentMove; //Set queued move to our current move
					}
					turnAround = false; //Don't turn around again
					state = 0; //Make us vulnerable
				}
			} else { //If not fleeing
				state = 1; //We can attack
				 //The number of frames is the image width/32 minus one
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		//Going back to the box (buggy depending on the level's layout)
		else if(state==2) { //If we got eaten or we are in the box
			this->targetX = enemyStart[ID][0]; //Our targetX is the x we started at
			this->targetY = enemyStart[ID][1]; //Our targetY is the y we started at
			if(x/32 == enemyStart[0][0]/32 && y/32 == (enemyStart[0][1]-32)/32) { //If we reached our destination
				x = enemyStart[2][0]; //Manually set the x and y to the middle of the box; this eliminates the problem of enemies getting stuck.
				y = enemyStart[2][1];
				currentMove = UP; //Make it so that our current and next moves are to get out of the box
				queuedMove = UP;
				moveTimer = 100; //Don't move for 100 loops
				state = 1; //Make us able to attack
				speed = aliveSpeed; //Use our normal speed
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		if(state !=2) { //If not dead,
			if(mapArray[int(x)/32][int(y)/32]==-9) { //If we are in the box 
				this->targetX = enemyStart[ID][0]; //Our targetX is the x outside the box
				this->targetY = enemyStart[ID][1]; //Our targetY is the y outside the box
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////
		if(roam || flee){
			while(abs(currentMove - queuedMove) % 2 == 0) //while queuedMove and currentMove are the same/opposite direction
				queuedMove = rand()%4+1;

			if(!canMove(currentMove, elementals) && !canMove(queuedMove, elementals)){ //unable to move in current or queued direction
				int availableMoves = 0;

				for(int checkDirection = 1; checkDirection < 5; checkDirection++) //check for valid directions
					if(canMove(checkDirection, elementals))
						availableMoves++;

				if(availableMoves > 1)
					do{ //found more than 1 direction, make a new random direction to travel that isn't opposite direction
						queuedMove = rand()%4+1;
					}while(!canMove(queuedMove, elementals) || abs(currentMove - queuedMove) == 2);
				else //could only find 1 direction, make queuedMove the opposite direction*/
					if(currentMove < 3)
						queuedMove = currentMove + 2;
					else
						queuedMove = currentMove - 2;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		//Sets the animation depending on the state
		switch(state) {
		case 0:
			row = 1;
			if(!flashing)
				row = 1; //Use flee animation
			else
				row = 0;
			maxFrame = (sprite->w/32)-1; //Frames of animation is the sprite's width/32 minus one
			break;
		case 1:
			row = 0; //Use normal animation
			maxFrame = (sprite->w/32)-1; //Frames of animation is the sprite's width/32 minus one
			break;
		case 2:
			row = 2; //Use the eyeballs
			maxFrame = 0; //One frame of animation
			curFrame = 0; //Set frame to first frame
			break;
		}
		//////////////////////////////////////////////////////////////////////////////////////
		if(canMove(queuedMove, elementals))
			if(mapArray[int(x)/32][int(y)/32] != 12)
				currentMove = queuedMove;

		if(canMove(currentMove,elementals))
			switch(currentMove){
			case UP:
					y += -1;
				break;
			case DOWN:
					y += 1;
				break;
			case LEFT:
					x += -1;
				break;
			case RIGHT:
					x += 1;
				break;
			default:
				break;
			}
		//////////////////////////////////////////////////////////////////////////////////////
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
	}
}

bool Enemies::canMove(int direction, Enemies elemental[4])
{
	bool move = true;
	switch(direction){
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case(UP): 
		if((mapArray[(int)(x)/32][(int)(y-1)/32] == 1 || mapArray[(int)(x+32-1)/32][(int)(y-1)/32] == 1) || (mapArray[(int)(x)/32][(int)(y-1)/32] == 11 || mapArray[(int)(x+32-1)/32][(int)(y-1)/32] == 11)) //adjust top side: left Y - 1, right Y - 1, right x - 1
			move = false;
		else
			for(int enemyCheck = 0; enemyCheck < ENEMIES; enemyCheck++)
				if(!(y == elemental[enemyCheck].y && x == elemental[enemyCheck].x) && state!=2 && (mapArray[int(x)/32][int(y)/32]!=-9))
					if(y == elemental[enemyCheck].y+32
						&& ((x >= elemental[enemyCheck].x && x < elemental[enemyCheck].x+32) || (x+32 > elemental[enemyCheck].x && x+32 <= elemental[enemyCheck].x+32)))
					{
						move = false;
						break;
					}
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case(DOWN):
		if((mapArray[(int)(x)/32][(int)(y+32)/32] == 1 || mapArray[(int)(x+32-1)/32][((int)y+32)/32] == 1) || (mapArray[(int)(x)/32][(int)(y+32)/32] == 11 || mapArray[(int)(x+32-1)/32][((int)y+32)/32] == 11)) //adjust bottom side: right X - 1
			move = false;
		else
			for(int enemyCheck = 0; enemyCheck < ENEMIES; enemyCheck++)
				if(!(y == elemental[enemyCheck].y && x == elemental[enemyCheck].x)  && state!=2 && (mapArray[int(x)/32][int(y)/32]!=-9))
					if(y+32 == elemental[enemyCheck].y 
						&& ((x >= elemental[enemyCheck].x && x < elemental[enemyCheck].x+32) || (x+32 > elemental[enemyCheck].x && x+32 <= elemental[enemyCheck].x+32)))
						move = false;
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case(LEFT):
		if((mapArray[((int)x-1)/32][((int)y)/32] == 1 || mapArray[((int)x-1)/32][((int)y+32-1)/32] == 1) || (mapArray[((int)x-1)/32][((int)y)/32] == 11 || mapArray[((int)x-1)/32][((int)y+32-1)/32] == 11)) //adjust left side: X - 1, bottom Y - 1
			move = false;
		else
			for(int enemyCheck = 0; enemyCheck < ENEMIES; enemyCheck++)
				if(!(y == elemental[enemyCheck].y && x == elemental[enemyCheck].x)  && state!=2 && (mapArray[int(x)/32][int(y)/32]!=-9))
					if(x == elemental[enemyCheck].x+32 
						&& ((y >= elemental[enemyCheck].y && y < elemental[enemyCheck].y+32) || (y+32 > elemental[enemyCheck].y && y+32 <= elemental[enemyCheck].y+32)))
						move = false;
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case(RIGHT):
		if((mapArray[((int)x+32)/32][((int)y)/32] == 1 || mapArray[((int)x+32)/32][((int)y+32-1)/32] == 1) || (mapArray[((int)x+32)/32][((int)y)/32] == 11 || mapArray[((int)x+32)/32][((int)y+32-1)/32] == 11)) //adjust right side: bottom Y - 1
			move = false;
		else
			for(int enemyCheck = 0; enemyCheck < ENEMIES; enemyCheck++)
				if(!(y == elemental[enemyCheck].y && x == elemental[enemyCheck].x)  && state!=2 && (mapArray[int(x)/32][int(y)/32]!=-9))
					if(x+32 == elemental[enemyCheck].x 
						&& ((y >= elemental[enemyCheck].y && y < elemental[enemyCheck].y+32) || (y+32 > elemental[enemyCheck].y && y+32 <= elemental[enemyCheck].y+32)))
						move = false;
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	return move;
}

int Enemies::findSmallestDist(Enemies elementals[]) {

	//Okay guys, try to follow me here..
	//This array stores the distances of each point
	double dist[4] = {0};
	if(canMove(UP, elementals) && abs(currentMove - UP) != 2){
		dist[0] = getDistance(x+16, y-16, targetX, targetY); //UP
	}
	else{
		dist[0] = 0;
	}

	if(canMove(LEFT, elementals) && abs(currentMove - LEFT) != 2){
		dist[1] = getDistance(x-16, y+16, targetX, targetY); //LEFT
	}
	else{
		dist[1] = 0;
	}

	if(canMove(DOWN, elementals) && abs(currentMove - DOWN) != 2){
		dist[2] = getDistance(x+16, y+48, targetX, targetY); //DOWN
	}
	else{
		dist[2] = 0;
	}

	if(canMove(RIGHT, elementals) && abs(currentMove - RIGHT) != 2){
		dist[3] = getDistance(x+48, y+16, targetX, targetY); //RIGHT
	}else{
		dist[3] = 0;
	}

	//The smallest distance array indices are parallel to the directions defined above.
	//Therefore, we can use this little trick to find the direction of the smallest distance.
	//We will also filter out the "reversed direction" in the results

	//We will use the first element to compare to the others

	//if run into pocket
	//current move = opposite
	//queued = rand not opposite, not same
	//when current becomes queued detect again
	int smallest = 513;
	int dir = currentMove;
	for(int i = 0; i<4; i++)
		if(dist[i] < smallest && dist[i] != 0){
			//if(abs(dir-(i+1)) != 2) {
			smallest = dist[i];
			dir = i+1;
			//}
		}
		/*if(currentMove == dir){
			if(dir < 3)
				dir = currentMove + 2;
			else
				dir = currentMove - 2; 
			//dir = currentMove;
		}*/
	return dir;
}

void Enemies::updateEnemies(Enemies elemental[],int targetX, int targetY) { 
		for(int enemy = 0; enemy < ENEMIES; enemy++) {
			elemental[enemy].moveTimer--;
			if(elemental[enemy].moveTimer<0) {
				if(elemental[enemy].state!=2)
					elemental[enemy].movement(targetX, targetY, elemental);
				else
					elemental[enemy].movement(enemyStart[0][0], enemyStart[0][1]-32, elemental);
			}
			elemental[enemy].updateSprite();
		}
}

void Enemies::drawEnemies(Enemies elemental[], BITMAP* dest) {
		for(int enemy = 0; enemy < ENEMIES; enemy++)
				elemental[enemy].drawFrame(dest);
}