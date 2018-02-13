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
#define Pulse_Length 10
#define Sleep_Time 0.05
//D
#define DUTYCYCLE(x, range) x / (float)range * 100
//U
void trigger(void);
void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);
uint32_t start_tick_, dist_tick_;
void WHEEL_STOP(int pi);

#define Front_MIN_LENGTH 8
#define Right_MIN_LENGTH 6
#define Left_MIN_LENGTH 6

#define WHEEL_SPEED_HIGH 60
#define WHEEL_SPEED_MIDDLE 50
#define WHEEL_SPEED_LOW 54

void WHEEL_STOP(int pi)
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
        float Fdistance = 0, Ldistance = 0, Rdistance = 0;
        float sumf[4], sumr[4], suml[4];

        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 3; i++)
            {
                start_tick_ = dist_tick_ = 0;
                gpio_trigger(pi, FRONT_TRIG_PINNO, Pulse_Length, PI_HIGH);
                time_sleep(Sleep_Time); //185cm일때 10930마이크로 초 걸린다. (약0.01초)
                if (dist_tick_ && start_tick_)
                {
                    Fdistance = dist_tick_ / 1000000. * 340 / 2 * 100;
                    if (Fdistance < 2 || Fdistance > 400)
                    {
                        printf("Too close or too far\n");
                        WHEEL_STOP(pi);
                    }

                    else
                    {
                        printf("FRONT : %6dus, Distance : %6.1f cm\n", dist_tick_, Fdistance);
                        sumf[i] = Fdistance;
                    }
                }
                else
                {
                    printf("Sense Error!!!!!\n");
                    WHEEL_STOP(pi);
                }

                start_tick_ = dist_tick_ = 0;
                gpio_trigger(pi, RIGHT_TRIG_PINNO, Pulse_Length, PI_HIGH);
                time_sleep(Sleep_Time);
                if (dist_tick_ && start_tick_)
                {
                    Rdistance = dist_tick_ / 1000000. * 340 / 2 * 100;
                    if (Rdistance < 2 || Rdistance > 400)
                    {
                        printf("Too close or too far\n");
                        WHEEL_STOP(pi);
                    }
                    else
                    {
                        printf("RIGHT : %6dus, Distance : %6.1f cm\n", dist_tick_, Rdistance);
                        sumr[i] = Rdistance;
                    }
                }
                else
                {
                    printf("Sense Error!!!!!\n");
                    WHEEL_STOP(pi);
                }

                start_tick_ = dist_tick_ = 0;
                gpio_trigger(pi, LEFT_TRIG_PINNO, Pulse_Length, PI_HIGH);
                time_sleep(Sleep_Time);
                if (dist_tick_ && start_tick_)
                {
                    Ldistance = dist_tick_ / 1000000. * 340 / 2 * 100;
                    if (Ldistance < 2 || Ldistance > 400)
                    {
                        printf("Too close or too far\n");
                        WHEEL_STOP(pi);
                    }
                    else
                    {
                        printf("LEFT : %6dus, Distance : %6.1f cm\n", dist_tick_, Ldistance);
                        suml[i] = Ldistance;
                    }
                }
                else
                {
                    printf("Sense Error!!!!!\n");
                    WHEEL_STOP(pi);
                }
            }

            sumf[3] = sumf[0] + sumf[1] + sumf[2];
            sumr[3] = sumr[0] + sumr[1] + sumr[2];
            suml[3] = suml[0] + suml[1] + suml[2];
            if (j == 3)
            {
                printf("sumf[0] = %f, sumf[1] = %f, sumf[2] = %f\n", sumf[0], sumf[1], sumf[2]);
                printf("sumr[0] = %f, sumr[1] = %f, sumr[2] = %f\n", sumr[0], sumr[1], sumr[2]);
                printf("suml[0] = %f, suml[1] = %f, suml[2] = %f\n", suml[0], suml[1], suml[2]);
                if ((sumf[0] == sumf[1]) && (sumf[1] == sumf[2]))
                {
                    printf("STOP Fromt\n");
                }
                if ((sumr[0] == sumr[1]) && (sumr[1] == sumr[2]))
                {
                    printf("STOP RIGHT\n");
                }
                if ((suml[0] == suml[1]) && (suml[1] == suml[2]))
                {
                    printf("STOP LEFT\n");
                }
            }
        }

        if (Fdistance > Front_MIN_LENGTH)
        {

            if (Ldistance < Rdistance)
            {
                set_PWM_dutycycle(pi, LR, WHEEL_SPEED_HIGH);
                set_PWM_dutycycle(pi, RR, WHEEL_SPEED_LOW);
                set_PWM_dutycycle(pi, LF, PI_LOW);
                set_PWM_dutycycle(pi, RF, PI_LOW);
                printf("Right_Reverse\n");
                time_sleep(1);
                WHEEL_STOP(pi);
                time_sleep(2);
            }
            else if (Rdistance < Ldistance)
            {
                set_PWM_dutycycle(pi, LR, WHEEL_SPEED_HIGH);
                set_PWM_dutycycle(pi, RR, WHEEL_SPEED_HIGH);
                set_PWM_dutycycle(pi, LF, PI_LOW);
                set_PWM_dutycycle(pi, RF, PI_LOW);
                printf("Left_Reverse\n");
                time_sleep(1);
                WHEEL_STOP(pi);
                time_sleep(2);
            }
            else if (abs(Ldistance - Rdistance) < 3)
            {
                set_PWM_dutycycle(pi, LR, WHEEL_SPEED_MIDDLE);
                set_PWM_dutycycle(pi, RR, WHEEL_SPEED_MIDDLE);
                set_PWM_dutycycle(pi, LF, PI_LOW);
                set_PWM_dutycycle(pi, RF, PI_LOW);
                printf("Reverse\n");
                time_sleep(1);
                WHEEL_STOP(pi);
                time_sleep(2);
            }
            else
            {
                set_PWM_dutycycle(pi, LF, WHEEL_SPEED_MIDDLE);
                set_PWM_dutycycle(pi, RF, WHEEL_SPEED_MIDDLE);
                set_PWM_dutycycle(pi, LR, PI_LOW);
                set_PWM_dutycycle(pi, RR, PI_LOW);
                printf("Forward\n");
                time_sleep(1);
                WHEEL_STOP(pi);
                time_sleep(2);
            }
        }
        else
        {
            printf("======Front <<<<<<<<<< 5 => REVERSE======\n");
            if (Ldistance < Rdistance)
            {
                set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
                set_PWM_dutycycle(pi, RR, WHEEL_SPEED_LOW);
                set_PWM_dutycycle(pi, LR, PI_LOW);
                set_PWM_dutycycle(pi, RF, PI_LOW);
                printf("Right_Triple_Accel\n");
                time_sleep(1);
                WHEEL_STOP(pi);
                time_sleep(2);
            }
            else if (Rdistance < Ldistance)
            {
                set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH);
                set_PWM_dutycycle(pi, LR, WHEEL_SPEED_LOW);
                set_PWM_dutycycle(pi, RR, PI_LOW);
                set_PWM_dutycycle(pi, LF, PI_LOW);
                printf("Left_Triple_Accel\n");
                time_sleep(1);
                WHEEL_STOP(pi);
                time_sleep(2);
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
