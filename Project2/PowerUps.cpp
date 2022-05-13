#include "PowerUps.h" 
#include "Player.h"


PowerUps::PowerUps(void)
{
	pelletsCollected = 0;
	 
}


PowerUps::~PowerUps(void)
{
}


//void PowerUps::powerUpsMain()
//{
////	pellets.loadSprite("Graphics//pellet1.bmp", 32, 32, 0, 1);
// 
//
//	if (playerCollision ( wizard.x, wizard.y, wizard.width, wizard.height, x, y, width, height))
//	{	
//		score += 10;
//	}
//
//	//powerPellet.loadSprite("Graphics//powerPellet.bmp", 32, 32, 0, 1);
//	
//	
//	powerPellets();
//}

//void PowerUps::powerPellets()
//{
//	wizard.state = 1;
//	enemies.state = 0;
//	if (playerCollision ( wizard.x, wizard.y, wizard.width, wizard.height, x, y, width, height))
//	{
//		score += 50;
//	}
//
//
//}

//void PowerUps::death()
//{
//	if(wizard.state < enemies.state && enemies.state != 2 && playerCollision ( wizard.x, wizard.y, wizard.width, wizard.height, enemies.x, enemies.y, enemies.width, enemies.height))
//		wizard.state = 2;
//	if(wizard.state > enemies.state && playerCollision ( wizard.x, wizard.y, wizard.width, wizard.height, enemies.x, enemies.y, enemies.width, enemies.height))
//	{
//		score += 200;
//		enemies.state = 2;
//	}
//}

//void PowerUps::bonusItems(Player wizard)
//{
//	if(playerCollision ( wizard.x, wizard.y, wizard.width, wizard.height, x, y, width, height))
//		score += 100;
//}

bool PowerUps::playerCollision (int playerX, int playerY, int playerW, int playerH, int x, int y, int width, int height)
	{
			
	//bottom to other's top
	if (y + height < playerY) {
		return false;
	}
	//top to other's bottom
	if (y > playerY + playerH) {
		return false;
	}
	//right to other's left
	if (x + width < playerX) {
		return false;
	}
	//left to other's right
	if (x > playerX + playerW) {
		return false;
	}

	//there is a collision
	return true;
	}

