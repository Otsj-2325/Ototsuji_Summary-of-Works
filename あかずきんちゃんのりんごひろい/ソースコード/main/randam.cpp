#include "randam.h"

void initRand()
{
	srand(time(NULL));
}

int getRand()
{
	return rand();
}

int getLimitRand(int limit)
{
	return rand() % limit;
}
