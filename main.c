/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <ti84pce.h>

/* Standard headers - it's recommended to leave them included */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <keypadc.h>
#include <graphc.h>
#include <fileioc.h>
#include <tiles.h>
#include <logo.h>
#include <debug.h>

/* Other available headers */
// stdarg.h, setjmp.h, assert.h, ctype.h, float.h, iso646.h, limits.h, errno.h

bool exitSection;
bool quit;
unsigned char key;
int tilerow;
int tilecolumn;
int tilenumber;
int boardx;
int boardy;
int selectx;
int selecty;
int score;
unsigned char tiles[10][10];
Tile buffer1;
Tile buffer2;
Tile buffer3;

bool buffer1Active;
bool buffer2Active;
bool buffer3Active;

int currentBufferInUse;
int whichL;
bool canDrawTileBool;
bool movesRemaining;
int flicker;
int oldHS;
bool darkMode;
bool lastClearCheck;

void main() {
	ti_CloseAll();
	gc_InitGraph();
	gc_DrawBuffer();
	setDefaultHS();
	flicker = 0;
	quit = false;
	exitSection = false;
	darkMode = false;
	while (kb_AnyKey()) {

	}
	gc_SetPalette(logo_pal, sizeof logo_pal);
	gc_SetColor(0x75,0x0000);
	gc_SetTextColor(0x75);
	while (exitSection == false) {
		gc_SetTextXY(100, 120);
		gc_PrintString("Highscore: ");
		gc_SetTextXY(100+75, 120);
		gc_PrintInt(oldHS,5);
		flicker++;
		if (flicker <35) {
			gc_SetTextXY(85, 160);
			gc_PrintString("Press any key to start ");
			gc_SetTextXY(96, 170);
			gc_PrintString("Press clear to exit");
		}
		if (flicker == 71) {
			flicker = 0;
		}
		gc_NoClipDrawSprite(logo, 110,80,100,31);
		gc_SwapDraw();
		gc_FillScrn(0xFF);
		key = kb_ScanGroup(kb_group_6);
		if (key & kb_Clear) {
			gc_FillScrn(0xFF);
			exitSection = true;
			quit = true;
			while (kb_AnyKey()) {

			}
		}
		if (kb_AnyKey()) {
			gc_FillScrn(0xFF);
			exitSection = true;
			while (kb_AnyKey()) {

			}
		}
	}
	if (!quit) {
		startGame();
	}
	else {
		gc_CloseGraph();
		_OS(asm("CALL _DrawStatusBar"));
	}
}
void setDefaultHS() {
	unsigned int index, file, i;
	/* Close all open file handles before we try any funny business */
	ti_CloseAll();

	/* Open a file for writting -- delete it if it already exists */
	file = ti_Open("tentenhs", "a+");

	if (file) {
		ti_Rewind(file);
		ti_Read(&oldHS, sizeof(int), 1, file);
	}
	else {
		oldHS = 0;
	}
	ti_Close(file);

}
void drawBuffers() {
	int xx, yy;
	if (currentBufferInUse == 1) {
			for (xx = 0; xx < 5; xx++)
			{
				for (yy = 0; yy < 5; yy++)
				{
					if (buffer2.shape[xx][yy] == true) { // if the tile is set 
						gc_SetColorIndex(buffer2.color);
						gc_NoClipRectangle(xx * 21 + 215, yy * 21 + 19, 20, 20);
						fixCorners(xx * 21 + 215, yy * 21 + 19);
					}
				}
			}

			for (xx = 0; xx < 5; xx++)
			{
				for (yy = 0; yy < 5; yy++)
				{
					if (buffer3.shape[xx][yy] == true) { // if the tile is set 
						gc_SetColorIndex(buffer3.color);
						gc_NoClipRectangle(xx * 21 + 215, yy * 21 + 19 +110, 20, 20);
						fixCorners(xx * 21 + 215, yy * 21 + 19 +110);
					}
				}
			}
	}
	if (currentBufferInUse == 2) {
		for (xx = 0; xx < 5; xx++)
		{
			for (yy = 0; yy < 5; yy++)
			{
				if (buffer3.shape[xx][yy] == true) { // if the tile is set 
					gc_SetColorIndex(buffer3.color);
					gc_NoClipRectangle(xx * 21 + 215, yy * 21 + 19, 20, 20);
					fixCorners(xx * 21 + 215, yy * 21 + 19);
				}
			}
		}

		for (xx = 0; xx < 5; xx++)
		{
			for (yy = 0; yy < 5; yy++)
			{
				if (buffer1.shape[xx][yy] == true) { // if the tile is set 
					gc_SetColorIndex(buffer1.color);
					gc_NoClipRectangle(xx * 21 + 215, yy * 21 + 19 + 110, 20, 20);
					fixCorners(xx * 21 + 215, yy  * 21 + 19 + 110);
				}
			}
		}
	}
	if (currentBufferInUse == 3) {
		for (xx = 0; xx < 5; xx++)
		{
			for (yy = 0; yy < 5; yy++)
			{
				if (buffer1.shape[xx][yy] == true) { // if the tile is set 
					gc_SetColorIndex(buffer1.color);
					gc_NoClipRectangle(xx * 21 + 215, yy * 21 + 19, 20, 20);
					fixCorners(xx* 21 + 215, yy * 21 + 19);
				}
			}
		}

		for (xx = 0; xx < 5; xx++)
		{
			for (yy = 0; yy < 5; yy++)
			{
				if (buffer2.shape[xx][yy] == true) { // if the tile is set 
					gc_SetColorIndex(buffer2.color);
					gc_NoClipRectangle(xx * 21 + 215, yy * 21 + 19 + 110, 20, 20);
					fixCorners(xx * 21 + 215, yy * 21 + 19 + 110);
				}
			}
		}
	}
}
void saveNewHS(int newHS) {
	unsigned int index, file, i;
	if (newHS > oldHS) {
		oldHS = newHS;
		/* Close all open file handles before we try any funny business */
		ti_CloseAll();
		/* Open a file for writting -- delete it if it already exists */
		file = ti_Open("tentenhs", "a+");

		if (file) {
			ti_Rewind(file);
			ti_Write(&newHS, sizeof(int), 1, file);
		}
		ti_Close(file);
	}

}
void startGame()
{
	gc_InitGraph();
	gc_DrawBuffer();
	gc_SetTextColor(0xFF00);
	gc_FillScrn(0xFF);
	gc_SwapDraw();
	srand(*(unsigned long*)0xF30044);
	clearTile(&buffer1);
	clearTile(&buffer2);
	clearTile(&buffer3);
	setRandomTile(&buffer1);
	setRandomTile(&buffer2);
	setRandomTile(&buffer3);
	buffer1Active = true;
	buffer2Active = true;
	buffer3Active = true;
	currentBufferInUse = 1;
	exitSection = false;
	movesRemaining = true;
	boardx = 3;
	boardy = 18;
	selectx = 0;
	selecty = 0;
	score = 0;
	drawBoardStart();
	canDrawTileBool = true;
	while (exitSection == false) {
		checkBuf();
		drawBoardRF();
		gc_SetTextXY(275, 5);
		gc_PrintInt(score, 5);
		if (currentBufferInUse == 1) {
			drawTileHover(buffer1, selectx, selecty);
		}
		if (currentBufferInUse == 2) {
			drawTileHover(buffer2, selectx, selecty);
		}
		if (currentBufferInUse == 3) {
			drawTileHover(buffer3, selectx, selecty);
		}
		/*if (canDrawTileBool == true) {
			gc_SetColorIndex(0x47);
			gc_NoClipRectangleOutline(selectx * 21 + boardx, selecty * 21 + boardy, 21, 21);
		}
		else {
			gc_SetColorIndex(0xE0);
			gc_NoClipRectangleOutline(selectx * 21 + boardx, selecty * 21 + boardy, 21, 21);
		}*/
		drawBuffers();
		gc_SwapDraw();
		if (!darkMode) {
			gc_SetColor(0xFF, gc_RGBTo1555(255, 255, 255));
		}
		else {
			gc_SetColor(0xFF, gc_RGBTo1555(26, 26, 26));
		}
		gc_FillScrn(0xFF);
		key = kb_ScanGroup(kb_group_7);
		if (key & kb_Down) {
			if (currentBufferInUse == 1) {
				inBounds(buffer1, selectx, selecty+1);
			}
			if (currentBufferInUse == 2) {
				inBounds(buffer2, selectx, selecty+1);
			}
			if (currentBufferInUse == 3) {
				inBounds(buffer3, selectx, selecty+1);
			}
			if (selecty < 9 && canDrawTileBool) {
				selecty++;
				if (currentBufferInUse == 1) {
					canDrawTile(buffer1, selectx, selecty);
				}
				if (currentBufferInUse == 2) {
					canDrawTile(buffer2, selectx, selecty);
				}
				if (currentBufferInUse == 3) {
					canDrawTile(buffer3, selectx, selecty);
				}
			}
		}
		if (key & kb_Up) {
			if (currentBufferInUse == 1) {
				inBounds(buffer1, selectx, selecty-1);
			}
			if (currentBufferInUse == 2) {
				inBounds(buffer2, selectx, selecty-1);
			}
			if (currentBufferInUse == 3) {
				inBounds(buffer3, selectx, selecty-1);
			}
			if (selecty > 0 && canDrawTileBool) {
				selecty--;
				if (currentBufferInUse == 1) {
					canDrawTile(buffer1, selectx, selecty);
				}
				if (currentBufferInUse == 2) {
					canDrawTile(buffer2, selectx, selecty);
				}
				if (currentBufferInUse == 3) {
					canDrawTile(buffer3, selectx, selecty);
				}
			}
		}
		if (key & kb_Left) {
			if (currentBufferInUse == 1) {
				inBounds(buffer1, selectx-1, selecty);
			}
			if (currentBufferInUse == 2) {
				inBounds(buffer2, selectx-1, selecty);
			}
			if (currentBufferInUse == 3) {
				inBounds(buffer3, selectx-1, selecty);
			}
			if (selectx > 0 && canDrawTileBool) {
				selectx--;
				if (currentBufferInUse == 1) {
					canDrawTile(buffer1, selectx, selecty);
				}
				if (currentBufferInUse == 2) {
					canDrawTile(buffer2, selectx, selecty);
				}
				if (currentBufferInUse == 3) {
					canDrawTile(buffer3, selectx, selecty);
				}
			}
		}
		if (key & kb_Right) {
			if (currentBufferInUse == 1) {
				inBounds(buffer1, selectx+1, selecty);
			}
			if (currentBufferInUse == 2) {
				inBounds(buffer2, selectx+1, selecty);
			}
			if (currentBufferInUse == 3) {
				inBounds(buffer3, selectx+1, selecty);
			}
			if (selectx < 9 && canDrawTileBool) {
				selectx++;
				if (currentBufferInUse == 1) {
					canDrawTile(buffer1, selectx, selecty);
				}
				if (currentBufferInUse == 2) {
					canDrawTile(buffer2, selectx, selecty);
				}
				if (currentBufferInUse == 3) {
					canDrawTile(buffer3, selectx, selecty);
				}
			}
		}
		if (selectx > 9 || selectx < 0)
		{
			selectx = 0;
		}
		if (selecty > 9 || selecty < 0)
		{
			selecty = 0;
		}
		key = kb_ScanGroup(kb_group_6);
		if (key & kb_Clear) {
			exitSection = true;
		}
		key = kb_ScanGroup(kb_group_1);
		if (key & kb_2nd) {
			if (currentBufferInUse == 1) {
				canDrawTile(buffer1, selectx, selecty);
			}
			if (currentBufferInUse == 2) {
				canDrawTile(buffer2, selectx, selecty);
			}
			if (currentBufferInUse == 3) {
				canDrawTile(buffer3, selectx, selecty);
			}
			if (canDrawTileBool) {
				if (currentBufferInUse == 1) {
					drawTile(buffer1, selectx, selecty);
				}
				if (currentBufferInUse == 2) {
					drawTile(buffer2, selectx, selecty);
				}
				if (currentBufferInUse == 3) {
					drawTile(buffer3, selectx, selecty);
				}

				if (currentBufferInUse == 1) {
					buffer1Active = false;
					clearTile(&buffer1);
				}
				if (currentBufferInUse == 2) {
					buffer2Active = false;
					clearTile(&buffer2);
				}
				if (currentBufferInUse == 3) {
					buffer3Active = false;
					clearTile(&buffer3);
				}
				currentBufferInUse++;
				if (currentBufferInUse == 4) {
					currentBufferInUse = 1;
				}
				checkBuf();
				checkForTileRows();
				if (currentBufferInUse == 1) {
					canDrawTile(buffer1, selectx, selecty);
				}
				if (currentBufferInUse == 2) {
					canDrawTile(buffer2, selectx, selecty);
				}
				if (currentBufferInUse == 3) {
					canDrawTile(buffer3, selectx, selecty);
				}
				anyMovesLeft();
				if (!movesRemaining) {
					exitSection = true;
				}
			}
		}
		if (key & kb_Mode) {
			darkMode = !darkMode;
			while (kb_AnyKey()) {

			}
		}
		key = kb_ScanGroup(kb_group_2);
		if (key & kb_Alpha) {
			currentBufferInUse++;
			if (currentBufferInUse == 4) {
				currentBufferInUse = 1;
			}
			checkBuf();
			dbg_printf(dbgout, "cbiu %d", currentBufferInUse);
			while (kb_AnyKey()) {

			}
		}
	}
		saveNewHS(score);
		dead();
}

