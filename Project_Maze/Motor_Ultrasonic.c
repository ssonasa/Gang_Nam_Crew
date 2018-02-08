#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include <math.h>

//D
#define LF 17
#define LR 22
#define RF 24
#define RR 23
//U
#define FRONT_TRIG_PINNO 16
#define FRONT_ECHO_PINNO 13
#define RIGHT_TRIG_PINNO 19
#define RIGHT_ECHO_PINNO 6
#define LEFT_TRIG_PINNO 21
#define LEFT_ECHO_PINNO 20
//D
#define DUTYCYCLE(x, range) x / (float)range * 100
//U
void trigger(void);
void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);
uint32_t start_tick_, dist_tick_;

#define Front_MIN_LENGTH 8
#define Right_MIN_LENGTH 6
#define Left_MIN_LENGTH 6

#define WHEEL_SPEED_HIGH 70
#define WHEEL_SPEED_MIDDLE 50
#define WHEEL_SPEED_LOW 68

void WHEEL_STOP(void)
{
    set_PWM_dutycycle(pi, LF, PI_LOW);
    set_PWM_dutycycle(pi, RF, PI_LOW);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RR, PI_LOW);
}
int main(void)
{

    //int i;
    int pi;
    //D
    int default_range = 100;
    int range;

    float Fdistance, Rdistance, Ldistance;

    if ((pi = pigpio_start(NULL, NULL)) < 0)
    {
        fprintf(stderr, "%s\n", pigpio_error(pi));
        exit(-1); //return 1;
    }
    //D
    set_mode(pi, LF, PI_OUTPUT);
    set_mode(pi, LR, PI_OUTPUT);
    set_mode(pi, RF, PI_OUTPUT);
    set_mode(pi, RR, PI_OUTPUT);
    //D
    set_PWM_range(pi, LF, default_range);
    range = get_PWM_range(pi, LF);
    set_PWM_range(pi, LR, default_range);
    set_PWM_range(pi, RF, default_range);
    set_PWM_range(pi, RR, default_range);
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
    time_sleep(1); // delay 1 second

    while (1)
    {
        //Front Ultrasonic sensor
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, FRONT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.02);
        Fdistance = dist_tick_ / 1000000. * 340 / 2 * 100;
        if (Fdistance > 2 && Fdistance < 200)
        {
            if (dist_tick_ && start_tick_)
            {
                if (Fdistance < 2 || Fdistance > 400)
                    printf("FRONT_Distance : %6.1f cm\n", Fdistance);
                else
                    printf("FRONT_Distance : %6.1f cm\n", Fdistance);
            }
        }
        else
        {
            printf("2cm > From Wall to FRONT Sensor || From Wall to FRONT Sensor > 200cm\n");
            set_PWM_dutycycle(pi, LF, 0);
            set_PWM_dutycycle(pi, LR, 0);
            set_PWM_dutycycle(pi, RF, 0);
            set_PWM_dutycycle(pi, RR, 0);
        } //Front Ultrasonic sensor
        //Right Ultrasonic sensor
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, RIGHT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.02);
        Rdistance = dist_tick_ / 1000000. * 340 / 2 * 100;
        if (Rdistance > 2 && Rdistance < 200)
        {
            if (dist_tick_ && start_tick_)
            {
                if (Rdistance < 2 || Rdistance > 400)
                    printf("RIGHT Distance : %6.1f cm\n", Rdistance);
                else
                    printf("RIGHT Distance : %6.1f cm\n", Rdistance);
            }
        }
        else
        {
            printf("2cm > From Wall to RIGHT Sensor || From Wall to RIGHT Sensor > 200cm\n");
            set_PWM_dutycycle(pi, LF, 0);
            set_PWM_dutycycle(pi, LR, 0);
            set_PWM_dutycycle(pi, RF, 0);
            set_PWM_dutycycle(pi, RR, 0);
        } //Right Ultrasonic sensor
        //Left Ultrasonic sensor
        start_tick_ = dist_tick_ = 0;
        gpio_trigger(pi, LEFT_TRIG_PINNO, 10, PI_HIGH);
        time_sleep(0.02);
        Ldistance = dist_tick_ / 1000000. * 340 / 2 * 100;
        if (Ldistance > 2 && Ldistance < 200)
        {
            if (dist_tick_ && start_tick_)
            {
                if (Ldistance < 2 || Ldistance > 400)
                    printf("LEFT Distance : %6.1f cm\n", Ldistance);
                else
                    printf("LEFT Distance : %6.1f cm\n", Ldistance);
            }
        }
        else
        {
            printf("2cm > From Wall to LEFT Sensor || From Wall to LEFT Sensor > 200cm\n");
            set_PWM_dutycycle(pi, LF, 0);
            set_PWM_dutycycle(pi, LR, 0);
            set_PWM_dutycycle(pi, RF, 0);
            set_PWM_dutycycle(pi, RR, 0);
        } //Left Ultrasonic sensor

        if (Fdistance > Front_MIN_LENGTH)
        {
            if (abs(Ldistance - Rdistance) < 1.5)
            {
                set_PWM_dutycycle(pi, LF, WHEEL_SPEED_MIDDLE);
                set_PWM_dutycycle(pi, RF, WHEEL_SPEED_MIDDLE);
                set_PWM_dutycycle(pi, LR, PI_LOW);
                set_PWM_dutycycle(pi, RR, PI_LOW);
                printf("Forward\n");
                time_sleep(1);
                WHEEL_STOP();
            }
            else if (Ldistance < Rdistance)
            {
                set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
                set_PWM_dutycycle(pi, RF, WHEEL_SPEED_LOW);
                set_PWM_dutycycle(pi, LR, PI_LOW);
                set_PWM_dutycycle(pi, RR, PI_LOW);
                printf("Right_Forward\n");
                time_sleep(1);
                WHEEL_STOP();
            }
            else if (Rdistance < Ldistance)
            {
                set_PWM_dutycycle(pi, LF, WHEEL_SPEED_LOW);
                set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH);
                set_PWM_dutycycle(pi, LR, PI_LOW);
                set_PWM_dutycycle(pi, RR, PI_LOW);
                printf("Left_Forward\n");
                time_sleep(1);
                WHEEL_STOP();
            }
            else
            {
                set_PWM_dutycycle(pi, LR, WHEEL_SPEED_MIDDLE);
                set_PWM_dutycycle(pi, RR, WHEEL_SPEED_MIDDLE);
                set_PWM_dutycycle(pi, LF, PI_LOW);
                set_PWM_dutycycle(pi, RF, PI_LOW);
                printf("Reverse\n");
                time_sleep(1);
                WHEEL_STOP();
            }
        }
        else
        {
            printf("======Front <<<<<<<<<< 5 => REVERSE======\n");
            if (Ldistance < Rdistance)
            {

                set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH);
                set_PWM_dutycycle(pi, LR, WHEEL_SPEED_LOW);
                set_PWM_dutycycle(pi, RR, PI_LOW);
                set_PWM_dutycycle(pi, LF, PI_LOW);
                printf("Left_Triple_Accel\n");
                time_sleep(1);
                WHEEL_STOP();
            }
            else if (Rdistance < Ldistance)
            {
                set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
                set_PWM_dutycycle(pi, RR, WHEEL_SPEED_LOW);
                set_PWM_dutycycle(pi, LR, PI_LOW);
                set_PWM_dutycycle(pi, RF, PI_LOW);
                printf("Right_Triple_Accel\n");
                time_sleep(1);
                WHEEL_STOP();
            }
        }
    }
    pigpio_stop(pi);

    return 0;
}

void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick)
{
    if (level == PI_HIGH)
        start_tick_ = tick;
    else if (level == PI_LOW)
        dist_tick_ = tick - start_tick_;
}
