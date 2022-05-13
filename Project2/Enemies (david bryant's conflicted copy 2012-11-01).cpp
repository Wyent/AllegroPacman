#include "Enemies.h"
#include "Globals.h"
#define ENEMIES 4
Enemies::Enemies(void)
{
	//srand(time(NULL));
	state = 1;
	speed = 4;
	currentMove = UP;
	queuedMove =  rand()%4+1;
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
		if(getDistance(x, y, targetX, targetY) < 512)
			roam = false;
		else
			roam = true;
		if(!roam){
			if(x%32==0 && y%32==0) //Problem was here, kept missing the checkpoint
				queuedMove = findSmallestDist(elementals);
			if(!canMove(currentMove, elementals) && !canMove(queuedMove, elementals) && !roam)
				roam=true;
		}
		//////////////////////////////////////////////////////////////////////////////////////
		if(roam){
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
		if(canMove(queuedMove, elementals))
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

		if(mapArray[x/32][y/32]==6) {
			if(y<0)
				y = BG->h-32;
			else if (y>SCREEN_H-31)
				y = 0;
		}
	}
}

bool Enemies::canMove(int direction, Enemies elemental[4])
{
	bool move = true;
	switch(direction){
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case(UP): 
		if((mapArray[(x)/32][(y-1)/32] == 1 || mapArray[(x+32-1)/32][(y-1)/32] == 1)) //adjust top side: left Y - 1, right Y - 1, right x - 1
			move = false;
		else
			for(int enemyCheck = 0; enemyCheck < ENEMIES; enemyCheck++)
				if(!(y == elemental[enemyCheck].y && x == elemental[enemyCheck].x))
					if(y == elemental[enemyCheck].y+32
						&& ((x >= elemental[enemyCheck].x && x < elemental[enemyCheck].x+32) || (x+32 > elemental[enemyCheck].x && x+32 <= elemental[enemyCheck].x+32)))
					{
						move = false;
						break;
					}
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case(DOWN):
		if((mapArray[(x)/32][(y+32)/32] == 1 || mapArray[(x+32-1)/32][(y+32)/32] == 1)) //adjust bottom side: right X - 1
			move = false;
		else
			for(int enemyCheck = 0; enemyCheck < ENEMIES; enemyCheck++)
				if(!(y == elemental[enemyCheck].y && x == elemental[enemyCheck].x))
					if(y+32 == elemental[enemyCheck].y 
						&& ((x >= elemental[enemyCheck].x && x < elemental[enemyCheck].x+32) || (x+32 > elemental[enemyCheck].x && x+32 <= elemental[enemyCheck].x+32)))
						move = false;
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case(LEFT):
		if((mapArray[(x-1)/32][(y)/32] == 1 || mapArray[(x-1)/32][(y+32-1)/32] == 1)) //adjust left side: X - 1, bottom Y - 1
			move = false;
		else
			for(int enemyCheck = 0; enemyCheck < ENEMIES; enemyCheck++)
				if(!(y == elemental[enemyCheck].y && x == elemental[enemyCheck].x))
					if(x == elemental[enemyCheck].x+32 
						&& ((y >= elemental[enemyCheck].y && y < elemental[enemyCheck].y+32) || (y+32 > elemental[enemyCheck].y && y+32 <= elemental[enemyCheck].y+32)))
						move = false;
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case(RIGHT):
		if((mapArray[(x+32)/32][(y)/32] == 1 || mapArray[(x+32)/32][(y+32-1)/32] == 1)) //adjust right side: botom Y - 1
			move = false;
		else
			for(int enemyCheck = 0; enemyCheck < ENEMIES; enemyCheck++)
				if(!(y == elemental[enemyCheck].y && x == elemental[enemyCheck].x))
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
		for(int enemy = 0; enemy < ENEMIES; enemy++){
			elemental[enemy].movement(targetX, targetY, elemental);
			if(elemental[enemy].y > enemyStartY - 64)
				elemental[enemy].speed = 2;
		}
}

void Enemies::drawEnemies(Enemies elemental[], BITMAP* dest) {
		for(int enemy = 0; enemy < ENEMIES; enemy++)
				elemental[enemy].drawFrame(dest);
}