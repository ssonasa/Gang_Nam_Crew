#include <stdio.h>
#include <pigpiod_if2.h>
#include "74hc595_functions.h" 

int pi;
int init(void)
{
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        return 0;
    }
    
    set_mode(pi, SDATA, PI_OUTPUT);
    set_mode(pi, STR_CLK, PI_OUTPUT);
    set_mode(pi, SHR_CLK, PI_OUTPUT);
    set_mode(pi, SHR_CLEAR, PI_OUTPUT);
    gpio_write(pi, SHR_CLEAR, 1);
    return 1;
}

void release(void)
{
    allclear();
    pigpio_stop(pi);
}

void allclear(void)
{
    gpio_write(pi, SHR_CLEAR, 0);   gpio_write(pi, SHR_CLEAR, 1); // clear
    gpio_write(pi, STR_CLK, 0);     gpio_write(pi, STR_CLK, 1);   // latch
}

void set(int index)
{
    gpio_write(pi, SDATA, 1);
    gpio_write(pi, SHR_CLK, 0);
    gpio_write(pi, SHR_CLK, 1);
    gpio_write(pi, SDATA, 0);

    for(int i = 0 ; i < index ; i++){
        gpio_write(pi, SHR_CLK, 0);
        gpio_write(pi, SHR_CLK, 1);   // shift
    }
    gpio_write(pi, STR_CLK, 0);
    gpio_write(pi, STR_CLK, 1);   // latch
}

// unsinged 8bit int
void set8(uint8_t value)
{
	for(int i = 0 ; i < 8 ; i++){
		int mask = 0b1 << i;
		if((value & mask) == 0)
			gpio_write(pi, SDATA, 0);
		else
			gpio_write(pi, SDATA, 1);
		gpio_write(pi, SHR_CLK, 0); 
		gpio_write(pi, SHR_CLK, 1); 
	}
	// letch
	gpio_write(pi, STR_CLK, 0); 
	gpio_write(pi, STR_CLK, 1);
}

void set16(uint16_t value)
{
    for(int i = 0 ; i < 16 ; i++){
        int mask = 0b1 << i;
        if((value & mask) == 0)
            gpio_write(pi, SDATA, 0);
        else
            gpio_write(pi, SDATA, 1);
        gpio_write(pi, SHR_CLK, 0);
        gpio_write(pi, SHR_CLK, 1);
    }
    // letch
    gpio_write(pi, STR_CLK, 0);
    gpio_write(pi, STR_CLK, 1);
}


