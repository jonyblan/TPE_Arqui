/* sampleCodeModule.c */

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {
	//All the following code may be removed 
	*v = 'h';
	//*(v+1) = 0x74;
	*(v+1)='e';


	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xA11BC0DE;

	return 0xDEADBEEF;
}