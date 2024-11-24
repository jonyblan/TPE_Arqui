#include "arquilib.h"
#include <stdint.h>
#include "libasm.h"
#include "time.h"

#define SCREEN_SIZE 600

#define CANT_BLOCKS 30
#define CANT_FOODS 1

#define BLOCK_SIZE (SCREEN_SIZE / CANT_BLOCKS)

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

#define AUX 525

#define UP1 14
#define DOWN1 15
#define RIGHT1 16
#define LEFT1 18
#define UP2 19
#define DOWN2 20
#define RIGHT2 21
#define LEFT2 23


#define IGNORE_CODE 256

#define PLAYER1_CODE 512
#define PLAYER2_CODE 1024

#define TAIL_CODE 32//(32 | PLAYER1_CODE) // these 2 have to be trunkated with code_left/code_right/code_up/code_down to work properly
#define HEAD_CODE1 576//(64 | PLAYER1_CODE)
#define HEAD_CODE2 128//(128 | PLAYER2_CODE)

#define NOTHING_CODE 0
#define FOOD_CODE 2048

#define FOOD_COLLITION_CODE 1
#define SNAKE_COLLITION_CODE 2
#define WALL_COLLITION_CODE 4

#define EMPTY_EVEN_COL 0x91FF33
#define EMPTY_ODD_COL 0x7EE02B
#define FOOD_COL 0x00FF0000
#define SNAKE1_COL 0x0000B33C
#define SNAKE2_COL 0x00006600
#define HEAD_COL 0x00000000
#define EYE_COL 0x00FFFFFF
#define ERROR_COL 0x00FF00FF
#define LETTER_COL 0xFFF200

#define BOTH_LOSE_CODE 1
#define PLAYER1_WINS_CODE 2
#define PLAYER2_WINS_CODE 3

#define CANT_PLAYERS 2

uint8_t pseudoRandom = 98;

typedef struct {
	int lifeTime;
	int code;	
} BodyPart;

