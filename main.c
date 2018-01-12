#include "myLib.h"
#include "background.h"
#include <stdlib.h>
#include <stdio.h>

int main() 
{
	char buffer1[40];

	REG_DISPCNT = MODE_3 | BG2_ENABLE;

	enum GBAState state = TRANSSTART;

	int lives = 3;
	int totalGameObjects = 0;
	int seed = 0;

	while(1)  //game loop
	{    
		switch(state)
		{	
			case TRANSSTART:
				totalGameObjects = 0;
				lives = 3;
				drawHomeScreen();
				state = START;
				waitForVblank();
			break;

			case START:
				seed += 10;
				if (KEY_DOWN_NOW(BUTTON_START))
				{
					state = TRANSGAME;
				}
				HELICOPTER heli = {HELI_INITIALX, HELI_INITIALY, HELI_XDEL, HELI_YDEL, HELI_INITIALX, HELI_INITIALY, RED};
			break;

			case TRANSGAME:
				srand(seed);
				int numGameObjs = 0;
				WALL gameObjs[NUMWALLS];
				drawImage(0, 0, DRIBBBLE_WIDTH, DRIBBBLE_HEIGHT, dribbble);
				WALL objs[NUMWALLS];
				for(int i = 0; i < NUMWALLS; i++)
				{
					objs[i].x = 25 + rand() % 194;
					objs[i].y = 10 + rand() % 109;
					objs[i].width = 10;
					objs[i].height = 30;
				}
				heli.x = 0;
				state = GAME;
				waitForVblank();
			break;

			case GAME:
				//checks to see if players has won
				if (totalGameObjects >= 35)
				{
					state = WIN;
				}

				//checks if player wants to restart
				if (KEY_DOWN_NOW(BUTTON_SELECT))
				{
					state = TRANSSTART;
				}

				if(KEY_DOWN_NOW(BUTTON_UP))
				{	
					heli.oldy = heli.y;
					heli.y = heli.y - heli.ydel;
				}

				if(KEY_DOWN_NOW(BUTTON_RIGHT))
				{
					heli.oldx = heli.x;
					heli.x = heli.x + heli.xdel;				
				}

				if(KEY_DOWN_NOW(BUTTON_DOWN))
				{
					heli.oldy = heli.y;
					heli.y = heli.y + heli.ydel;
				}

				if (!(KEY_DOWN_NOW(BUTTON_UP)) && !(KEY_DOWN_NOW(BUTTON_DOWN))) 
				{
					heli.oldy = heli.y;
					heli.y = heli.y + heli.ydel;
				}

				//checks if helicopter is touching the ground
				if (heli.y == 149)
				{
					state = TRANSEND;	
				}

				//check for collision
				int check = FALSE;
				//int check2 = FALSE;
				for (int i = 0; i < numGameObjs; i++)
				{
					check = FALSE;
					check = checkForCollision(heli, gameObjs[i]);

					if (check == TRUE) {
						state = TRANSEND;
					} 
				}

				waitForVblank();

				//keep object in screen
				if (heli.y > 150) 
				{
					heli.y = 150;
				}

				if (heli.y < 0) 
				{
					heli.y = 0;
				}

				fixBackground(heli.oldy, heli.oldx, CHOPPER_WIDTH, CHOPPER_HEIGHT, dribbble);
				drawImage(heli.y, heli.x, CHOPPER_WIDTH, CHOPPER_HEIGHT, chopper);
			
				//check to see if wall should be placed on game screen
				for(int i = 0; i < NUMWALLS; i++)
				{
					int tempx = objs[i].x;
					int tempy = objs[i].y;
					int flag = FALSE;

					for (int j = 0; j < numGameObjs; j++) 
					{
						if (abs(gameObjs[j].x - tempx) < 15 || abs(gameObjs[j].y - tempy) < 20) 
						{
							flag = TRUE;
						} 
					}

					if (flag == FALSE) 
					{	
						gameObjs[numGameObjs++] = objs[i];
						totalGameObjects += 1; 
						drawImage(objs[i].y, objs[i].x, objs[i].width, objs[i].height, wall);
					}
				}

				//write score onto game screen
				sprintf(buffer1, "Lives: %d", lives);
				drawString(150, 5, buffer1, RED);	

				//update variables
				heli.oldx = heli.x;
				heli.oldy = heli.y;

				//update seed
				seed += 10;

				//move game-screen along
				if (heli.x == 229)
				{
					state = TRANSGAME;
				}
			break;

			case TRANSEND: 	//transition state for BUFFER	
				lives--;

				//checks to see if player has remaining lives
				if (lives == 0)
				{
					state = BUFFER;
				} else 
				{
					delay(30);
					heli.y = 2;
					state = TRANSGAME;
				}
			break;

			case BUFFER:	//transition state for END
				delay(10);
				drawEndScreen();
				state = END;
			break;

			case WIN:       //state for win
				delay(10);
				drawWinScreen();
				state = END;
			break;

			case END:		//state for END
				if (KEY_DOWN_NOW(BUTTON_SELECT))
				{
					state = TRANSSTART;
				}
			break;
		}
	}
}

 