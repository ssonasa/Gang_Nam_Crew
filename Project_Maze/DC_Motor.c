#include "Functions.h"

int main(void)
{
	int pi;

	if ((pi = pigpio_start(NULL, NULL)) < 0)
	{
		fprintf(stderr, "%s\n", pigpio_error(pi));
		exit(-1);
	}

	//gpio핀 output설정
	set_ServoMotor(pi);

	//FORWARD
	WHEEL_FORWARD(pi);
	//RIGHT_FORWARD
	WHEEL_RIGHT_FORWARD(pi);
	//LEFT_FORWARD
	WHEEL_LEFT_FORWARD(pi);
	//REVERSE
	WHEEL_REVERSE(pi);
	//RIGHT_TRIPLE_ACCEL
	WHEEL_RIGHT_TRIPLE_ACCEL(pi);
	//LEFT_TRIPLE_ACCEL
	WHEEL_LEFT_TRIPLE_ACCEL(pi);
	//RIGHT_TURN
	WHEEL_RIGHT_TURN(pi);
	//Left_TURN
	WHEEL_LEFT_TURN(pi);
	

	printf("END\n");
	return 0;
}