typedef struct {
	int size; // SIZE = SCORE + 1
	int dir;
	int previousDir;
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

void drawLetter(int x, int y){
		drawFullBlock(x, y, LETTER_COL);
}

void drawFood(int x, int y){
	if(BLOCK_SIZE != 20){
		drawFullBlock(x, y, FOOD_COL);
		return;
	}
	drawEmpty(x, y);
	drawApple(x, y);
}

void drawSnake(int x, int y, int code){
	if(BLOCK_SIZE != 20){
		int col;
		if((code & PLAYER1_CODE) != 0){
			col = SNAKE1_COL;
		}
		else if((code & PLAYER2_CODE) != 0){
			col = SNAKE2_COL;
		}
		else{
			col = ERROR_COL;
		}
		drawFullBlock(x, y, col);
		return ;	
	}
	// these 3 & are not used with the usual != 0
	// as head_code1 contains both head_code and player1_code
	// which causes a problem when bodyparts are
	// assigned a code such as horizontal_code | player1_code
	// TODO fix it later, its just ugly
	// also, 2 ifs for heads, and 2 ifs for body, just why
	if((code & HEAD_CODE1) == HEAD_CODE1){
		switch(code ^ HEAD_CODE1){
			case CODE_LEFT: drawLeft(x, y, HEAD_COL); break;
			case CODE_RIGHT: drawRight(x, y, HEAD_COL); break;
			case CODE_UP: drawUp(x, y, HEAD_COL); break;
			case CODE_DOWN: drawDown(x, y, HEAD_COL); break;
			default: drawFullBlock(x, y, ERROR_COL);
		}
		return ;
	}
	if((code & HEAD_CODE2) == HEAD_CODE2){
		switch(code ^ HEAD_CODE2){
			case CODE_LEFT: drawLeft(x, y, HEAD_COL); break;
			case CODE_RIGHT: drawRight(x, y, HEAD_COL); break;
			case CODE_UP: drawUp(x, y, HEAD_COL); break;
			case CODE_DOWN: drawDown(x, y, HEAD_COL); break;
			default: drawFullBlock(x, y, ERROR_COL);
		}
		return ;
	}
	if((code & PLAYER1_CODE) != 0){
		switch(code ^ PLAYER1_CODE){
			case HORIZONTAL_CODE: drawHorizontal(x, y, SNAKE1_COL); break;
			case VERTICAL_CODE: drawVertical(x, y, SNAKE1_COL); break;
			case BOTTOM_RIGHT_CODE: drawBottomRight(x, y, SNAKE1_COL); break;
			case BOTTOM_LEFT_CODE: drawBottomLeft(x, y, SNAKE1_COL); break;
			case TOP_RIGHT_CODE: drawTopRight(x, y, SNAKE1_COL); break;
			case TOP_LEFT_CODE: drawTopLeft(x, y, SNAKE1_COL); break;
			default: drawFullBlock(x, y, ERROR_COL);
		}
		return ;
	}
	if((code & PLAYER2_CODE) != 0){
		switch(code ^ PLAYER2_CODE){
			case HORIZONTAL_CODE: drawHorizontal(x, y, SNAKE2_COL); break;
			case VERTICAL_CODE: drawVertical(x, y, SNAKE2_COL); break;
			case BOTTOM_RIGHT_CODE: drawBottomRight(x, y, SNAKE2_COL); break;
			case BOTTOM_LEFT_CODE: drawBottomLeft(x, y, SNAKE2_COL); break;
			case TOP_RIGHT_CODE: drawTopRight(x, y, SNAKE2_COL); break;
			case TOP_LEFT_CODE: drawTopLeft(x, y, SNAKE2_COL); break;
			default: drawFullBlock(x, y, ERROR_COL);
		}
		return ;
	}
	drawFullBlock(x, y, ERROR_COL);
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
void drawStartUpAnimation(){
	int signX=2;
	int signY=11;
	drawLetterS(signX,signY); 
	drawLetterN(signX+5,signY);
	drawLetterA(signX+10,signY); 
	drawLetterK(signX+15,signY); 
	drawLetterE(signX+20,signY); 
	drawLetterExclamation(signX+25,signY);
	drawSnakeAnimation(1,24);
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
	if(c == 0){
		return '\0';
	}
	return IGNORE_CODE;
}

void keyPressed(int moves[2]){
	char c;
	int move;
	while(1 == 1){
		c = getChar();
		move = charToCode(c);
		if(move == 0){
			return ;
		}
		if(move == IGNORE_CODE){
			continue ;
		}
		if(move >= UP2){
			moves[1] = move;
		}
		else{
			moves[0] = move;
		}
	}
}

void analizeKeyPressed(Player *player, int move){
	// head cant go backwards
	if(	(move == IGNORE_CODE) || 
		((player->dir + move) == (UP1 + DOWN1)) || 
		((player->dir + move) == (LEFT1 + RIGHT1)) ||
		((player->dir + move) == (UP2 + DOWN2)) ||
		((player->dir + move) == (LEFT2 + RIGHT2))){
		return;
	}
	player->dir = move;
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

int analizeColitions(BodyPart board[CANT_BLOCKS][CANT_BLOCKS], Player *player, int xHead, int yHead){
	int ret = 0;
	int newXHead = xHead + dirToX(player->dir);
	int newYHead = yHead + dirToY(player->dir);
	ret += FOOD_COLLITION_CODE * analizeFoodCollition(board, newXHead, newYHead);
	ret += WALL_COLLITION_CODE * analizeWallCollition(newXHead, newYHead);
	ret += SNAKE_COLLITION_CODE * analizeBodyCollition(board, newXHead, newYHead);
	return ret;
}

void moveHead(int i, int j, BodyPart board[CANT_BLOCKS][CANT_BLOCKS], Player *player, int headCode){
	int newXHead = i + dirToX(player->dir);
	int newYHead = j + dirToY(player->dir);
	switch(2 * dirToX(player->dir) + dirToY(player->dir)){
		case -1: 
			board[newXHead][newYHead].code = headCode | CODE_DOWN; 
		break;
		case 1: 
			board[newXHead][newYHead].code = headCode | CODE_UP; 
		break;
		case -2:
			board[newXHead][newYHead].code = headCode | CODE_RIGHT; 
		break;
		case 2:
			board[newXHead][newYHead].code = headCode | CODE_LEFT; 
		break;
	}
	board[newXHead][newYHead].lifeTime = player->size;
}

// kind of complicated way to put the right code in each bodypart so that
// the snake is coherent and it doesnt just occupy the whole block
// it can be updated but whatever, making a mouse is more fun
void setCode(int i, int j, BodyPart board[CANT_BLOCKS][CANT_BLOCKS], Player *player, int playerCode){
	if(player->previousDir == player->dir){
		if(dirToX(player->dir) != 0){
			board[i][j].code = HORIZONTAL_CODE;
		}
		else{
			board[i][j].code = VERTICAL_CODE;
		}
	}
	else{
		switch(2 * dirToX(player->previousDir) + dirToY(player->previousDir)){
			case -1: 
				if(dirToX(player->dir) == 1){
					board[i][j].code = TOP_LEFT_CODE;
				}
				else{
					board[i][j].code = TOP_RIGHT_CODE;
				}
			break;
			case 1: 
				if(dirToX(player->dir) == 1){
					board[i][j].code = BOTTOM_LEFT_CODE;
				}
				else{
					board[i][j].code = BOTTOM_RIGHT_CODE;
				}
			break;
			case -2:
				if(dirToY(player->dir) == 1){
					board[i][j].code = TOP_LEFT_CODE;
				}
				else{
					board[i][j].code = BOTTOM_LEFT_CODE;
				}
			break;
			case 2:
				if(dirToY(player->dir) == 1){
					board[i][j].code = TOP_RIGHT_CODE;
				}
				else{
					board[i][j].code = BOTTOM_RIGHT_CODE;
				}
			break;
		}
	}
	board[i][j].code |= playerCode;
}

// The idea is that if a snake's size is, say 5, each time the head moves to a block
// that block belongs to the snake for a total of 5 frames
// or, in other words, it's starting lifetime is 5
// and when it reaches 0, it returns to being part of the board
void changeLifetimes(BodyPart board[CANT_BLOCKS][CANT_BLOCKS], int ignore){
	for(int i = 0; i < CANT_BLOCKS; i++){
		for(int j = 0; j < CANT_BLOCKS; j++){
			if((board[i][j].code == NOTHING_CODE) || (board[i][j].code == FOOD_CODE)){
				continue;
			}

			if(((board[i][j].code & PLAYER1_CODE) != 0) && ((ignore & 1) != 0)){
				continue;
			}

			if(((board[i][j].code & PLAYER2_CODE) != 0) && ((ignore & 2) != 0)){
				continue;
			}

			board[i][j].lifeTime--;
			
			if(board[i][j].lifeTime == 0){
				board[i][j].code = NOTHING_CODE;
			}
		}
	}
}


//TODO
int processHead(BodyPart board[CANT_BLOCKS][CANT_BLOCKS], Player *player, int headCode, int playerCode){
	int ret;
	int cont = 1;
	// searches for the snake's head
	for(int i = 0; (i < CANT_BLOCKS) && (cont == 1); i++){
		for(int j = 0; (j < CANT_BLOCKS) && (cont == 1); j++){
			if((board[i][j].code & headCode) == headCode){
				// checks for any type of collition
				ret = analizeColitions(board, player, i, j);
				
				moveHead(i, j, board, player, headCode);
							
				// sets the code for the previous head's position
				// for drawing purposes
				setCode(i, j, board, player, playerCode);
							
				cont = 0;
			}
		}
	}
	return ret;
}

int update(BodyPart board[CANT_BLOCKS][CANT_BLOCKS], Player *player, int moves[CANT_PLAYERS], int headCode, int playerCode, int playerIndex){
	pseudoRandom *= 3;
	keyPressed(moves);
	analizeKeyPressed(player, moves[playerIndex]);
	int ret;

	ret = processHead(board, player, headCode, playerCode);

	if((ret & FOOD_COLLITION_CODE) != 0){
		player->size++;
	}
	
	player->previousDir = player->dir;

	return ret;		
}

void waitForStart(int moves[CANT_PLAYERS]){
	char c;
	int ret = IGNORE_CODE;
	while((moves[0] == 0) || (moves[1] == 0)){
		c = getChar();
		ret = charToCode(c);
		if(ret == IGNORE_CODE){
			continue;
		}
		if(ret >= UP2){
			moves[1] = ret;
			continue;
		}
		moves[0] = ret;
	}
}

void iniBoard(BodyPart board[CANT_BLOCKS][CANT_BLOCKS]){
	for(int i = 0; i < CANT_BLOCKS; i++){
		for(int j = 0; j < CANT_BLOCKS; j++){
			board[i][j] = (BodyPart){0, NOTHING_CODE};
		}
	}
	
	// this lines can be changed at will. although beware of not colliding the snakes
	// nor the food with any of the snakes at this starting positions
	board[7][5] = (BodyPart){3, HEAD_CODE1|CODE_LEFT};
	board[6][5] = (BodyPart){2, HORIZONTAL_CODE|PLAYER1_CODE};
	board[5][5] = (BodyPart){1, HORIZONTAL_CODE|PLAYER1_CODE};
	board[7][9] = (BodyPart){3, HEAD_CODE2|CODE_LEFT};
	board[6][9] = (BodyPart){2, HORIZONTAL_CODE|PLAYER2_CODE};
	board[5][9] = (BodyPart){1, HORIZONTAL_CODE|PLAYER2_CODE};
	board[12][7] = (BodyPart){0, FOOD_CODE};
}

int isGameEnded(int ret1, int ret2){
	if(((ret1 & SNAKE_COLLITION_CODE) != 0) || ((ret1 & WALL_COLLITION_CODE) != 0)){
		if(((ret2 & SNAKE_COLLITION_CODE) != 0) || ((ret2 & WALL_COLLITION_CODE) != 0)){
			return BOTH_LOSE_CODE;
		}
		return PLAYER2_WINS_CODE;
	}
	if(((ret2 & SNAKE_COLLITION_CODE) != 0) || ((ret2 & WALL_COLLITION_CODE) != 0)){
		return PLAYER1_WINS_CODE;
	}
	return 0;
}

void snake(){
	int counter = 0;
	static BodyPart board[CANT_BLOCKS][CANT_BLOCKS];
	static Player player1 = {4, 0};
	static Player player2 = {4, 0};
	static int moves[CANT_PLAYERS] = {0, 0};
	moves[0] = 0; // it bugs after reloading snake if these 2 lines aren't here
	moves[1] = 0;
	
	iniBoard(board);	
	
	timerWait(18);
	draw(board);
	timerWait(18);
	drawStartUpAnimation();
	timerWait(18);
	
	timerWait(18);
	draw(board);
	waitForStart(moves);
	player1.dir = moves[0];
	player2.dir = moves[1];
	player1.previousDir = player1.dir;
	player2.previousDir = player2.dir;

	int ret1 = 0;
	int ret2 = 0;
	int cont = 1;
	int gameEnd;

	while(cont == 1){
		ret1 = update(board, &player1, moves, HEAD_CODE1, PLAYER1_CODE, 0);
		ret2 = update(board, &player2, moves, HEAD_CODE2, PLAYER2_CODE, 1);
		int code1 = 0;
		int code2 = 0;
		if(ret1 == FOOD_COLLITION_CODE){
			code1 = 1;
		}
		if(ret2 == FOOD_COLLITION_CODE){
			code2 = 1;
		}
		changeLifetimes(board, code1+code2*2);
		draw(board);
		counter++;
		gameEnd = isGameEnded(ret1, ret2);
		if(gameEnd != 0){
			cont = 0;
		}
	}
	clear();
	char * msg;
	if(gameEnd == PLAYER1_WINS_CODE){
		msg = "Player 1 wins\n\0";
	}
	if(gameEnd == PLAYER2_WINS_CODE){
		msg = "Player 2 wins\n\0";
	}
	if(gameEnd == BOTH_LOSE_CODE){
		msg = "Come on, you cant be BOTH bad\n\0";
	}
	printc(msg, 0x00FF0000);
}

void drawApple(x, y){
	drawFullBlock(x, y, FOOD_COL);
	return ;
	int trueX = x * BLOCK_SIZE;
	int trueY = y * BLOCK_SIZE;
}

// bit tables for drawing

void drawVertical(int x, int y, int col){
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
				putPixel(col, i, j);
			}
		}
	}
}


