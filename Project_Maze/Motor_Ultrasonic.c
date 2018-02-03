#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
//D
#define INPUT1 17
#define INPUT2 22
#define INPUT3 24
#define INPUT4 23
//U
#define FRONT_TRIG_PINNO 16
#define FRONT_ECHO_PINNO 13
#define RIGHT_TRIG_PINNO 19
#define RIGHT_ECHO_PINNO 6
#define LEFT_TRIG_PINNO 21
#define LEFT_ECHO_PINNO 20
//D
#define DUTYCYCLE(x, range) x/(float)range*100
//U
void trigger(void);
void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);
uint32_t start_tick_, dist_tick_;

int main(void){

    //int i;
    int pi;
    //D
    int default_range = 100;
    int range;

    float Fdistance, Rdistance, Ldistance;
    
    if((pi = pigpio_start(NULL, NULL)) < 0){
        fprintf(stderr, "%s\n", pigpio_error(pi));
        exit(-1); //return 1;
    }
    //D
    set_mode(pi, INPUT1, PI_OUTPUT); set_mode(pi, INPUT2, PI_OUTPUT);
    set_mode(pi, INPUT3, PI_OUTPUT); set_mode(pi, INPUT4, PI_OUTPUT);
    //D
    set_PWM_range(pi, INPUT1, default_range);
    range = get_PWM_range(pi, INPUT1);
    set_PWM_range(pi, INPUT2, default_range);
    set_PWM_range(pi, INPUT3, default_range);
    set_PWM_range(pi, INPUT4, default_range);
    //U
    set_mode(pi, FRONT_TRIG_PINNO, PI_OUTPUT);
    set_mode(pi, FRONT_ECHO_PINNO, PI_INPUT);
    callback(pi, FRONT_ECHO_PINNO, EITHER_EDGE, cb_func_echo);
    gpio_write(pi, FRONT_TRIG_PINNO, PI_OFF);
    //U
    set_mode(pi, RIGHT_TRIG_PINNO, PI_OUTPUT);
    set_mode(pi, RIGHT_ECHO_PINNO, PI_INPUT);
    callback(pi, RIGHT_ECHO_PINNO, EITHER_EDGE, cb_func_echo);
    gpio_write(pi, RIGHT_TRIG_PINNO, PI_OFF);
    //U
    set_mode(pi, LEFT_TRIG_PINNO, PI_OUTPUT);
    set_mode(pi, LEFT_ECHO_PINNO, PI_INPUT);
    callback(pi, LEFT_ECHO_PINNO, EITHER_EDGE, cb_func_echo);
    gpio_write(pi, LEFT_TRIG_PINNO, PI_OFF);
    time_sleep(1);     // delay 1 second

    while(1){
        //Front Ultrasonic sensor
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
        //Right Ultrasonic sensor
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
        //Left Ultrasonic sensor
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, LEFT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.05);
        Ldistance = dist_tick_ / 1000000. * 340 / 2 * 100;
        //Left Ultrasonic sensor
        if(Ldistance > 2 && Ldistance < 200){
            if(dist_tick_ && start_tick_){
                if(Ldistance < 2 || Ldistance > 400)
                    printf("LEFT : %6dus, Distance : %6.1f cm\n", dist_tick_, Ldistance);
                else{ 
                    printf("LEFT : %6dus, Distance : %6.1f cm\n", dist_tick_, Ldistance);
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
        //Reverse
        if((Fdistance <= 5.5) && (Rdistance == Ldistance)){
            set_PWM_dutycycle(pi, INPUT1, 30);
			set_PWM_dutycycle(pi, INPUT3, 30);
			gpio_write(pi, INPUT2, PI_LOW);
			gpio_write(pi, INPUT4, PI_LOW);
            printf("REVERSE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            //time_sleep(0.1);
        }
        //Left_Reverse
        else if((Fdistance <= 5.5) && (Rdistance < Ldistance)){
			set_PWM_dutycycle(pi, INPUT1, 40);
			set_PWM_dutycycle(pi, INPUT3, 30);
			gpio_write(pi, INPUT2, PI_LOW);
			gpio_write(pi, INPUT4, PI_LOW);
            printf("Left_Reverse!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			//time_sleep(0.1);
        }
        //Right_Reverse
        else if((Fdistance <= 5.5) && (Ldistance < Rdistance)){
			set_PWM_dutycycle(pi, INPUT1, 30);
            set_PWM_dutycycle(pi, INPUT3, 40);
			gpio_write(pi, INPUT2, PI_LOW);
			gpio_write(pi, INPUT4, PI_LOW);
            printf("Right_Reverse!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			//time_sleep(0.1);
        }
        //Forward
        else if((Fdistance > 5.5) && (Rdistance == Ldistance)){
            set_PWM_dutycycle(pi, INPUT2, 30);
			set_PWM_dutycycle(pi, INPUT4, 30);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT3, PI_LOW);
            printf("FORWARD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            //time_sleep(0.1);
        }
        //Left_Forward
        else if((Fdistance > 5.5) && (Rdistance < Ldistance)){
			set_PWM_dutycycle(pi, INPUT2, 40);
			set_PWM_dutycycle(pi, INPUT4, 30);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT3, PI_LOW);
            printf("Left_Forward!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			//time_sleep(0.1);
        }
        //Right_Forward
        else if((Fdistance > 5.5) && (Ldistance < Rdistance)){
			set_PWM_dutycycle(pi, INPUT2, 30);
            set_PWM_dutycycle(pi, INPUT4, 40);
			gpio_write(pi, INPUT1, PI_LOW);
			gpio_write(pi, INPUT3, PI_LOW);
            printf("Right_Forward!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			//time_sleep(0.1);
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
