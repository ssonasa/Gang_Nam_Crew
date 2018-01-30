#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

#define INPUT1 5
#define INPUT2 6
#define INPUT3 17
#define INPUT4 27

#define TRIG_PINNO 13
#define ECHO_PINNO 19

#define DUTYCYCLE(x, range) x/(float)range*100

void trigger(void);
void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);

uint32_t start_tick_, dist_tick_;
int main(void){
    int i;
    int pi;
    
    int default_range = 255;
    int range;
    int j;

    float distance;
    
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        exit(-1); //return 1;
    }

    set_mode(pi, INPUT1, PI_OUTPUT); set_mode(pi, INPUT2, PI_OUTPUT);
    set_mode(pi, INPUT3, PI_OUTPUT); set_mode(pi, INPUT4, PI_OUTPUT);
    
    set_PWM_range(pi, INPUT1, default_range);
    range = get_PWM_range(pi, INPUT1);
    set_PWM_range(pi, INPUT2, default_range);
    set_PWM_range(pi, INPUT3, default_range);
    set_PWM_range(pi, INPUT4, default_range);

    set_mode(pi, TRIG_PINNO, PI_OUTPUT);
    set_mode(pi, ECHO_PINNO, PI_INPUT);
    callback(pi, ECHO_PINNO, EITHER_EDGE, cb_func_echo);
    gpio_write(pi, TRIG_PINNO, PI_OFF);
    time_sleep(1);     // delay 1 second

    while(1){
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.05);
        distance = dist_tick_ / 1000000. * 340 / 2 * 100;

        if(distance > 2 && distance < 200){
            if(dist_tick_ && start_tick_){
                if(distance < 2 || distance > 400)
                    printf("range error\n");
                else{ 
                    printf("interval : %6dus, Distance : %6.1f cm\n", dist_tick_, distance);
                    /*
                    //forward or reverse
                    set_PWM_dutycycle(pi, INPUT1, distance*20);
                    set_PWM_dutycycle(pi, INPUT2, 0);
                    set_PWM_dutycycle(pi, INPUT3, 0);
                    set_PWM_dutycycle(pi, INPUT4, distance*20);
                    */
                    /*
                    //left turn  or right trun 
                    set_PWM_dutycycle(pi, INPUT1, distance*20);
                    set_PWM_dutycycle(pi, INPUT2, 0);
                    set_PWM_dutycycle(pi, INPUT3, distance*20);
                    set_PWM_dutycycle(pi, INPUT4, 0);
                    */
                    
                    //right trun or left turn
                    set_PWM_dutycycle(pi, INPUT1, 0);
                    set_PWM_dutycycle(pi, INPUT2, distance*20);
                    set_PWM_dutycycle(pi, INPUT3, 0);
                    set_PWM_dutycycle(pi, INPUT4, distance*20);
                    
                }
            }
        }
        else{
            printf("sense error\n");
            set_PWM_dutycycle(pi, INPUT1, 0);
            set_PWM_dutycycle(pi, INPUT2, 0);
            set_PWM_dutycycle(pi, INPUT3, 0);
            set_PWM_dutycycle(pi, INPUT4, 0);

        }
            
        

        time_sleep(0.1);
    }
    pigpio_stop(pi);


/*
    for(i = 0 ; i < 5 ; i++){
        for(j = 0; j < range; j+=50){
            set_PWM_dutycycle(pi, INPUT1, j);
            //gpio_write(pi, INPUT1, PI_HIGH);
            gpio_write(pi, INPUT2, PI_LOW);        
            set_PWM_dutycycle(pi, INPUT3, j);
            //gpio_write(pi, INPUT3, PI_HIGH); 
            gpio_write(pi, INPUT4, PI_LOW);
            //time_sleep(1);
        }
        for(j = 0; j < range; j+=50){
            gpio_write(pi, INPUT1, PI_LOW);
            set_PWM_dutycycle(pi, INPUT2, j);
            //gpio_write(pi, INPUT2, PI_HIGH);
            gpio_write(pi, INPUT3, PI_LOW);
            set_PWM_dutycycle(pi, INPUT4, j);
            //gpio_write(pi, INPUT4, PI_HIGH);
            // time_sleep(1);            
        }
    }
    
    gpio_write(pi, INPUT1, PI_LOW); gpio_write(pi, INPUT2, PI_LOW);
    gpio_write(pi, INPUT3, PI_LOW); gpio_write(pi, INPUT4, PI_LOW);
*/
    return 0;
}

void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick)
{
    if(level == PI_HIGH)
        start_tick_ = tick;
    else if(level == PI_LOW)
        dist_tick_ = tick - start_tick_;
}