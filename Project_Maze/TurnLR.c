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
        gpio_write(pi, INPUT1, PI_LOW);
	    gpio_write(pi, INPUT2, PI_LOW);
	    gpio_write(pi, INPUT3, PI_LOW);
	    gpio_write(pi, INPUT4, PI_LOW);
        time_sleep(2);
		
		//Turn_left
		for (i = 0; i <= 30; i++) {
			set_PWM_dutycycle(pi, INPUT3, 60);
			set_PWM_dutycycle(pi, INPUT2, 55);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT4, PI_LOW);
           printf("Left_turn %d\n", i);
			time_sleep(0.01);
	}
		/*
		//Turn_right
		for (i = 30; i <= range; i += 10) {
			set_PWM_dutycycle(pi, INPUT1, i + 20);
			set_PWM_dutycycle(pi, INPUT3, i);
			gpio_write(pi, INPUT2, PI_LOW);
			gpio_write(pi, INPUT4, PI_LOW);
            printf("Right_turn %d\n", i);
			time_sleep(0.5);
		}*/
		
	}
        gpio_write(pi, INPUT1, PI_LOW);
	    gpio_write(pi, INPUT2, PI_LOW);
	    gpio_write(pi, INPUT3, PI_LOW);
	    gpio_write(pi, INPUT4, PI_LOW);


	return 0;
}