void dead()
{
	while (kb_AnyKey()) {

	}
	exitSection = false;
	while (exitSection == false) {
		drawBoardRF();
		drawBuffers();
		if (!darkMode) {
			gc_SetColor(0xFF, gc_RGBTo1555(255, 255, 255));
		}
		else {
			gc_SetColor(0xFF, gc_RGBTo1555(26, 26, 26));
		}

		key = kb_ScanGroup(kb_group_6);
		if (key & kb_Clear) {
			exitSection = true;
		}

		key = kb_ScanGroup(kb_group_1);
		if (!(key & kb_Mode)) {
			gc_SetColorIndex(0x73);
			gc_NoClipRectangle(0,40, 320, 40);
			gc_SetColor(0x78, gc_RGBTo1555(255, 255, 255));
			gc_SetTextColor(0x7378);
			gc_SetTextXY(125, 50);
			gc_PrintString("Game Over");
			gc_SetTextXY(139, 60);
			gc_PrintInt(score, 5);
		}
		if (key & kb_2nd) {
			exitSection = true;
			quit = true;
		}
		gc_SwapDraw();
		gc_FillScrn(0xFF);
	}
	if (quit) {
		quit = false;
		startGame();
	}
	else {
		main();
	}
}
void checkBuf() {
	if (!buffer1Active && !buffer2Active && !buffer3Active) {
		clearTile(&buffer1);
		clearTile(&buffer2);
		clearTile(&buffer3);
		setRandomTile(&buffer1);
		setRandomTile(&buffer2);
		setRandomTile(&buffer3);
		buffer1Active = true;
		buffer2Active = true;
		buffer3Active = true;
	}
		if (currentBufferInUse == 1 && !buffer1Active) {
			currentBufferInUse++; 
			checkBuf();
		}
		if (currentBufferInUse == 2 && !buffer2Active) {
			currentBufferInUse++;
			checkBuf();
		}
		if (currentBufferInUse == 3 && !buffer3Active) {
			currentBufferInUse=1;
			checkBuf();
		}
		fixBounds();
}
void fixBounds() {
	if (currentBufferInUse == 1) {
		inBounds(buffer1, selectx, selecty);
	}
	if (currentBufferInUse == 2) {
		inBounds(buffer2, selectx, selecty);
	}
	if (currentBufferInUse == 3) {
		inBounds(buffer3, selectx, selecty);
	}
	if (!canDrawTileBool) {
		selectx--;
		selecty--;
		fixBounds();
	}
}
void clearTile(Tile *t) {
	createEmpty(t);
}
void anyMovesLeft() {
	int xx, yy;
	bool movesRemainingSet = false;
	for (xx = 0; xx < 10; xx++)
	{
		for (yy = 0; yy < 10; yy++)
		{
			canDrawTile(buffer1, xx, yy);
			if (canDrawTileBool && buffer1Active) {
				dbg_printf(dbgout, "Move remaining ");
				movesRemaining = true;
				movesRemainingSet = true;
			}
			canDrawTile(buffer2, xx, yy);
			if (canDrawTileBool && buffer2Active) {
				dbg_printf(dbgout, "Move remaining ");
				movesRemaining = true;
				movesRemainingSet = true;
			}
			canDrawTile(buffer3, xx, yy);
			if (canDrawTileBool && buffer3Active) {
				dbg_printf(dbgout, "Move remaining ");
				movesRemaining = true;
				movesRemainingSet = true;
			}
		}
	}
	if (!movesRemainingSet) {
		movesRemaining = false;
	}
	dbg_printf(dbgout, "No moves");
}
void setRandomTile(Tile *t) {
	int r = rand() % 13;
	switch (r) {
	default:
		createOne(t);
		break;
	case 0:
		createOne(t);
		break;
	case 1:
		createTwoHorizontal(t);
		break;
	case 2:
		createTwoVertical(t);
		break;
	case 3:
		createThreeHorizontal(t);
		break;
	case 4:
		createThreeVertical(t);
		break;
	case 5:
		createFourHorizontal(t);
		break;
	case 6:
		createFourVertical(t);
		break;
	case 7:
		createFiveHorizontal(t);
		break;
	case 8:
		createFiveVertical(t);
		break;
	case 9:
		create2Sq(t);
		break;
	case 10:
		create3Sq(t);
		break;
	case 11:
	    whichL = rand() % 4;
		if (whichL == 0) {
			create3L1(t);
		}
		if (whichL == 1) {
			create3L2(t);
		}
		if (whichL == 2) {
			create3L3(t);
		}
		if (whichL == 3) {
			create3L4(t);
		}
		break;
	case 12:
		whichL = rand() % 4;
		if (whichL == 0) {
			create5L1(t);
		}
		if (whichL == 1) {
			create5L2(t);
		}
		if (whichL == 2) {
			create5L3(t);
		}
		if (whichL == 3) {
			create5L4(t);
		}
		break;
	}
}

