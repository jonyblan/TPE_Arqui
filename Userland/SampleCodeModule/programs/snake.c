#include "arquilib.h"
#include <stdint.h>
#include "libasm.h"

#define SCREEN_SIZE 600

#define CANT_BLOCKS 30
#define CANT_FOODS 1

#define BLOCK_SIZE (SCREEN_SIZE / CANT_BLOCKS)

#define NOTHING_CODE 0
#define FOOD_CODE 4096
#define SNAKE_CODE 2
#define CODE_LEFT 3
#define CODE_RIGHT 4
#define CODE_UP 5
#define CODE_DOWN 6
#define HORIZONTAL_CODE 7
#define VERTICAL_CODE 8
#define BOTTOM_RIGHT_CODE 10
#define BOTTOM_LEFT_CODE 11
#define TOP_RIGHT_CODE 12
#define TOP_LEFT_CODE 13

#define UP1 14
#define DOWN1 15
#define RIGHT1 16
#define LEFT1 18
#define UP2 19
#define DOWN2 20
#define RIGHT2 21
#define LEFT2 23


#define TAIL_CODE 32 // these 2 have to be trunkated with code_left/code_right/code_up/code_down to work properly
#define HEAD_CODE1 64
#define HEAD_CODE2 2048

#define IGNORE_CODE 128

#define FOOD_COLLITION_CODE 256
#define SNAKE_COLLITION_CODE 512
#define WALL_COLLITION_CODE 1024

#define EMPTY_EVEN_COL 0x91FF33
#define EMPTY_ODD_COL 0x7EE02B
#define SNAKE_COL

uint8_t pseudoRandom = 98;

typedef struct {
	int lifeTime;
	int code;	
} BodyPart;

typedef struct {
	int size;
	int dir;
} Player;

void drawFullBlock(int x, int y, uint32_t color){
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			putPixel(color, i, j);
		}
	}
}

void drawEmpty(int x, int y){
	uint32_t col;
	if((x+y)%2){
		col = EMPTY_EVEN_COL;
	}
	else{
		col = EMPTY_ODD_COL;
	}
	drawFullBlock(x, y, col);
}

void drawFood(int x, int y){
	if(BLOCK_SIZE != 20){
		drawFullBlock(x, y, 0x00FF0000);
		return;
	}
	drawEmpty(x, y);
	drawApple(x, y);
}


void drawSnake(int x, int y, int code){
	drawHorizontal(x, y);
	return ;
	if(code & HEAD_CODE1 != 0){
		switch(code - HEAD_CODE1){
			case 1: drawLeft(x, y); break;
			case 2: drawRight(x, y); break;
			case 3: drawUp(x, y); break;
			case 4: drawDown(x, y); break;
		}
	}
	else if(code & HEAD_CODE2 != 0){
		switch(code - HEAD_CODE1){
			case 1: drawLeft(x, y); break;
			case 2: drawRight(x, y); break;
			case 3: drawUp(x, y); break;
			case 4: drawDown(x, y); break;
		}
	}
	else if(code & TAIL_CODE != 0){
		switch(code - HEAD_CODE1){
			case 1: drawRight(x, y); break;
			case 2: drawLeft(x, y); break;
			case 3: drawDown(x, y); break;
			case 4: drawUp(x, y); break;
		}
	}
	else{
		switch(code){
			case HORIZONTAL_CODE: drawHorizontal(x, y); break;
			case VERTICAL_CODE: drawVertical(x, y); break;
			case BOTTOM_RIGHT_CODE: drawBottomRight(x, y); break;
			case BOTTOM_LEFT_CODE: drawBottomLeft(x, y); break;
			case TOP_RIGHT_CODE: drawTopRight(x, y); break;
			case TOP_LEFT_CODE: drawTopLeft(x, y); break;
		}
	}
	drawFullBlock(x, y, 0x0000FF);
}

void drawBlock(int x, int y, int code){
	if(code == NOTHING_CODE){
		drawEmpty(x, y);
	}
	else if(code == FOOD_CODE){
		drawFood(x, y);
	}
	else{
		drawSnake(x, y, code);
	}
}

void draw(BodyPart board[CANT_BLOCKS][CANT_BLOCKS]){
	for(int i = 0; i < CANT_BLOCKS; i++){
		for(int j = 0; j < CANT_BLOCKS; j++){
			drawBlock(i, j, board[i][j].code);
		}
	}
}

int charToCode(char c){
	if(c == 'w'){
		return UP1;
	}
	if(c == 'a'){
		return LEFT1;
	}
	if(c == 's'){
		return DOWN1;
	}
	if(c == 'd'){
		return RIGHT1;
	}
	if(c == 'i'){
		return UP2;
	}
	if(c == 'j'){
		return LEFT2;
	}
	if(c == 'k'){
		return DOWN2;
	}
	if(c == 'l'){
		return RIGHT2;
	}
	return IGNORE_CODE;
}

