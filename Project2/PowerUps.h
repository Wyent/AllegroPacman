#pragma once
#include "Player.h"
#include "Enemies.h"
#include "World.h"

class PowerUps //: public Player
{
public: 
	int pelletsCollected;
	
	PowerUps(void);
	~PowerUps(void);

	Player wizard;
	Enemies enemies;
	bool pellet;
	bool powerPellet;
	
	void powerUpsMain();
	void powerPellets();
	void death();
	bool playerCollision (int playerX, int playerY, int playerW, int playerH, int x, int y, int width, int height);	
	void bonusItems(Player wizard);
	////elemental powers
	//int ability[4];

	//int water();
	//int light();
	//int metal();
	//int wind();
	//void fire();
	//int dark();
	//int sand();
	//int electric();
	//int earth();
	//int poison();
	//int ice();
	//int rock();
	//int nature();


};