void drawTileHover(struct Tile t, int tx, int ty) {
	int tn = 0;
	int xn = tx;
	int yn = ty;

	int xx, yy;
	for (xx = 0; xx < 5; xx++)
	{
		for (yy = 0; yy < 5; yy++)
		{
			if (t.shape[xx][yy] == true) { // if the tile is set 
				gc_SetColorIndex(t.color);
				gc_NoClipRectangle((xx + xn) * 21 + boardx, (yy + yn) * 21 + boardy, 20, 20);
				fixCorners((xx + xn) * 21 + boardx, (yy + yn) * 21 + boardy);
			}
		}
	}
}
void checkForTileRows() {
	bool columnsClear[10];
	bool rowsClear[10];

	int z;
	for (z = 0; z < 10; z++) {
		checkColumn(z);
		columnsClear[z] = lastClearCheck;
	}

	for (z = 0; z < 10; z++) {
		checkRow(z);
		rowsClear[z] = lastClearCheck;
	}

	for (z = 0; z < 10; z++) {
		if (columnsClear[z]) {
			clearColumn(z);
		}
	}

	for (z = 0; z < 10; z++) {
		if (rowsClear[z]) {
			clearRow(z);
		}
	}
}
bool checkRow(int y) {
	bool yesRow = true;
	int xx;
	for (xx = 0; xx < 10; xx++)
	{
		if (tiles[xx][y] == 0x00) { // if the tile isnt set 
			yesRow = false;
		}
	}
	if (yesRow) {
		dbg_printf(dbgout, "going to clear row %d", y);
	}
	lastClearCheck = yesRow;
	return yesRow;
}
bool checkColumn(int x) {
	bool yesRow = true;
	int yy;
	for (yy = 0; yy < 10; yy++)
	{
		if (tiles[x][yy] == 0x00) { // if the tile isnt set 
			yesRow = false;
		}

	}
	if (yesRow) {
		dbg_printf(dbgout, "going to clear column %d", x);
	}
	lastClearCheck = yesRow;
	return yesRow;
}

