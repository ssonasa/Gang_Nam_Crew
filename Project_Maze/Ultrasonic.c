#include <unistd.h>
#include "myFunctions.h"

uint32_t start_tick_, dist_tick_; //초음파 시간을 계산하기 위한 변수

void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick)
{
    if (level == PI_HIGH)
        start_tick_ = tick;
    else if (level == PI_LOW)
        dist_tick_ = tick - start_tick_;
}

int main()
{

    int pi;

    if ((pi = pigpio_start(NULL, NULL)) < 0)
        return 1;

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

    time_sleep(1); // delay 1 second

    while (1)
    {
        float Fdistance = 0, Ldistance = 0, Rdistance = 0;
        float sumf = 0, sumr = 0, suml = 0;

        for (int i = 0; i < 3; i++)
        {
            start_tick_ = dist_tick_ = 0;
            gpio_trigger(pi, FRONT_TRIG_PINNO, Pulse_Length, PI_HIGH);
            time_sleep(Sleep_Time); //185cm일때 10930마이크로 초 걸린다. (약0.01초)
            if (dist_tick_ && start_tick_)
            {
                Fdistance = dist_tick_ / 1000000. * 340 / 2 * 100;
                if (Fdistance < 2 || Fdistance > 400)
                    printf("Too close or too far\n");
                else
                {
                    printf("FRONT : %6dus, Distance : %6.1f cm, sumf = %6.1f\n", dist_tick_, Fdistance, sumf);
                    sumf = sumf + Fdistance;
                }
            }
            else
                printf("Sense Error!!!!!\n");

            start_tick_ = dist_tick_ = 0;
            gpio_trigger(pi, RIGHT_TRIG_PINNO, Pulse_Length, PI_HIGH);
            time_sleep(Sleep_Time);
            if (dist_tick_ && start_tick_)
            {
                Rdistance = dist_tick_ / 1000000. * 340 / 2 * 100;
                if (Rdistance < 2 || Rdistance > 400)
                    printf("Too close or too far\n");
                else
                {
                    printf("RIGHT : %6dus, Distance : %6.1f cm\n", dist_tick_, Rdistance);
                    sumr = sumr + Rdistance;
                }
            }
            else
                printf("Sense Error!!!!!\n");

            start_tick_ = dist_tick_ = 0;
            gpio_trigger(pi, LEFT_TRIG_PINNO, Pulse_Length, PI_HIGH);
            time_sleep(Sleep_Time);

            if (dist_tick_ && start_tick_)
            {
                Ldistance = dist_tick_ / 1000000. * 340 / 2 * 100;
                if (Ldistance < 2 || Ldistance > 400)
                    printf("Too close or too far\n");
                else
                {
                    printf("LEFT : %6dus, Distance : %6.1f cm\n", dist_tick_, Ldistance);
                    suml = suml + Ldistance;
                }
            }
            else
                printf("Sense Error!!!!!\n");
        }
        printf("AVG FRONT : %6.1f, Distance : %6.1f cm\n", sumf, sumf / 3);
        printf("AVG RIGHT : %6.1f, Distance : %6.1f cm\n", sumr, sumr / 3);
        printf("AVG LEFT : %6.1f, Distance : %6.1f cm\n", suml, suml / 3);
        
        //Reverse
        if ((Fdistance <= 5.5) && (Rdistance == Ldistance))
        {
            printf("REVERSE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(0.5);
        }
        //Left_Reverse
        else if ((Fdistance <= 5.5) && (Rdistance < Ldistance))
        {
            time_sleep(0.5);
        }
        //Right_Reverse
        else if ((Fdistance <= 5.5) && (Ldistance < Rdistance))
        {
            printf("Right_Reverse!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(0.5);
        }
        //Forward
        else if ((Fdistance > 5.5) && (Rdistance == Ldistance))
        {
            printf("FORWARD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(0.5);
        }
        //Left_Forward
        else if ((Fdistance > 5.5) && (Rdistance < Ldistance))
        {
            printf("Left_Forward!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(0.5);
        }
        //Right_Forward
        else if ((Fdistance > 5.5) && (Ldistance < Rdistance))
        {
            printf("Right_Forward!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            time_sleep(0.5);
        }
        else
        {
            printf("Sense Error!!!!!\n");

            time_sleep(5);
        }
        time_sleep(1);
    }

    pigpio_stop(pi);

    return 0;
}