void drawUp(int x, int y, int col){
	int matrix[20][20] = 
		{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
				putPixel(col, i, j);
			}
			if(matrix[i-trueX][j-trueY] == 2){
				putPixel(EYE_COL, i, j);
			}
		}
	}
}

void drawDown(int x, int y, int col){
	int matrix[20][20] = 
		{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1},
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
				putPixel(col, i, j);
			}
			if(matrix[i-trueX][j-trueY] == 2){
				putPixel(EYE_COL, i, j);
			}
		}
	}
}

void drawLeft(int x, int y, int col){
	int matrix[20][20] = {
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0},
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
				putPixel(col, i, j);
			}
			if(matrix[i-trueX][j-trueY] == 2){
				putPixel(EYE_COL, i, j);
			}
		}
	}
}

void drawHorizontal(int x, int y, int col){
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
				putPixel(col, i, j);
			}
		}
	}
}

void drawRight(int x, int y, int col){
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
		{0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0},
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
				putPixel(col, i, j);
			}
			if(matrix[i-trueX][j-trueY] == 2){
				putPixel(EYE_COL, i, j);
			}
		}
	}
}

void drawBottomLeft(int x, int y, int col){
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
				putPixel(col, i, j);
			}
		}
	}
}

void drawTopLeft(int x, int y, int col){
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
				putPixel(col, i, j);
			}
		}
	}
}

