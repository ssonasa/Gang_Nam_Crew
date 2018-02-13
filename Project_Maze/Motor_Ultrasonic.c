#include <math.h>
#include "Functions.h"

//U
void trigger(void);
void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);
uint32_t start_tick_, dist_tick_;

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
    set_ServoMotor(pi);
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
        float sumf[5], sumr[5], suml[5];
        int Fbool, Rbool, Lbool;

        for (int j = 0; j < 3; j++)
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
                        printf("FRONT : Too close or too far\n");
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
                        printf("RIGHT : Too close or too far\n");
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
                        printf("LEFT : Too close or too far\n");
                        WHEEL_STOP(pi);
                    }
                    else
                    {
                        printf("LEFT : %6dus, Distance : %6.1f cm\n", dist_tick_, Ldistance);
                        suml[i] = Ldistance;
                        if (i == 2)
                            printf("\n");
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
            sumf[4] = sumf[3] / 3;
            sumr[4] = sumr[3] / 3;
            suml[4] = suml[3] / 3;
            if (j == 2)
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

        if (sumf[4] > WIDTH)
            Fbool = 1;
        else if (sumf[4] < WIDTH)
            Fbool = 0;

        if (sumr[4] > WIDTH)
            Rbool = 1;
        else if (sumr[4] < WIDTH)
            Rbool = 0;

        if (suml[4] > WIDTH)
            Lbool = 1;
        else if (suml[4] < WIDTH)
            Lbool = 0;

        printf("Fbool = %d, Rbool = %d, Lbool = %d\n", Fbool, Rbool, Lbool);
        
        if (Rbool == 1)
        {
            //우회전
            WHEEL_RIGHT_TRIPLE_ACCEL(pi);
        }
        else if (Rbool == 0)
        {
            if (Fbool == 1)
            {
                if (Rdistance == Ldistance)
                {
                    //직진
                    WHEEL_FORWARD(pi);
                }
                else if (Rdistance < Ldistance)
                {
                    //LEFT_FORWARD
                    WHEEL_LEFT_FORWARD(pi);

                }
                else if (Rdistance > Ldistance)
                {
                    //RIGHT_FORWARD
                    WHEEL_RIGHT_FORWARD(pi);
                }
            }
            else if (Fbool == 0)
            {
                if (Lbool == 1)
                {
                    //좌회전
                    WHEEL_LEFT_TRIPLE_ACCEL(pi);
                }
                else
                {
                    //유턴 자린데 일단 후진 넣어놨음
                    WHEEL_REVERSE(pi);
                }
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
