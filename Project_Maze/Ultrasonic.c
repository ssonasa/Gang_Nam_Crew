#include <stdio.h>
#include <unistd.h>
#include <pigpiod_if2.h>

#define FRONT_TRIG_PINNO 16
#define FRONT_ECHO_PINNO 13
#define RIGHT_TRIG_PINNO 19
#define RIGHT_ECHO_PINNO 6
#define LEFT_TRIG_PINNO 21
#define LEFT_ECHO_PINNO 20

#define Pulse_Length 10

void trigger(void);
void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);

uint32_t start_tick_, dist_tick_;

int main()
{
    float Fdistance, Ldistance, Rdistance;
    int pi;

    if ((pi = pigpio_start(NULL, NULL)) < 0) return 1;

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

    printf("Raspberry Pi HC-SR04 UltraSonic sensor\n" );

    while(1){
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, FRONT_TRIG_PINNO, Pulse_Length, PI_HIGH);
        time_sleep(0.05);   //185cm일때 10930마이크로 초 걸린다. (약0.01초)

        if(dist_tick_ && start_tick_){
            //distance = (float)(dist_tick_) / 58.8235;
            Fdistance = dist_tick_ / 1000000. * 340 / 2 * 100;
            if(Fdistance < 2 || Fdistance > 400)
                printf("range error\n");
            else 
                printf("FRONT : %6dus, Distance : %6.1f cm\n", dist_tick_, Fdistance);
        }
        else
            printf("sense error\n");
        
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, RIGHT_TRIG_PINNO, Pulse_Length, PI_HIGH);
        time_sleep(0.05);
        if(dist_tick_ && start_tick_){
            //distance = (float)(dist_tick_) / 58.8235;
            Rdistance = dist_tick_ / 1000000. * 340 / 2 * 100;
            if(Rdistance < 2 || Rdistance > 400)
                printf("range error\n");
            else 
                printf("RIGHT : %6dus, Distance : %6.1f cm\n", dist_tick_, Rdistance);
        }
        else
            printf("sense error\n");
        
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, LEFT_TRIG_PINNO, Pulse_Length, PI_HIGH);
        time_sleep(0.05);

        if(dist_tick_ && start_tick_){
            //distance = (float)(dist_tick_) / 58.8235;
            Ldistance = dist_tick_ / 1000000. * 340 / 2 * 100;
            if(Ldistance < 2 || Ldistance > 400)
                printf("range error\n");
            else 
                printf("LEFT : %6dus, Distance : %6.1f cm\n", dist_tick_, Ldistance);
        }
        else
            printf("sense error\n");

        //Reverse
        if((Fdistance <= 5.5) && (Rdistance == Ldistance)){
            printf("REVERSE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(0.5);
        }
        //Left_Reverse
        else if((Fdistance <= 5.5) && (Rdistance < Ldistance)){
			time_sleep(0.5);
        }
        //Right_Reverse
        else if((Fdistance <= 5.5) && (Ldistance < Rdistance)){
            printf("Right_Reverse!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			time_sleep(0.5);
        }
        //Forward
        else if((Fdistance > 5.5) && (Rdistance == Ldistance)){
            printf("FORWARD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(0.5);
        }
        //Left_Forward
        else if((Fdistance > 5.5) && (Rdistance < Ldistance)){
            printf("Left_Forward!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			time_sleep(0.5);
        }
        //Right_Forward
        else if((Fdistance > 5.5) && (Ldistance < Rdistance)){
            printf("Right_Forward!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			time_sleep(0.5);
        }
        else{
            printf("sense error\n");

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
