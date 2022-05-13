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
BITMAP* HUD;
BITMAP* HUDTemp;
BITMAP* Lives;
BITMAP* Items;
BITMAP* Score;
Player wizard;
Enemies elemental[ENEMIES];
PowerUps pellets;
void init();
void deinit();
void drawBuffer();
void loop_timer();
void drawScore();
int getPlaceValue(int number, int place);
void drawHUD(World Level);
void fade_in(BITMAP *bmp_orig, int speed, int color = 0x000000);
void fade_out(int speed, int color = 0x000000);
int main() {
	init();
	World level;
	//Load the Level here
	level.loadLevel(++theLevel, elemental);
	wizard.loadSprite("Graphics//Sheets//PlayerSpriteSheet.bmp", 32, 32, 5, 6);
	elemental[0].moveTimer = 50;
	elemental[0].loadSprite("Graphics//Sheets//RockSheet.bmp", 32, 32, 1, 2);
	elemental[1].moveTimer = 150;
	elemental[1].loadSprite("Graphics//Sheets//SandSheet.bmp", 32, 32, 1, 2);
	elemental[2].moveTimer = 100;
	elemental[2].loadSprite("Graphics//Sheets//ElectricSheet.bmp", 32, 32, 1, 2);
	elemental[3].moveTimer = 200;
	elemental[3].loadSprite("Graphics//Sheets//LightSheet.bmp", 32, 32, 1, 2);
	for(int i = 0; i<ENEMIES; i++) {
		elemental[i].aliveSpeed = 2; //pow(2.0, i);
		elemental[i].frameDelay = 15;
	}
	for(int enemy = 0; enemy < ENEMIES; enemy++){
		elemental[enemy].ID = enemy; //Give each enemy an ID
		//elemental[enemy].x = enemyStartX-(32/**enemy*/);	//(enemyStartX -32) * 2 -32 -32;
		elemental[enemy].x = enemyStart[enemy][0];	//(enemyStartX -32) * 2 -32 -32;
		elemental[enemy].y = enemyStart[enemy][1];	//(enemyStartX -32) * 2 -32 -32;
	}
	HUD = load_bitmap("Graphics//HudIcons//HUDbackground.bmp", 0);
	HUDTemp = create_bitmap(HUD->w, HUD->h);
	Lives = load_bitmap("Graphics//HUDIcons//lives.bmp",0);
	Items = load_bitmap("Graphics//HUDIcons//items_text.bmp",0);
	Score = load_bitmap("Graphics//HUDIcons//score_text.bmp",0);
	//elemental[3].x = enemyStartX-32;	//(enemyStartX -32) * 2 -32 -32;
	//elemental[3].y = enemyStartY-32;	//enemyStartY +32;
	//elemental[1].y -=64;*/
	//mapArray[elemental[3].x/32][elemental[3].y/32] = 1;

	buffer = create_bitmap(40*32,32*32+80);
	LOCK_VARIABLE(counter);
	LOCK_FUNCTION(loop_timer);
	install_int(loop_timer, alarm);
	srand(time(NULL));
	createNumbers();
	bool playerFollow = false;
	int element = -1;
	//level.drawWorld(buffer);
	//blit(BG, buffer, 0, 0, 0, 0, buffer->w, buffer->h);
	//fade_in(buffer, 3, 0x000000);
	BITMAP* startScreen = load_bitmap("Graphics//SplashAttack//StartGame.bmp", 0);
	BITMAP* lossScreen = load_bitmap("Graphics//SplashAttack//EndGame.bmp", 0);

	wizard.frameDelay = 10;
	//wizard.x = startX;
	//wizard.y = startY;
	while(!key[KEY_ENTER]) {
		if(counter) {
			blit(BG, buffer, 0, 0, 0, 0, buffer->w, buffer->h);

			wizard.movement();

			level.updateWorld(&wizard, elemental);

			Enemies::updateEnemies(elemental, wizard.x, wizard.y);

			level.drawWorld(buffer);
			wizard.checkSpell(elemental);
			level.screenX = -224;
			level.screenY = -256;
			blit(startScreen, buffer, 0, 0, -level.screenX, -level.screenY, startScreen->w, startScreen->h);
			wizard.drawFrame(buffer);
			Enemies::drawEnemies(elemental, buffer);

			blit(buffer, screen, 0, 0, level.screenX, level.screenY, buffer->w, buffer->h);
			clear_bitmap(buffer);
			counter = 0;
		}

	}
	theLevel = 1;
	level.loadLevel(++theLevel, elemental);

	fade_out(3, 0x000000);
	fade_in(buffer, 3, 0x000000);
	while (!key[KEY_ESC] && theLevel < 4 && lives>0) {
			//set enemy spawn points
			for(int enemy = 0; enemy < ENEMIES; enemy++){
				elemental[enemy].ID = enemy; //Give each enemy an ID
				//elemental[enemy].x = enemyStartX-(32/**enemy*/);	//(enemyStartX -32) * 2 -32 -32;
				elemental[enemy].x = enemyStart[enemy][0];	//(enemyStartX -32) * 2 -32 -32;
				elemental[enemy].y = enemyStart[enemy][1];	//(enemyStartX -32) * 2 -32 -32;
			}

		wizard.x = startX;
		wizard.y = startY;
		wizard.frameDelay = 10;
		blit(BG, buffer, 0, 0, 0, 0, buffer->w, buffer->h);
		level.drawWorld(buffer);
		fade_in(buffer, 3, 0x000000);

		do {
			if(counter)
			{
				//Put background on buffer
				blit(BG, buffer, 0, 0, 0, 0, buffer->w, buffer->h);
				if(wizard.getState() !=2)
					wizard.movement();
				else
					wizard.doDeath();
				level.updateWorld(&wizard, elemental);
				//To clean up the code a bit, I've made static functions in the enemies class to handle both updating and drawing
				//I actually learned something from Java class.. ha ha.
				Enemies::updateEnemies(elemental, wizard.x, wizard.y);

			
				if(key[KEY_P])
					playerFollow = true;
				if(key[KEY_O]){
					playerFollow = false;
					if(++element >= ENEMIES)
						element = 0;
				}
				if(playerFollow)
					level.followTarget(wizard.x-SCREEN_W/2, wizard.y-SCREEN_H/2 +40, 2);
				else
					level.followTarget(elemental[1].x-SCREEN_W/2, elemental[1].y-SCREEN_H/2, 2);

				level.drawWorld(buffer);
				wizard.checkSpell(elemental);
				wizard.drawFrame(buffer);
				//Drawing function for enemies
				Enemies::drawEnemies(elemental, buffer);
	#ifdef _DEBUG
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+2, makecol(255,255,255), makecol(255,0,0), "X: %d", elemental[1].x);
				textprintf_ex(buffer, font, -level.screenX+10, -level.screenY+12, makecol(255,255,255), makecol(255,0,0), "Y: %d", elemental[1].y);
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+42, makecol(255,255,255), makecol(255,0,0), "Direction: %d", elemental[1].currentMove);
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+52, makecol(255,255,255), makecol(255,0,0), "Queue: %d", elemental[1].queuedMove);
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+62, makecol(255,255,255), makecol(255,0,0), "State: %d", wizard.state);

				/*textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+72, makecol(255,255,255), makecol(255,0,0), "LeftCheckDistance: %d", elemental[3].checkDistance[1]);
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+82, makecol(255,255,255), makecol(255,0,0), "DownCheckDistance: %d", elemental[3].checkDistance[2]);
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+92, makecol(255,255,255), makecol(255,0,0), "RightCheckDistance: %d", elemental[3].checkDistance[3]);
				*/if(elemental[3].roam)
					textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+102, makecol(255,255,255), makecol(255,0,0), "Roaming: true");
				else
					textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+102, makecol(255,255,255), makecol(255,0,0), "Roaming: false");

				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+112, makecol(255,255,255), makecol(255,0,0), "Enemy State: %d", elemental[1].state);
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+122, makecol(255,255,255), makecol(255,0,0), "TargetX: %d", elemental[1].targetX);
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+132, makecol(255,255,255), makecol(255,0,0), "TargetY: %d", elemental[1].targetY);
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+152, makecol(255,255,255), makecol(255,0,0), "Total Pellets: %d", level.numPellets);
				textprintf_ex(buffer, font, -level.screenX+10,  -level.screenY+162, makecol(255,255,255), makecol(255,0,0), "Pellets Left: %d", level.pelletsLeft);


	#endif
				clear_bitmap(HUDTemp);
				blit(HUD, HUDTemp, 0, 0, 0, 0, HUD->w, HUD->h);
				int temp = getPlaceValue(lives, 1);
				masked_blit(numberArray[temp],HUDTemp,0,0,138,10,HUD->w, HUD->h);
				masked_blit(Lives, HUDTemp,0,0,10,10,HUD->w, HUD->h);
				masked_blit(Items, HUDTemp,0,0,450,10,HUD->w, HUD->h);
				masked_blit(Score, HUDTemp,0,0,180,15,HUD->w, HUD->h);
				drawScore();
				blit(HUDTemp, buffer, 0, 0, -level.screenX, -level.screenY+520, HUD->w, HUD->h);
			
				acquire_screen();
				blit(buffer, screen, 0, 0, level.screenX, level.screenY, buffer->w, buffer->h);
				release_screen();
				clear_bitmap(buffer);
				//wizard.wind();

				counter = 0;
			}
			
		}while(level.getRemainingPellets() > 0 && !key[KEY_ESC] && lives > 0);
		if(lives<=0)
			fade_out(3, 0xFF0000);
		else
			fade_out(3, 0x000000);

	level.loadLevel(++theLevel, elemental);
	if(lives!=0)
		++lives;
	}
	wizard.currentMove = 0;
	wizard.queuedMove = 0;

	if(lives<=0) {
			theLevel = 4;
			level.loadLevel(++theLevel,elemental);
			//set enemy spawn points
			for(int enemy = 0; enemy < ENEMIES; enemy++){
				elemental[enemy].ID = enemy; //Give each enemy an ID
				//elemental[enemy].x = enemyStartX-(32/**enemy*/);	//(enemyStartX -32) * 2 -32 -32;
				elemental[enemy].x = enemyStart[enemy][0];	//(enemyStartX -32) * 2 -32 -32;
				elemental[enemy].y = enemyStart[enemy][1];	//(enemyStartX -32) * 2 -32 -32;
			}
			
		bool fade = true;
		while(!key[KEY_ENTER]) {
			if(counter) {
				//blit(BG, buffer, 0, 0, 0, 0, buffer->w, buffer->h);
				wizard.movement();

				level.updateWorld(&wizard, elemental);

				Enemies::updateEnemies(elemental, wizard.x, wizard.y);

				//level.drawWorld(buffer);
				wizard.checkSpell(elemental);
				//level.screenX = -224;
				//level.screenY = -256;
				blit(lossScreen, buffer, 0, 0, 224, 256, startScreen->w, startScreen->h);
				wizard.drawFrame(buffer);
				Enemies::drawEnemies(elemental, buffer);

				blit(buffer, screen, 0, 0, level.screenX, level.screenY, buffer->w, buffer->h);
				if(fade) {
					fade_in(buffer, 3, 0xFF0000);
					fade = false;
				}
				clear_bitmap(buffer);
				
				counter = 0;
			}
		}
	}

	destroy_bitmap(startScreen);
	startScreen = nullptr;

	destroy_bitmap(lossScreen);
	lossScreen = nullptr;
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
	destroy_bitmap(HUD);
	HUD = nullptr;
	destroy_bitmap(HUDTemp);
	HUDTemp = nullptr;
	destroy_bitmap(Lives);
	Lives = nullptr;
	destroy_bitmap(Score);
	Score = nullptr;
	destroy_bitmap(Items);
	Items = nullptr;

	for(int i = 0; i<10; i++) {
		destroy_bitmap(numberArray[i]);
		numberArray[i] = nullptr;
	}
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