void analizeKeyPressed1(Player *player1){
	char c = getChar();
	int move = charToCode(c);
	// head cant go backwards
	if(	(move == IGNORE_CODE) || 
		((player1->dir + move) == (UP1 + DOWN1)) || 
		((player1->dir + move) == (LEFT1 + RIGHT1))  ){
		return;
	}
	player1->dir = move;
}

void analizeKeyPressed2(Player *player1){
	char c = getChar();
	int move = charToCode(c);
	// head cant go backwards
	if(	(move == IGNORE_CODE) || 
		((player1->dir + move) == (UP2 + DOWN2)) || 
		((player1->dir + move) == (LEFT2 + RIGHT2))  ){
		return;
	}
	player1->dir = move;
}

int dirToX(int dirCode){
	if((dirCode == LEFT1) || (dirCode == LEFT2)){
		return -1;
	}
	if((dirCode == RIGHT1) || (dirCode == RIGHT2)){
		return 1;
	}
	return 0;
}

int dirToY(int dirCode){
	if((dirCode == UP1) || (dirCode == UP2)){
		return -1;
	}
	if((dirCode == DOWN1) || (dirCode == DOWN2)){
		return 1;
	}
	return 0;
}

int analizeWallCollition(int newXHead, int newYHead){
	if(newXHead >= CANT_BLOCKS || newXHead < 0 || newYHead >= CANT_BLOCKS || newYHead < 0){
		return 1;
	}
	return 0;	
}

void generateNewFood(BodyPart board[CANT_BLOCKS][CANT_BLOCKS]){
	int newX = pseudoRandom % 20;
	pseudoRandom *= 3;
	int newY = pseudoRandom % 20;
	pseudoRandom *= 3;
	while(board[newX][newY].code != NOTHING_CODE){ // a lot of lag when snake is big
		newX = pseudoRandom % 20;
		pseudoRandom *= 3;
		newY = pseudoRandom % 20;
		pseudoRandom *= 3;
	}

	board[newX][newY].code = FOOD_CODE;
}

int analizeFoodCollition(BodyPart board[CANT_BLOCKS][CANT_BLOCKS], int newXHead, int newYHead){
	if(board[newXHead][newYHead].code == FOOD_CODE){
		generateNewFood(board);
		return 1;
	}
	return 0;
}

int analizeBodyCollition(BodyPart board[CANT_BLOCKS][CANT_BLOCKS], int newXHead, int newYHead){
	if((board[newXHead][newYHead].code != NOTHING_CODE) && (board[newXHead][newYHead].code != FOOD_CODE)){
		return 1;
	}
	return 0;
}

int analizeColitions(BodyPart board[CANT_BLOCKS][CANT_BLOCKS], Player *player1, int xHead, int yHead){
	int ret = 0;
	int newXHead = xHead + dirToX(player1->dir);
	int newYHead = yHead + dirToY(player1->dir);
	ret += FOOD_COLLITION_CODE * analizeFoodCollition(board, newXHead, newYHead);
	ret += WALL_COLLITION_CODE * analizeWallCollition(newXHead, newYHead);
	ret += SNAKE_COLLITION_CODE * analizeBodyCollition(board, newXHead, newYHead);
	return ret;
}

int update1(BodyPart board[CANT_BLOCKS][CANT_BLOCKS], Player *player1){
	pseudoRandom *= 3;
	analizeKeyPressed1(player1);
	int ret;
	int cont = 1;

	for(int i = 0; (i < CANT_BLOCKS) && (cont == 1); i++){
		for(int j = 0; (j < CANT_BLOCKS) && (cont == 1); j++){
			if((board[i][j].code & HEAD_CODE1) != 0){	
				ret = analizeColitions(board, player1, i, j);
				int newXHead = i + dirToX(player1->dir);
				int newYHead = j + dirToY(player1->dir);
				board[newXHead][newYHead].code = HEAD_CODE1 | CODE_RIGHT;
				board[newXHead][newYHead].lifeTime = player1->size;
				
				board[i][j].code = HORIZONTAL_CODE;
				cont = 0;
			}
		}
	}

	if((ret & FOOD_COLLITION_CODE) != 0){
		player1->size++;
	}
		
	for(int i = 0; i < CANT_BLOCKS; i++){
		for(int j = 0; j < CANT_BLOCKS; j++){
			if((board[i][j].code == NOTHING_CODE) || (board[i][j].code == FOOD_CODE)){
				continue;
			}

			if((ret & FOOD_COLLITION_CODE) == 0){
				board[i][j].lifeTime--;
			}

			if(board[i][j].lifeTime == 0){
				board[i][j].code = NOTHING_CODE;
			}
		}
	}

	return ret;		
}