void drawTopRight(int x, int y, int col){
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
				putPixel(col, i, j);
			}
		}
	}
}

void drawBottomRight(int x, int y, int col){
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
				putPixel(col, i, j);
			}
		}
	}
}

void drawLetterS(int x, int y){
		int matrix[7][4] = {
			{0,1,1,0},
			{1,0,0,1},
			{1,0,0,0},
			{0,1,1,0},
			{0,0,0,1},
			{1,0,0,1},
			{0,1,1,0}};
		for(int i=0; i<7; i++){
			for(int j=0; j<4;j++){
				if(matrix[i][j]==1){
					drawLetter(x+j,y+i);
				}
			}
		}
	}
void drawLetterN(int x, int y){
	int matrix[7][4] = {
		{1,0,0,1},
		{1,1,0,1},
		{1,1,0,1},
		{1,0,1,1},
		{1,0,1,1},
		{1,0,0,1},
		{1,0,0,1}};
	for(int i=0; i<7;i++){
		for(int j=0; j<4; j++){
			if(matrix[i][j]==1){
				drawLetter(x+j,y+i);
			}
		}
	}
}
void drawLetterA(int x, int y){
	int matrix[7][4] = {
		{0,1,1,0},
		{1,0,0,1},
		{1,0,0,1},
		{1,1,1,1},
		{1,0,0,1},
		{1,0,0,1},
		{1,0,0,1}};
	for(int i=0; i<7;i++){
		for(int j=0; j<4;j++){
			if(matrix[i][j]==1){
				drawLetter(x+j,y+i);
			}
		}
	}
}
void drawLetterK(int x, int y){
	int matrix[7][4] = {
		{1,0,0,1},
		{1,0,0,1},
		{1,0,1,0},
		{1,1,0,0},
		{1,0,1,0},
		{1,0,1,1},
		{1,0,0,1}};
	for(int i=0; i<7; i++){
		for(int j=0; j<4; j++){
			if(matrix[i][j]==1){
				drawLetter(x+j,y+i);
			}
		}
	}
}
void drawLetterE(int x, int y){
	int matrix[7][4] = {
		{1,1,1,1},
		{1,0,0,0},
		{1,0,0,0},
		{1,1,1,0},
		{1,0,0,0},
		{1,0,0,0},
		{1,1,1,1}};
	for(int i=0; i<7;i++){
		for(int j=0; j<4;j++){
			if(matrix[i][j]==1){
				drawLetter(x+j,y+i);
			}
		}
	}
}
void drawLetterExclamation(int x, int y){
	int matrix[6][1] = {
		{1},
		{1},
		{1},
		{1},
		{1},
		{0}};
	for(int i=0; i<7; i++){
		for(int j=0; j<1;j++){
			if(matrix[i][j]==1){
				drawLetter(x+j,y+i);
			}
		}
	}
	drawApple(x,y+6);
}
void drawSnakeAnimation(int x, int y){
	int timeBetweenFrames=2;
	for(int i=0; i<26;i++){
		if(i==0){
			drawLeft(x,y,HEAD_COL);
			timerWait(timeBetweenFrames);
		}
		if(i==1){
			drawLeft(x,y,HEAD_COL);
			drawHorizontal(x-1,y,SNAKE1_COL);
			timerWait(timeBetweenFrames);
		}
		if(i>1){

			drawLeft(x,y,HEAD_COL);
			drawHorizontal(x-1,y,SNAKE1_COL);
			drawHorizontal(x-2,y,SNAKE1_COL);
			drawEmpty(x-3,y);
			timerWait(timeBetweenFrames);
		}
		x++;
	}
	for(int i=0; i<7; i++){
		y--;
		if(i==0){
			drawDown(x,y,HEAD_COL);
			drawBottomRight(x,y+1,SNAKE1_COL);
			drawHorizontal(x-1,y+1,SNAKE1_COL);
			drawEmpty(x-2,y+1);
			drawEmpty(x-3,y+1);
			timerWait(timeBetweenFrames);
		}
		if(i==1){
			drawDown(x,y,HEAD_COL);
			drawVertical(x,y+1,SNAKE1_COL);
			drawVertical(x,y+2,SNAKE1_COL);
			drawEmpty(x-1,y+2);
			timerWait(timeBetweenFrames);
		}
		if(i>1 && i<6){
			drawDown(x,y,HEAD_COL);
			drawVertical(x,y+1,SNAKE1_COL);
			drawVertical(x,y+2,SNAKE1_COL);
			drawEmpty(x,y+3);
			timerWait(timeBetweenFrames);
		}
		if(i==6){
			drawDown(x,y,HEAD_COL);
			drawVertical(x,y+1,SNAKE1_COL);
			drawVertical(x,y+2,SNAKE1_COL);
			drawVertical(x,y+3,SNAKE1_COL);
			timerWait(timeBetweenFrames);
		}
	}
	x++;
	drawLeft(x,y,HEAD_COL);
	drawTopLeft(x-1,y,SNAKE1_COL);
	drawVertical(x-1,y+1,SNAKE1_COL);
	drawVertical(x-1,y+2,SNAKE1_COL);
	drawEmpty(x-1,y+3);
	timerWait(timeBetweenFrames);
	x++;
	drawLeft(x,y,HEAD_COL);
	drawHorizontal(x-1,y,SNAKE1_COL);
	drawTopLeft(x-2,y,SNAKE1_COL);
	drawVertical(x-2,y+1,SNAKE1_COL);
	drawEmpty(x-2,y+2);
	timerWait(timeBetweenFrames);
	x++;
	drawHorizontal(x-1,y,SNAKE1_COL);
	drawHorizontal(x-2,y,SNAKE1_COL);
	drawHorizontal(x-3,y,SNAKE1_COL);
	drawEmpty(x-3,y+1);
	timerWait(timeBetweenFrames);
	x++;
	drawHorizontal(x-2,y,SNAKE1_COL);
	drawHorizontal(x-3,y,SNAKE1_COL);
	drawEmpty(x-4,y);
	timerWait(timeBetweenFrames);
	x++;
	drawHorizontal(x-3,y,SNAKE1_COL);
	drawEmpty(x-4,y);
	timerWait(timeBetweenFrames);
	x++;
	drawEmpty(x-4,y);
	timerWait(timeBetweenFrames);
}