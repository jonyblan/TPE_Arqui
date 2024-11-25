#include "utillib.h";
#include "arquilib.h";
#include <stdint.h>

static const char * messages[] = {
	"Outlook Good\0",
	"Signs Point to Yes\0",
	"Reply Hazy, Try Again\0",
	"Better Times Ahead\0",
	"You May Rely on It\0",
	"As I See It, Yes\0",
	"Most Likely\0",
	"It is Certain\0",
	"Don’t Count on It\0",
	"Cannot Predict Now\0",
	"Concentrate and Ask Again\0",
	"Ask Again Later\0",
	"Better Not Tell You Now\0",
	"Cannot Foretell Now\0",
	"My Reply is No\0",
	"Outlook Not So Good\0",
	"Very Doubtful\0",
	"No\0"
	};

uint32_t * colors = {GREEN, GREEN, YELLOW, GREEN, GREEN, GREEN, GREEN, GREEN, RED, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, RED, RED, RED, RED};



void magicBall(char *argv){
	static uint8_t pseudoRandom = 73;
	pseudoRandom *= 3;
	pseudoRandom *= 3;
	pseudoRandom *= 3;
	printc(messages[pseudoRandom%18], colors[pseudoRandom%18]);
}