int update2(BodyPart board[CANT_BLOCKS][CANT_BLOCKS], Player *player1){
	pseudoRandom *= 3;
	analizeKeyPressed2(player1);
	int ret;
	int cont = 1;

	for(int i = 0; (i < CANT_BLOCKS) && (cont == 1); i++){
		for(int j = 0; (j < CANT_BLOCKS) && (cont == 1); j++){
			if((board[i][j].code & HEAD_CODE2) != 0){	
				ret = analizeColitions(board, player1, i, j);
				int newXHead = i + dirToX(player1->dir);
				int newYHead = j + dirToY(player1->dir);
				board[newXHead][newYHead].code = HEAD_CODE2 | CODE_RIGHT;
				board[newXHead][newYHead].lifeTime = player1->size;
				board[i][j].code = HORIZONTAL_CODE;
				cont = 0;
			}
		}
	}

	if((ret & FOOD_COLLITION_CODE) != 0){
		player1->size++;
	}

	return ret;		
}

int waitForStart(){
	char c;
	int ret = IGNORE_CODE;
	while(ret == IGNORE_CODE){
		c = getChar();
		ret = charToCode(c);
	}
	return ret;
}

void snake(){
	int counter = 0;
	static BodyPart board[CANT_BLOCKS][CANT_BLOCKS];
	static Player player1 = {3, 0};
	static Player player2 = {3, 0};
	for(int i = 0; i < CANT_BLOCKS; i++){
		for(int j = 0; j < CANT_BLOCKS; j++){
			board[i][j] = (BodyPart){0, NOTHING_CODE};
		}
	}
	board[7][5] = (BodyPart){3, HEAD_CODE1|CODE_RIGHT};
	board[6][5] = (BodyPart){2, HORIZONTAL_CODE};
	board[5][5] = (BodyPart){1, TAIL_CODE|CODE_RIGHT};
	board[7][9] = (BodyPart){3, HEAD_CODE2|CODE_RIGHT};
	board[6][9] = (BodyPart){2, HORIZONTAL_CODE};
	board[5][9] = (BodyPart){1, TAIL_CODE|CODE_RIGHT};
	board[12][7] = (BodyPart){0, FOOD_CODE};

	draw(board);

	/*
	drawVertical(0, 0);
	drawHorizontal(0, 1);
	drawUp(0, 2);
	drawDown(0, 3);
	drawLeft(0, 4);
	drawRight(0, 5);
	drawBottomLeft(0, 6);
	drawTopLeft(0, 7);
	drawTopRight(0, 8);
	drawBottomRight(0, 9);
	*/
	
	drawDown(0, 0);
	drawVertical(0, 1);
	drawBottomLeft(0, 2);
	drawHorizontal(1, 2);
	drawTopRight(2, 2);
	drawVertical(2, 3);
	drawBottomRight(2, 4);
	drawTopLeft(1, 4);
	drawUp(1, 5);
	
	player1.dir = waitForStart();
	player2.dir = waitForStart();
	int ret1 = 0;
	int ret2 = 0;
	int cont = 1;

	while(cont == 1){
		ret1 = update1(board, &player1);
		//ret2 = update2(board, &player2);
		draw(board);
		counter++;
		if(((ret1 & SNAKE_COLLITION_CODE) != 0) || ((ret1 & WALL_COLLITION_CODE) != 0)){
			cont = 0;
		}
	}
	clear();
}

void drawApple(x, y){
	drawFullBlock(x, y, 0xFF0000);
	return ;
	uint32_t red = 0x00FF0000;
	uint32_t green = 0x0000FF00;
	uint32_t black = 0x00000000;
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
}

void drawVertical(x, y){
	int matrix[20][20] = 
		{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	
	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}


void drawUp(x, y){
	int matrix[20][20] = 
		{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	
	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}

void drawDown(x, y){
	int matrix[20][20] = 
		{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	
	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}

void drawLeft(x, y){
	int matrix[20][20] = {
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	
	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}

void drawHorizontal(x, y){
	int matrix[20][20] = {
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}};
	
	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}

void drawRight(x, y){
	int matrix[20][20] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}};
	
	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}

void drawBottomLeft(x, y){
	int matrix[20][20] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}};
	
	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}

void drawTopLeft(int x, int y){
	int matrix[20][20] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}};


	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}

void drawTopRight(x, y){
	int matrix[20][20] = {
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	
	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}

void drawBottomRight(x, y){
	int matrix[20][20] = {
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	
	drawEmpty(x, y);
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
	for(int i = trueX; i < trueX + BLOCK_SIZE; i++){
		for(int j = trueY; j < trueY + BLOCK_SIZE; j++){
			if(matrix[i-trueX][j-trueY] == 1){
				putPixel(0x000000FF, i, j);
			}
		}
	}
}