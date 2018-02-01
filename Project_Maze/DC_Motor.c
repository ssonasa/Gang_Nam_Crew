#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

#define INPUT1 17
#define INPUT2 22
#define INPUT3 24
#define INPUT4 23

#define DUTYCYCLE(x, range) x/(float)range*100

int main(void)
{
	int i;
	int pi;

	int default_range = 100;
	int range;

	if ((pi = pigpio_start(NULL, NULL)) < 0) {
		fprintf(stderr, "%s\n", pigpio_error(pi));
		exit(-1);
	}

	set_mode(pi, INPUT1, PI_OUTPUT);
	set_mode(pi, INPUT2, PI_OUTPUT);
	set_mode(pi, INPUT3, PI_OUTPUT);
	set_mode(pi, INPUT4, PI_OUTPUT);

	set_PWM_range(pi, INPUT1, default_range);
	set_PWM_range(pi, INPUT2, default_range);
	set_PWM_range(pi, INPUT3, default_range);
	set_PWM_range(pi, INPUT4, default_range);
	range = get_PWM_range(pi, INPUT1);

	for (int k = 0; k < 5; k++) {
        /*gpio_write(pi, INPUT1, PI_LOW);
	    gpio_write(pi, INPUT2, PI_LOW);
	    gpio_write(pi, INPUT3, PI_LOW);
	    gpio_write(pi, INPUT4, PI_LOW);
        time_sleep(2);*/
		//Forward
		for (i = 50; i < range; i += 10) {
			set_PWM_dutycycle(pi, INPUT2, i);
			set_PWM_dutycycle(pi, INPUT4, i);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT3, PI_LOW);
            printf("Forward %d\n", i);
			time_sleep(1);
		}
		//Slow_stop
/*		for (i = range; i > 0; i -= 10) {
			set_PWM_dutycycle(pi, INPUT2, i);
			set_PWM_dutycycle(pi, INPUT4, i);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT3, PI_LOW);
			printf("Slow_stop %d\n", i);
			time_sleep(1);
		}
		//Reverse        
		for (i = 50; i < range; i += 10) {
			set_PWM_dutycycle(pi, INPUT1, i);
			set_PWM_dutycycle(pi, INPUT3, i);
			gpio_write(pi, INPUT2, PI_LOW);
			gpio_write(pi, INPUT4, PI_LOW);
			printf("Reverse %d\n", i);
			time_sleep(1);
		}
		//Turn_left
		for (i = 50; i < range; i += 10) {
			set_PWM_dutycycle(pi, INPUT2, i);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT3, PI_LOW);
			gpio_write(pi, INPUT4, PI_LOW);
            printf("Turn_left %d\n", i);
			time_sleep(1);
		}
		//Turn_right
		for (i = 50; i < range; i += 10) {
			set_PWM_dutycycle(pi, INPUT4, i);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT2, PI_LOW);
			gpio_write(pi, INPUT3, PI_LOW);
            printf("Turn_right %d\n", i);
			time_sleep(1);
		}
		//Left_turn
		for (i = 50; i < range; i += 10) {
			set_PWM_dutycycle(pi, INPUT2, i + 20);
			set_PWM_dutycycle(pi, INPUT4, i);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT3, PI_LOW);
            printf("Left_turn %d\n", i);
			time_sleep(1);
		}
		//Right_turn
		for (i = 50; i < range; i += 10) {
			set_PWM_dutycycle(pi, INPUT4, i + 20);
			set_PWM_dutycycle(pi, INPUT2, i);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT3, PI_LOW);
            printf("Right_turn %d\n", i);
			time_sleep(1);
		}
        gpio_write(pi, INPUT1, PI_LOW);
	    gpio_write(pi, INPUT2, PI_LOW);
	    gpio_write(pi, INPUT3, PI_LOW);
	    gpio_write(pi, INPUT4, PI_LOW);
        time_sleep(1);*/
	}
        gpio_write(pi, INPUT1, PI_LOW);
	    gpio_write(pi, INPUT2, PI_LOW);
	    gpio_write(pi, INPUT3, PI_LOW);
	    gpio_write(pi, INPUT4, PI_LOW);


	return 0;
}



