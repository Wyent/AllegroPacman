#include <allegro.h>
#include <math.h>
#include "Globals.h"
#include "World.h"
#include "Player.h"
#include "PowerUps.h"
#include "Enemies.h"
#include <time.h>
#define ENEMIES 4
volatile int counter;
int alarm = 16;
BITMAP* buffer;
BITMAP* pellet;
Player wizard;
Enemies elemental[ENEMIES];
PowerUps pellets;
void init();
void deinit();
void drawBuffer();
void loop_timer();
int main() {
	init();
	World level;
	level.loadLevel("Graphics//testLevel1.bmp", 0);
	wizard.loadSprite("Graphics//PlayerPH.bmp", 32, 32, 0, 1);
	elemental[0].loadSprite("Graphics//ICE1.bmp", 32, 32, 0, 1);
	elemental[1].loadSprite("Graphics//ICE1.bmp", 32, 32, 0, 1);
	elemental[2].loadSprite("Graphics//ICE1.bmp", 32, 32, 0, 1);
	elemental[3].loadSprite("Graphics//ICE1.bmp", 32, 32, 0, 1);
	for(int i = 0; i<4; i++)
		elemental[i].speed = pow(2.0, i);
	pellet = load_bitmap("Graphics//Level1//pacdot.bmp", 0);
	wizard.x = startX;
	wizard.y = startY;
	for(int enemy = 0; enemy < ENEMIES; enemy++){
		elemental[enemy].x = enemyStartX-(32/**enemy*/);	//(enemyStartX -32) * 2 -32 -32;
		elemental[enemy].y = enemyStartY;	//enemyStartY +32;
	}
	/*elemental[3].x = enemyStartX-32;	//(enemyStartX -32) * 2 -32 -32;
	elemental[3].y = enemyStartY-32;	//enemyStartY +32;
	elemental[1].y -=64;*/
	//mapArray[elemental[3].x/32][elemental[3].y/32] = 1;

	buffer = create_bitmap(40*32,32*32);
	LOCK_VARIABLE(counter);
	LOCK_FUNCTION(loop_timer);
	install_int(loop_timer, alarm);
	srand(time(NULL));
	bool playerFollow = false;
	while (!key[KEY_ESC]) {
		if(counter)
		{
			blit(BG, buffer, 0, 0, 0, 0, buffer->w, buffer->h);

			//Put pellets on-screen
			for(int i = 0; i < 40; i++)
			{
				for(int j = 0; j < 32; j++)
				{
					if(mapArray[i][j] == 2)
						//draw pellet
						masked_blit(pellet, buffer, 0, 0, i*32, j*32, pellet->w, pellet->h);
				}
			}
			wizard.movement();
			//To clean up the code a bit, I've made static functions in the enemies class to handle both updating and drawing
			//I actually learned something from Java class.. ha ha.
			Enemies::updateEnemies(elemental, wizard.x, wizard.y);
			int element = -1;
			if(key[KEY_P])
				playerFollow = true;
			if(key[KEY_O]){
				playerFollow = false;
				if(++element >= ENEMIES)
					element = 0;
				
			}
			if(playerFollow)
				level.followTarget(wizard.x-SCREEN_W/2, wizard.y-SCREEN_H/2);
			else
				level.followTarget(elemental[3].x-SCREEN_W/2, elemental[3].y-SCREEN_H/2);

			wizard.drawFrame(buffer);
			//Drawing function for enemies
			Enemies::drawEnemies(elemental, buffer);

			textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+2, makecol(255,255,255), makecol(255,0,0), "X: %d", elemental[3].x);
			textprintf_ex(buffer, font, -level.screenX+10, -level.screenY+12, makecol(255,255,255), makecol(255,0,0), "Y:: %d", elemental[3].y);
			textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+42, makecol(255,255,255), makecol(255,0,0), "Direction: %d", elemental[3].currentMove);
			textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+52, makecol(255,255,255), makecol(255,0,0), "Queue: %d", elemental[3].queuedMove);
			/*textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+72, makecol(255,255,255), makecol(255,0,0), "LeftCheckDistance: %d", elemental[3].checkDistance[1]);
			textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+82, makecol(255,255,255), makecol(255,0,0), "DownCheckDistance: %d", elemental[3].checkDistance[2]);
			textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+92, makecol(255,255,255), makecol(255,0,0), "RightCheckDistance: %d", elemental[3].checkDistance[3]);
			*/if(elemental[3].roam)
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+102, makecol(255,255,255), makecol(255,0,0), "Roaming: true");
			else
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+102, makecol(255,255,255), makecol(255,0,0), "Roaming: false");
			//rectfill(buffer, -level.screenX, -level.screenY, 800-level.screenX, 64-level.screenY, 0xFF0000);
			blit(buffer, screen, 0, 0, level.screenX, level.screenY, buffer->w, buffer->h);
			clear_bitmap(buffer);

			counter = 0;
		}
	}
	deinit();
	return 0;
}
END_OF_MAIN()

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
	/* add other initializations here */
}

void deinit() {
	destroy_bitmap(BG);
	BG = nullptr;
	destroy_bitmap(buffer);
	buffer = nullptr;
	clear_keybuf();
	/* add other deinitializations here */
}

void drawBuffer()
{
	acquire_screen();
	blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
	release_screen();
}

void loop_timer(){
	counter++;
}END_OF_FUNCTION(loop_timer);