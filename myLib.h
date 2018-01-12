//////////////////////////////////////////////////////////////////////
//
//  INDEX
// 
//////////////////////////////////////////////////////////////////////
/* 
        General
        Video
        DMA
        Input

====================================================================*/

// *** General =======================================================

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef char s8;
typedef short s16;
typedef int s32;

#define TRUE 1
#define FALSE 0

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

// *** Video =======================================================
extern const unsigned char fontdata_6x8[12288];
extern u16* videoBuffer;               // 0x6000000
#define REG_DISPCNT  *(u16*) 0x4000000
#define SCANLINECOUNTER *(unsigned short *)0x4000006

#define COLOR(r,g,b) ((r) + ((g) << 5) + ((b) << 10))

#define BG2_ENABLE  (1<<10)

#define WHITE   COLOR(31, 31, 31)
#define RED     COLOR(31,0,0)
#define BLUE    COLOR(0,0,31)
#define YELLOW  COLOR(31,31,0)

#define MODE_3 3


/* DMA */
typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	u32                  cnt;
} DMA_CONTROLLER;

typedef struct
{	
	int x;
	int y;
	int xdel;
	int ydel;
	int oldx;
	int oldy;
	int color;
} HELICOPTER;

typedef struct
{
	int width;
	int height;
	int x;
	int y;
	u16* img;
} WALL;

enum GBAState 
{
	TRANSSTART,
	START,
	TRANSGAME,
	GAME,
	TRANSEND,
	BUFFER,
	END,
	WIN
};

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
// DMA channels
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

// DMA copies 16 bits at a time normally (1 pixel), 
// but can be set to copy 32 bits at a time to be 
// even faster for mutliples of 32 bits.
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

// Don't worry about these, the default DMA_NOW is good enough
// for 2110, if you want to go beyond the scope of the class you
// can ask a TA or the internet about the other options.
#define DMA_NOW (0 << 28)
#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

// *** Input =========================================================

// Buttons

// #define BUTTON_A		(1<<0)
// #define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
// #define BUTTON_R		(1<<8)
// #define BUTTON_L		(1<<9)

#define BUTTONS (*(unsigned int *)0x4000130)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define OFFSET(row, col, rowlen)  ((row)*(rowlen)+(col))

#define NUMWALLS 30
#define HELI_WIDTH 10
#define HELI_HEIGHT 10
#define HELI_INITIALX 0
#define HELI_INITIALY 0
#define HELI_XDEL 1
#define HELI_YDEL 1

// Prototypes
void waitForVblank();
void drawImage(int row, int col, int width, int height, const u16* image);
void drawRectangle(int row, int col, int width, int height, int color);
void fixBackground(int row, int col, int width, int height, const u16* image);
void initializeHelicopter(int width, int height, int x, int y, int xdel, int ydel, int oldx, int oldy, int color);
void drawString(int row, int col, char str[], int color);
void drawChar(int row, int col, char ch, int color);
void drawHomeScreen();
void setPixel(int row, int col, u16 color);
void drawEndScreen();
int checkForCollision(HELICOPTER heli, WALL gameObjs);
void drawWinScreen();
void delay(int n);
