#include <stdio.h>
#include <unistd.h>
#include <pigpiod_if2.h>

#define FRONT_TRIG_PINNO 16
#define FRONT_ECHO_PINNO 13
#define RIGHT_TRIG_PINNO 19
#define RIGHT_ECHO_PINNO 6
#define LEFT_TRIG_PINNO 21
#define LEFT_ECHO_PINNO 20


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
        gpio_trigger(pi, FRONT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.1);   //돌아올때까지 기다리지 못하면 오류값이 나온다. 거리가 멀어질수록 돌아오는 시간이 길어지니까 적당히 긴 시간동안 기다려야한다.

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
        gpio_trigger(pi, RIGHT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.1);
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
        gpio_trigger(pi, LEFT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.1);

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

        //Forward
        if((Ldistance <= Rdistance+2 && Ldistance >= Rdistance-2) && (Rdistance < Fdistance)){
            /*
            set_PWM_dutycycle(pi, INPUT1, 0);
            set_PWM_dutycycle(pi, INPUT2, Fdistance*20);
            set_PWM_dutycycle(pi, INPUT3, 0);
            set_PWM_dutycycle(pi, INPUT4, Fdistance*20);
            */
            printf("FORWARD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
            time_sleep(1);
        }
        else if((Rdistance <= Fdistance+2 && Rdistance >= Fdistance-2) && (Fdistance < Ldistance)){
            /*
            set_PWM_dutycycle(pi, INPUT1, Ldistance*20);
            set_PWM_dutycycle(pi, INPUT2, 0);
            set_PWM_dutycycle(pi, INPUT3, 0);
            set_PWM_dutycycle(pi, INPUT4, Ldistance*20);     
            */
            printf("LEFT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
            time_sleep(1);       
        }
        else if((Fdistance <= Ldistance+2 && Fdistance >= Ldistance-2) && (Ldistance < Rdistance)){
            /*
            set_PWM_dutycycle(pi, INPUT1, Rdistance*20);
            set_PWM_dutycycle(pi, INPUT2, 0);
            set_PWM_dutycycle(pi, INPUT3, Rdistance*20);
            set_PWM_dutycycle(pi, INPUT4, 0);
            */
            printf("RIGHT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
            time_sleep(1);
        }
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
