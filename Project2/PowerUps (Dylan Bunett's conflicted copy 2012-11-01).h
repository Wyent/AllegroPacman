#pragma once
#include "Player.h"
#include "Enemies.h"
#include "World.h"

class PowerUps : public Player
{
public: 
	
	PowerUps(void);
	~PowerUps(void);

	Player wizard;
	Enemies enemies;
	int pelletsCollected;
	
	void powerUpsMain();
	void powerPellets();
	void bonusItems(Player wizard);
	void death();
	bool PowerUps::playerCollision (int playerX, int playerY, int playerW, int playerH, int x, int y, int width, int height);	

	////elemental powers
	//int ability[4];

	//int water();
	//int light();
	//int metal();
	//int wind();
	//int fire();
	//int dark();
	//int sand();
	//int electric();
	//int earth();
	//int poison();
	//int ice();
	//int rock();
	//int nature();


};