void clearRow(int y) {
		int xx;
		for (xx = 0; xx < 10; xx++)
		{
			dbg_printf(dbgout, " clearing row %d", y);
			tiles[xx][y] = 0x00;
		}
}
void clearColumn(int x) {
		int yy;
		for (yy = 0; yy < 10; yy++)
		{
			dbg_printf(dbgout, " clearing column %d", x);
			tiles[x][yy] = 0x00;
		}
}
void drawTile(struct Tile t, int tx, int ty) {
	int tn = 0;
	int xn = tx;
	int yn = ty;

	int xx, yy;
	for (xx = 0; xx < 5; xx++)
	{
		for (yy = 0; yy < 5; yy++)
		{
			if (t.shape[xx][yy] == true) {
				tiles[xx+tx][yy+ty] = t.color;
				score++;
			}
		}
	}
}

bool canDrawTile(struct Tile t,int tx,int ty) {
	int tn = 0;
	int xn = tx++;
	int yn = ty++;
	bool allowDraw = true;

	int xx, yy;
	for (xx = 0; xx < 5; xx++)
	{
		for (yy = 0; yy < 5; yy++)
		{
			if (t.shape[xx][yy] == true) { // if the tile is set 
				if ((xx + xn) > 9 || (yy + yn) > 9) { //is it out of bounds?
					allowDraw = false;
				}
				if (tiles[xx + xn][yy + yn] != 0x00) { //is there already a tile here?
					allowDraw = false;
				}
			}
		}
	}
	canDrawTileBool = allowDraw;
	return allowDraw;
}
bool inBounds(struct Tile t, int tx, int ty) {
	int tn = 0;
	int xn = tx++;
	int yn = ty++;
	bool allowDraw = true;

	int xx, yy;
	for (xx = 0; xx < 5; xx++)
	{
		for (yy = 0; yy < 5; yy++)
		{
			if (t.shape[xx][yy] == true) { // if the tile is set 
				if ((xx + xn) > 9 || (yy + yn) > 9) { //is it out of bounds?
					allowDraw = false;
				}
			}
		}
	}
	canDrawTileBool = allowDraw;
	return allowDraw;
}
void drawBoardStart() {
	tilerow = 0;
	tilecolumn = 0;
	tilenumber = 0;
	while (tilenumber < 100) {
		tilenumber++;
		gc_SetColor(0x00, gc_RGBTo1555(230, 230, 230));
		gc_SetColorIndex(0x00);
		gc_NoClipRectangle(tilecolumn*21+boardx, tilerow*21+boardy,20,20);
		fixCorners(tilecolumn * 21 + boardx, tilerow * 21 + boardy);
		tiles[tilecolumn][tilerow] = 0x00;
		tilecolumn++;
		if (tilecolumn == 10) {
			tilecolumn = 0;
			tilerow++;
		}
	}

}
void drawBoardRF() {
	tilerow = 0;
	tilecolumn = 0;
	tilenumber = 0;
	while (tilenumber < 100) {
		tilenumber++;
		if (!darkMode) {
			gc_SetColor(0x00, gc_RGBTo1555(230, 230, 230));
		}
		else {
			gc_SetColor(0x00, gc_RGBTo1555(51, 51, 51));
		}
		gc_SetColorIndex(tiles[tilecolumn][tilerow]);
		gc_NoClipRectangle(tilecolumn * 21 + boardx, tilerow * 21 + boardy, 20, 20);
		fixCorners(tilecolumn * 21 + boardx, tilerow * 21 + boardy);
		tilecolumn++;
		if (tilecolumn == 10) {
			tilecolumn = 0;
			tilerow++;
		}
	}

}

