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

#define Front_MIN_LENGTH 8
#define Right_MIN_LENGTH 6
#define Left_MIN_LENGTH 6

#define WHEEL_SPEED_HIGH 51
#define WHEEL_SPEED_MIDDLE 50
#define WHEEL_SPEED_LOW 49

#define WIDTH 13
#define Pulse_Length 10
#define Sleep_Time 0.5
int pi;

//Major func
int Go(int arg);
void Rturn();
void Lturn();
void Uturn();
void Stop(int arg);

//Minor func
void Rgo();
void Lgo();
void Fgo();

float Fdistance, Rdistance, Ldistance;

//U
void trigger(void);
//void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);
uint32_t start_tick_, dist_tick_;


void WHEEL_STOP(void)
{
    set_PWM_dutycycle(pi, LF, PI_LOW);
    set_PWM_dutycycle(pi, RF, PI_LOW);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RR, PI_LOW);
}

void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick)
{
    if (level == PI_HIGH)
        start_tick_ = tick;
    else if (level == PI_LOW)
        dist_tick_ = tick - start_tick_;
}
int main(){
    float sumf = 0, sumr = 0, suml = 0;
    int min;
    int F;//1일때 갈 수 있음
    int R;//1일때 갈 수 있음
    int L;//1일대 갈 수 있음
    int arg;//Go값을위해
    //int i;
    //D
    int default_range = 100;
    int range;

    if ((pi = pigpio_start(NULL, NULL)) < 0)
    {
        fprintf(stderr, "%s\n", pigpio_error(pi));
        exit(-1); //return 1;
    }
    printf("1\n");
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

    while(1){
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
         printf("2\n");
        if(Fdistance > WIDTH) F=1;
        else if(Fdistance < WIDTH) F=0;

        if(Rdistance > WIDTH) R=1;
        else if(Rdistance < WIDTH) R=0;

        if(Ldistance > WIDTH) L=1;
        else if(Ldistance < WIDTH) L=0;
printf("%f %f %f \n", Fdistance, Rdistance, Ldistance);
time_sleep(0.5);
        if(R == 1){
            //(우회전 && 1칸 직진) && 정지
            //우회전
 printf("3\n");
            //1칸 직진

            //정지
            Rturn();}

        else {
            if(F == 1){
                 printf("4\n");
                //(직진 || 우직진 || 좌직진) && 정지
                //직진 함수 넣고
                /* 여기는 직진함수 안에 들어갈 내용
                //직진 || 우직진 || 좌직진
                if(Rdistance == Ldistance)
                //직진
                else if(Rdistance < Ldistance)
                //우직진
                else if(Rdistance > Ldistance)
                //좌직진
                 */
                //정지
                Go(arg);
 printf("5\n");
            }else{
                if(L == 1)
                    //(좌회전 && 1칸 직진) && 정지
                    //좌회전

                    //1칸직진

                    //정지
                    Lturn();

                else{
                    //(유턴 && 1칸 직진) && 정지
                    //유턴

                    //1칸 직진

                    //정지
                    Uturn();
                }
            }

        }
    }

}


int Go(int arg){

    int move=0;
    int num=5;

    if(Rdistance>num){
        move = 1;
    }else if(Rdistance<num){
        move = 2;
    }else if(Rdistance==num){
        move = 3;
    }

    switch(move){

        case 0:

            return -1;
        case 1://오른쪽 벽에서 멀어질 때
            Rgo();
            break;
        case 2://오른쪽 벽에 가까워졌을 때
            Lgo();
            break;
        case 3://거리가 적절할 때
            Fgo();
            break;
        default:

            return -1;



    }

    return move;


}
void Rturn(){

}
void Lturn(){

}
void Uturn(){

}


//Minor func
void Rgo(){//왼쪽 바퀴 속도 빨라지게
    set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
    set_PWM_dutycycle(pi, RF, WHEEL_SPEED_LOW);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RR, PI_LOW);
    printf("=======RF\n");
    time_sleep(1);
    WHEEL_STOP();
}
void Lgo(){//오른쪽 바퀴 속도 빠르게
    set_PWM_dutycycle(pi, LF, WHEEL_SPEED_LOW);
    set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RR, PI_LOW);
    printf("=======LF\n");
    time_sleep(1);
    WHEEL_STOP();
}
void Fgo(){//속도 같게
    set_PWM_dutycycle(pi, LF, WHEEL_SPEED_MIDDLE);
    set_PWM_dutycycle(pi, RF, WHEEL_SPEED_MIDDLE);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RR, PI_LOW);
    printf("=======FF\n");
    time_sleep(1);
    WHEEL_STOP();
}
