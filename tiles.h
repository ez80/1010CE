struct Tile
{
	bool shape[5][5];
	unsigned char color;
} tile;
typedef struct
{
	bool shape[5][5];
	unsigned char color;
} Tile;
void createEmpty(Tile *t) {
	t->color = 0x00;
	t->shape[0][0] = false;
	t->shape[0][1] = false;
	t->shape[0][2] = false;
	t->shape[0][3] = false;
	t->shape[0][4] = false;
	t->shape[1][0] = false;
	t->shape[1][1] = false;
	t->shape[1][2] = false;
	t->shape[1][3] = false;
	t->shape[1][4] = false;
	t->shape[2][0] = false;
	t->shape[2][1] = false;
	t->shape[2][2] = false;
	t->shape[2][3] = false;
	t->shape[2][4] = false;
	t->shape[3][0] = false;
	t->shape[3][1] = false;
	t->shape[3][2] = false;
	t->shape[3][3] = false;
	t->shape[3][4] = false;
	t->shape[4][0] = false;
	t->shape[4][1] = false;
	t->shape[4][2] = false;
	t->shape[4][3] = false;
	t->shape[4][4] = false;
}
void createOne(Tile *t) {
	t->color = 0x73;
	t->shape[0][0] = true;
}
void createTwoHorizontal(Tile *t) {
	t->color = 0xE5;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
}
void createTwoVertical(Tile *t) {
	
	t->color = 0xE5;
	t->shape[0][0] = true;
	t->shape[0][1] = true;
}

void createThreeHorizontal(Tile *t) {
	
	t->color = 0xE3;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
	t->shape[2][0] = true;
}
void createThreeVertical(Tile *t) {
	
	t->color = 0xE3;
	t->shape[0][0] = true;
	t->shape[0][1] = true;
	t->shape[0][2] = true;
}

void createFourHorizontal(Tile *t) {
	
	t->color = 0xF3;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
	t->shape[2][0] = true;
	t->shape[3][0] = true;
	
}
void createFourVertical(Tile *t) {
	
	t->color = 0xF3;
	t->shape[0][0] = true;
	t->shape[0][1] = true;
	t->shape[0][2] = true;
	t->shape[0][3] = true;
	
}
void createFiveHorizontal(Tile *t) {
	t->color = 0xE1;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
	t->shape[2][0] = true;
	t->shape[3][0] = true;
	t->shape[4][0] = true;
	
}
void createFiveVertical(Tile *t) {
	
	t->color = 0xE1;
	t->shape[0][0] = true;
	t->shape[0][1] = true;
	t->shape[0][2] = true;
	t->shape[0][3] = true;
	t->shape[0][4] = true;
	
}
void create2Sq(Tile *t) {
	
	t->color = 0xA7;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
	t->shape[0][1] = true;
	t->shape[1][1] = true;
	
}
void create3Sq(Tile *t) {
	
	t->color = 0x77;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
	t->shape[0][1] = true;
	t->shape[2][0] = true;
	t->shape[0][2] = true;
	t->shape[1][1] = true;
	t->shape[2][1] = true;
	t->shape[1][2] = true;
	t->shape[2][2] = true;
	
}
void create3L1(Tile *t) {
	
	t->color = 0x6F;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
	t->shape[0][1] = true;

	
}
void create3L2(Tile *t) {
	
	t->color = 0x6F;
	t->shape[0][1] = true;
	t->shape[1][1] = true;
	t->shape[1][0] = true;
	
}
void create3L3(Tile *t) {
	
	t->color = 0x6F;
	t->shape[0][0] = true;
	t->shape[0][1] = true;
	t->shape[1][1] = true;
	
}
void create3L4(Tile *t) {
	
	t->color = 0x6F;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
	t->shape[1][1] = true;
	
}

void create5L1(Tile *t) {
	
	t->color = 0x3E;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
	t->shape[0][1] = true;
	t->shape[0][2] = true;
	t->shape[2][0] = true;
}
void create5L2(Tile *t) {
	
	t->color = 0x3E;
	t->shape[2][0] = true;
	t->shape[2][1] = true;
	t->shape[2][2] = true;
	t->shape[1][2] = true;
	t->shape[0][2] = true;
}
void create5L3(Tile *t) {
	
	t->color = 0x3E;
	t->shape[0][0] = true;
	t->shape[1][0] = true;
	t->shape[2][0] = true;
	t->shape[2][1] = true;
	t->shape[2][2] = true;
}
void create5L4(Tile *t) {
	
	t->color = 0x3E;
	t->shape[2][2] = true;
	t->shape[1][2] = true;
	t->shape[0][2] = true;
	t->shape[0][0] = true;
	t->shape[0][1] = true;
}