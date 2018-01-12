#include "background.h"
#include "myLib.h"
#include <stdlib.h>

	u16* videoBuffer = (u16*)0x6000000;

void drawImage(int row, int col, int width, int height, const u16* image) 
{
    for (int r = 0; r < height; r++) 
    {
        DMA[3].src = image + r * width;
        DMA[3].dst = videoBuffer + ((row + r) * SCREEN_WIDTH) + col;
        DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void fixBackground(int row, int col, int width, int height, const u16* image) 
{
    for (int r = 0; r < height; r++) 
    {
        DMA[3].src = image + ((row + r) * SCREEN_WIDTH) + col;
        DMA[3].dst = videoBuffer + ((row + r) * SCREEN_WIDTH) + col;
        DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void drawChar(int row, int col, char ch, int color)
{
	for(int r = 0; r < 8; r++)
	{
		for(int c=0; c<6; c++)
		{
			if (fontdata_6x8[OFFSET(r, c, 6) + ch*48] == 1)
			{
				setPixel(row+r, col+c, color);
			}
		}
	}
}

void drawString(int row, int col, char str[], int color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col += 6;
	}
}

void drawRectangle(int row, int col, int width, int height, int color) 
{
	u16 rectColor = color;
    for (int r = 0; r < height; r++) 
    {
        DMA[3].src = &rectColor;
        DMA[3].dst = videoBuffer + ((row + r) * SCREEN_WIDTH + col);
        DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 161);
}

void drawHomeScreen()
{	
	drawImage(0, 0, STARTSCREEN_WIDTH, STARTSCREEN_HEIGHT, startscreen);
	drawString(140, 60, "Press ENTER to begin!", YELLOW);
}

void drawEndScreen()
{	
	drawImage(0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, gameover);
	drawString(80, 84, "The enemy won!", YELLOW);
	drawString(120, 42, "Press SELECT to play again.", YELLOW);
}

void drawWinScreen()
{	
	drawImage(0, 0, BILLLEAHY_WIDTH, BILLLEAHY_HEIGHT, billleahy);
	drawString(108, 50, "You dodged 30 obstacles!", YELLOW);
	drawString(128, 65, "You won Heli Attack", YELLOW);
	drawString(148, 50, "Press SELECT to play again.", YELLOW);
}

void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

int checkForCollision(HELICOPTER heli, WALL wall) 
{	
	int wxhx = wall.x - heli.x;
	int hxwx = heli.x - wall.x;
	int hywy = heli.y - wall.y;
	int wyhy = wall.y - heli.y;

	if (((wxhx <= CHOPPER_WIDTH && wxhx >= 0) || (hxwx <= WALL_WIDTH && hxwx >= 0)) && ((hywy <= WALL_HEIGHT && hywy >= 0) || (wyhy <= CHOPPER_HEIGHT && wyhy >= 0))) 
	{
		return TRUE;
	} 
	return FALSE;
}

void delay(int n)
{
	volatile int size = 0;
	for(int i=0; i<n*4000; i++)
	{
		size = size + 1;
	}
}
