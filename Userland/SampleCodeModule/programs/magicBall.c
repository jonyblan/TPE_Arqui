#include "utillib.h"
#include "arquilib.h"
#include <stdint.h>

static const char * messages[] = {
	"Outlook Good",
	"Signs Point to Yes",
	"Reply Hazy, Try Again",
	"Better Times Ahead",
	"You May Rely on It",
	"As I See It, Yes",
	"Most Likely",
	"It is Certain",
	"Don’t Count on It",
	"Cannot Predict Now",
	"Concentrate and Ask Again",
	"Ask Again Later",
	"Better Not Tell You Now",
	"Cannot Foretell Now",
	"My Reply is No",
	"Outlook Not So Good",
	"Very Doubtful",
	"No"
	};

uint32_t * colors = {GREEN, GREEN, YELLOW, GREEN, GREEN, GREEN, GREEN, GREEN, RED, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, RED, RED, RED, RED};



void magicBall(char *argv){
	static uint8_t pseudoRandom = 73;
	pseudoRandom *= 3;
	pseudoRandom *= 3;
	pseudoRandom *= 3;
	printc(messages[pseudoRandom%18], colors[pseudoRandom%18]);
}