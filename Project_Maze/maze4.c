#include <math.h>
#include "Functions.h"

void trigger(void);

int main()
{
    int pi;
    float sumf = 0, sumr = 0, suml = 0;
    int F; //1일때 갈 수 있음
    int R; //1일때 갈 수 있음
    int L; //1일때 갈 수 있음

    if ((pi = pigpio_start(NULL, NULL)) < 0)
    {
        fprintf(stderr, "%s\n", pigpio_error(pi));
        exit(-1);
    }
    set_ServoMotor(pi);
    set_Ultrasonic_sensor(pi);

    WHEEL_STOP(pi);
    time_sleep(3);

    Fgo(pi);
    time_sleep(30);

    while (1)
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
                printf("FRONT : %6dus, Distance : %6.1f cm\n", dist_tick_, Fdistance);
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

        //앞쪽이 열여있을때 else 닫혀있을때
        if (Fdistance > FRONT_WIDTH)
            F = 1;
        else if (Fdistance < FRONT_WIDTH)
            F = 0;
        //오른쪽이 열여있을때 else 닫혀있을때
        if (Rdistance > WIDTH)
            R = 1;
        else if (Rdistance < WIDTH)
            R = 0;
        //왼쪽이 열여있을때 else 닫혀있을때
        if (Ldistance > WIDTH)
            L = 1;
        else if (Ldistance < WIDTH)
            L = 0;

        printf("%f %f %f \n", Fdistance, Rdistance, Ldistance);
        time_sleep(0.05);

        //오른손 법칙(?)으로 오른쪽이 열여있을때를 가장 먼저 확인한다.
        if (R == 1)
        {
            printf("*****Rturn\n");
            Rturn(pi);
        }
        else
        {
            if (F == 1)
            {
                Go(0, pi);
            }
            else
            {
                if (L == 1)
                {
                    //(좌회전 && 1칸 직진) && 정지
                    printf("*****Lturn\n");
                    printf("\n\nfront %6.1f cm\n\n", Fdistance);
                    Lturn(pi);
                }

                else
                {
                    //(유턴 && 1칸 직진) && 정지
                    printf("*****Uturn\n");
                    Uturn(pi);
                }
            }
        }
    }
}
