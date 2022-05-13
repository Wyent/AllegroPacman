#pragma once
#include <allegro.h>
#include "Enemies.h"
#include "Globals.h"
#include "PowerUps.h"
#include "Player.h"

class World
{
public:
	
	int screenX;
	int screenY;
	int score;
	int lives;
	int level;
	int invulnTime;
	int numPellets;
	int pelletsLeft;
	int bonusTimer;
	int numKilled;
	int numBonuses;
	BITMAP* pellet;
	BITMAP* powerPellet;
	BITMAP* bonus;

	World(void);
	~World(void);
//	PowerUps p;
	bool loadLevel(int level, Enemies elemental[4]);
	/*Follows the given x, y coordinate. speedFactor is how
	smoothly the camera will follow the target. Higher value means
	slower, smoother scrolling.*/
	void followTarget(int x, int y, int speedFactor);

	void updateWorld(Player* thePlayer, Enemies elemental[]);
	void drawWorld(BITMAP* dest);
	void resetWorld(Player* thePlayer, Enemies []);
	//Decrements the level's power pellet timer.
	inline void invul_timer() {
		invulnTime--;
	}
	//Returns if the power pellet timer is still active
	inline bool isInvuln() {
		return invulnTime>0;
	}
	//Sets the power pellet timer to 6 sec.
	inline void setInvuln() {
		invulnTime += 360;
	}

	//Returns the number of remaining pellets, not counting power pellets!
	inline int getRemainingPellets() {
		return pelletsLeft;
	}
};

