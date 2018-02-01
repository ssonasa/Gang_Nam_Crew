#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

#define INPUT1 17
#define INPUT2 27
#define INPUT3 24
#define INPUT4 23

#define FRONT_TRIG_PINNO 16
#define FRONT_ECHO_PINNO 13
#define RIGHT_TRIG_PINNO 19
#define RIGHT_ECHO_PINNO 26
#define LEFT_TRIG_PINNO 21
#define LEFT_ECHO_PINNO 20

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

    float Fdistance, Rdistance, Ldistance;
    
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

    set_mode(pi, FRONT_TRIG_PINNO, PI_OUTPUT);
    set_mode(pi, FRONT_ECHO_PINNO, PI_INPUT);
    callback(pi, FRONT_ECHO_PINNO, EITHER_EDGE, cb_func_echo);
    gpio_write(pi, FRONT_TRIG_PINNO, PI_OFF);

    set_mode(pi, RIGHT_TRIG_PINNO, PI_OUTPUT);
    set_mode(pi, RIGHT_ECHO_PINNO, PI_INPUT);
    callback(pi, RIGHT_ECHO_PINNO, EITHER_EDGE, cb_func_echo);
    gpio_write(pi, RIGHT_TRIG_PINNO, PI_OFF);

    set_mode(pi, LEFT_TRIG_PINNO, PI_OUTPUT);
    set_mode(pi, LEFT_ECHO_PINNO, PI_INPUT);
    callback(pi, LEFT_ECHO_PINNO, EITHER_EDGE, cb_func_echo);
    gpio_write(pi, LEFT_TRIG_PINNO, PI_OFF);
    time_sleep(1);     // delay 1 second

    while(1){
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, FRONT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.05);
        Fdistance = dist_tick_ / 1000000. * 340 / 2 * 100;

        if(Fdistance > 2 && Fdistance < 200){
            if(dist_tick_ && start_tick_){
                if(Fdistance < 2 || Fdistance > 400)
                    printf("FRONT : %6dus, Distance : %6.1f cm\n", dist_tick_, Fdistance);
                else{ 
                    printf("FRONT : %6dus, Distance : %6.1f cm\n", dist_tick_, Fdistance);
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
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, RIGHT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.05);
        Rdistance = dist_tick_ / 1000000. * 340 / 2 * 100;

        if(Rdistance > 2 && Rdistance < 200){
            if(dist_tick_ && start_tick_){
                if(Rdistance < 2 || Rdistance > 400)
                    printf("RIGHT : %6dus, Distance : %6.1f cm\n", dist_tick_, Rdistance);
                else{ 
                    printf("RIGHT : %6dus, Distance : %6.1f cm\n", dist_tick_, Rdistance);
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
                start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, LEFT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.05);
        Ldistance = dist_tick_ / 1000000. * 340 / 2 * 100;

        if(Ldistance > 2 && Ldistance < 200){
            if(dist_tick_ && start_tick_){
                if(Ldistance < 2 || Ldistance > 400)
                    printf("LEFT : %6dus, Distance : %6.1f cm\n\n", dist_tick_, Ldistance);
                else{ 
                    printf("LEFT : %6dus, Distance : %6.1f cm\n\n", dist_tick_, Ldistance);
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
        //Forward
        if((Ldistance <= Rdistance+2 && Ldistance >= Rdistance-2) && (Rdistance < Fdistance)){
            /*
            set_PWM_dutycycle(pi, INPUT1, 0);
            set_PWM_dutycycle(pi, INPUT2, Fdistance*20);
            set_PWM_dutycycle(pi, INPUT3, 0);
            set_PWM_dutycycle(pi, INPUT4, Fdistance*20);
            */
            printf("FORWARD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(1);
        }
        else if((Rdistance <= Fdistance+2 && Rdistance >= Fdistance-2) && (Fdistance < Ldistance)){
            /*
            set_PWM_dutycycle(pi, INPUT1, Ldistance*20);
            set_PWM_dutycycle(pi, INPUT2, 0);
            set_PWM_dutycycle(pi, INPUT3, 0);
            set_PWM_dutycycle(pi, INPUT4, Ldistance*20);     
            */
            printf("LEFT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(1);       
        }
        else if((Fdistance <= Ldistance+2 && Fdistance >= Ldistance-2) && (Ldistance < Rdistance)){
            /*
            set_PWM_dutycycle(pi, INPUT1, Rdistance*20);
            set_PWM_dutycycle(pi, INPUT2, 0);
            set_PWM_dutycycle(pi, INPUT3, Rdistance*20);
            set_PWM_dutycycle(pi, INPUT4, 0);
            */
            printf("RIGHT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(1);
        }
        else{
            printf("sense error\n");
            set_PWM_dutycycle(pi, INPUT1, 0);
            set_PWM_dutycycle(pi, INPUT2, 0);
            set_PWM_dutycycle(pi, INPUT3, 0);
            set_PWM_dutycycle(pi, INPUT4, 0);
            time_sleep(5);
        }
        time_sleep(1);
    }
    pigpio_stop(pi);



    return 0;
}

void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick)
{
    if(level == PI_HIGH)
        start_tick_ = tick;
    else if(level == PI_LOW)
        dist_tick_ = tick - start_tick_;
}
