#include "myFunctions.h"

int main(void)
{
	int pi;

	if ((pi = pigpio_start(NULL, NULL)) < 0)
	{
		fprintf(stderr, "%s\n", pigpio_error(pi));
		exit(-1);
	}

    set_ServoMotor(pi);

	WHEEL_STOP(pi);
	
	printf("END\n");
	return 0;
}