int getPlaceValue(int number, int place)  {
	return ((number/place)%10);
}

void drawScore() {
	int index = 0;

	if(newScore > score)
		score+=(newScore-score)/4 +1;
	for(int i = 0; i<5; i++) {
		index = getPlaceValue(score, pow((double)10,i));
		
		masked_blit(numberArray[index], HUDTemp, 0, 0, 400-(i*24), 16, numberArray[index]->w, numberArray[index]->h);
	}
}

void transition() {
	//fade_out(3, 0x000000);
	fade_in(buffer, 3, 0x000000);
}

void fade_in(BITMAP *bmp_orig, int speed, int color)
{
   BITMAP *bmp_buff;
   bmp_buff = create_bitmap(SCREEN_W, SCREEN_H);
    if ((bmp_buff))
    {
        int a;
		
        if (speed <= 0) speed = 16;
        
        for (a = 0; a < 256; a+=speed)
        {
            clear_to_color(bmp_buff, color);
            set_trans_blender(getr(color),getg(color),getb(color),a);
            draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
            blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
        }
        destroy_bitmap(bmp_buff);
    } 

    blit(bmp_orig, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
}

void fade_out(int speed, int color)
{
    BITMAP *bmp_orig, *bmp_buff;
	bmp_orig = create_bitmap(SCREEN_W, SCREEN_H);
    bmp_buff = create_bitmap(SCREEN_W, SCREEN_H);
	if (bmp_orig)
    {
        if (bmp_buff)
        {
            int a;
            blit(screen, bmp_orig, 0,0, 0,0, SCREEN_W, SCREEN_H);
            if (speed <= 0) speed = 16;
        
            for (a = 255-speed; a > 0; a-=speed)
            {
				clear_to_color(bmp_buff, color);
                set_trans_blender(0,0,0,a);
                draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
                blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
            }
            destroy_bitmap(bmp_buff);
        }
        destroy_bitmap(bmp_orig);
    }

}