void fixCorners(int x, int y) {
	gc_SetColorIndex(0xFF);
	gc_ClipSetPixel(x, y);
	gc_ClipSetPixel(x + 1, y);
	gc_ClipSetPixel(x + 2, y);
	gc_ClipSetPixel(x + 3, y);
	gc_ClipSetPixel(x, y + 1);
	gc_ClipSetPixel(x, y + 2);
	gc_ClipSetPixel(x, y + 3);
	gc_ClipSetPixel(x + 1, y + 1);
	gc_ClipSetPixel(x + 2, y + 1);
	gc_ClipSetPixel(x + 1, y + 2);

	gc_ClipSetPixel(x, y+20);
	gc_ClipSetPixel(x + 1, y+20);
	gc_ClipSetPixel(x + 2, y + 20);
	gc_ClipSetPixel(x + 3, y + 20);
	gc_ClipSetPixel(x, y + 19);
	gc_ClipSetPixel(x, y + 18);
	gc_ClipSetPixel(x, y + 17);
	gc_ClipSetPixel(x + 1, y + 19);
	gc_ClipSetPixel(x + 2, y + 19);
	gc_ClipSetPixel(x + 1, y + 18);

	gc_ClipSetPixel(x + 20, y);
	gc_ClipSetPixel(x + 20, y + 1);
	gc_ClipSetPixel(x + 20, y + 2);
	gc_ClipSetPixel(x + 20, y + 3);
	gc_ClipSetPixel(x + 19, y);
	gc_ClipSetPixel(x + 18, y);
	gc_ClipSetPixel(x + 17, y );
	gc_ClipSetPixel(x + 19, y + 1);
	gc_ClipSetPixel(x + 19, y + 2);
	gc_ClipSetPixel(x + 18, y + 1);

	gc_ClipSetPixel(x + 20, y + 20);
	gc_ClipSetPixel(x + 20, y + 19);
	gc_ClipSetPixel(x + 20, y + 18);
	gc_ClipSetPixel(x + 20, y + 17);
	gc_ClipSetPixel(x + 19, y + 20);
	gc_ClipSetPixel(x + 18, y + 20);
	gc_ClipSetPixel(x + 17, y + 20);
	gc_ClipSetPixel(x + 19, y + 19);
	gc_ClipSetPixel(x + 19, y + 18);
	gc_ClipSetPixel(x + 18, y + 19);
}

