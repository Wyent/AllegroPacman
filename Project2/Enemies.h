#pragma once

#include "Movers.h"
#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4
#define WIND 1
#define ROCK 2
#define WATER 3
#define LIGHT 4
#define METAL 5
#define FIRE 6
#define DARK 7
#define SAND 8
#define ELECTRIC 9
#define EARTH 10
#define POISON 11
#define ICE 12
#define NATURE 13
class Enemies : public Movers
{
public:
	Enemies(void);
	~Enemies(void);

	bool canMove(int, Enemies[]);
	int currentMove;
	int queuedMove;
	int direction;
	int targetX;
	int targetY;
	int moveTimer;
	int vision;
	bool roam;
	bool flee;
	bool turnAround;
	bool flashing;
	int aliveSpeed;
	int type;
	void movement(int, int, Enemies[]);
	int findSmallestDist(Enemies[]);
	inline void Flee() {
		flee = true;
		turnAround = true;
	}
	static void updateEnemies(Enemies[], int targetX, int targetY);
	static void drawEnemies(Enemies[], BITMAP* dest);

};

