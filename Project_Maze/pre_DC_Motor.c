#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

#define LF 23
#define LR 24
#define RF 22
#define RR 17

#define DUTYCYCLE(x, range) x / (float)range * 100

#define Front_MIN_LENGTH 8
#define Right_MIN_LENGTH 6
#define Left_MIN_LENGTH 6

#define WHEEL_SPEED_HIGH 60
#define WHEEL_SPEED_MIDDLE 50
#define WHEEL_SPEED_LOW 58

void WHEEL_STOP(int pi)
{
	set_PWM_dutycycle(pi, LF, PI_LOW);
	set_PWM_dutycycle(pi, RF, PI_LOW);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
}

int main(void)
{
	int i;
	int pi;

	int default_range = 100;
	int range;

	if ((pi = pigpio_start(NULL, NULL)) < 0)
	{
		fprintf(stderr, "%s\n", pigpio_error(pi));
		exit(-1);
	}

	set_mode(pi, LF, PI_OUTPUT);
	set_mode(pi, LR, PI_OUTPUT);
	set_mode(pi, RF, PI_OUTPUT);
	set_mode(pi, RR, PI_OUTPUT);

	set_PWM_range(pi, LF, default_range);
	set_PWM_range(pi, LR, default_range);
	set_PWM_range(pi, RF, default_range);
	set_PWM_range(pi, RR, default_range);
	range = get_PWM_range(pi, LF);

	//FORWARD
	set_PWM_dutycycle(pi, LF, WHEEL_SPEED_MIDDLE);
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_MIDDLE);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Forward\n");
	time_sleep(3);
	WHEEL_STOP(pi);
	time_sleep(2);

	//RIGHT_FORWARD
	set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_LOW);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Right_Forward\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);

	//LEFT_FORWARD
	set_PWM_dutycycle(pi, LF, 50);
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Left_Forward\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);

	//REVERSE
	set_PWM_dutycycle(pi, LR, WHEEL_SPEED_MIDDLE);
	set_PWM_dutycycle(pi, RR, WHEEL_SPEED_MIDDLE);
	set_PWM_dutycycle(pi, LF, PI_LOW);
	set_PWM_dutycycle(pi, RF, PI_LOW);
	printf("Reverse\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);

	//RIGHT_REVERSE
	set_PWM_dutycycle(pi, LR, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, RR, WHEEL_SPEED_LOW);
	set_PWM_dutycycle(pi, LF, PI_LOW);
	set_PWM_dutycycle(pi, RF, PI_LOW);
	printf("Right_Reverse\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);

	//LEFT_REVERSE
	set_PWM_dutycycle(pi, LR, 54);
	set_PWM_dutycycle(pi, RR, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, LF, PI_LOW);
	set_PWM_dutycycle(pi, RF, PI_LOW);
	printf("Left_Reverse\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);

	//RIGHT_TRIPLE_ACCEL
	set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, RR, WHEEL_SPEED_LOW);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RF, PI_LOW);
	printf("Right_Triple_Accel\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);

	//LEFT_TRIPLE_ACCEL
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, LR, WHEEL_SPEED_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	set_PWM_dutycycle(pi, LF, PI_LOW);
	printf("Left_Triple_Accel\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);

	//RIGHT_TURN
	set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RF, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Right_Trun\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);

	//Left_TURN
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, LF, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Left_Trun\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);

	printf("END\n");
	return 0;
}
