#include "utillib.h"
#include "arquilib.h"
#include <stdint.h>

#define MESSAGE_COUNT 18
#define MAX_CHARS 128

static const char messages[MESSAGE_COUNT][MAX_CHARS] = {
	"Outlook Good\n",
	"Signs Point to Yes\n",
	"Reply Hazy, Try Again\n",
	"Better Times Ahead\n",
	"You May Rely on It\n",
	"As I See It, Yes\n",
	"Most Likely\n",
	"It is Certain\n",
	"Don’t Count on It\n",
	"Cannot Predict Now\n",
	"Concentrate and Ask Again\n",
	"Ask Again Later\n",
	"Better Not Tell You Now\n",
	"Cannot Foretell Now\n",
	"My Reply is No\n",
	"Outlook Not So Good\n",
	"Very Doubtful\n",
	"No\n"
	};

uint32_t colors[MESSAGE_COUNT] = {GREEN, GREEN, YELLOW, GREEN, GREEN, GREEN, GREEN, GREEN, RED, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, RED, RED, RED, RED};



void magicBall(char *argv){
	static uint8_t pseudoRandom = 73;
	pseudoRandom *= 3;
	pseudoRandom *= 3;
	pseudoRandom *= 3;
	printc(messages[pseudoRandom%18], colors[pseudoRandom%18]);
}