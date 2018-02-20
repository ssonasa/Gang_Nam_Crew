
#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include <math.h>
#include "Functions.h"

#define WAIT_TIME 0.01
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

#define WHEEL_SPEED_HIGH 45
#define WHEEL_SPEED_MIDDLE 60
#define WHEEL_SPEED_LOW 40

//Turn 상수
#define TURN_90 0.45
#define TURN_45 0.5
#define TURN_180 1.1
#define TURN_SPD 61
/* 속도61, 시간 0.5 = 77.14도 회전
   속도61, 시간 0.6 = 약 88~92도 회전?

   */

#define WIDTH 25
#define FRONT_WIDTH 15
#define Pulse_Length 10
#define Sleep_Time 0.01
int pi;

//Major func
int Go(double arg);
void Rturn();
void Lturn();
void Uturn();
void Stop(int arg);
void WHEEL_STOP(int pi);
//Minor func
void Rgo();
void Lgo();
void Fgo();
void WHEEL_RIGHT_TRIPLE_ACCEL(int pi);

float Fdistance, Rdistance, Ldistance;
int state=3;
/*state의 용도
  state가 변할 떄만 바퀴속도 값을 바꿔주기 위하여 있는 변수
  0; Fgo
  1; Rgo
  2; Lgo
  을 의미!!!
 */

//U
void trigger(void);
//void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);
uint32_t start_tick_, dist_tick_;

/*
   void WHEEL_STOP(void)
   {
   set_PWM_dutycycle(pi, LF, PI_LOW);
   set_PWM_dutycycle(pi, RF, PI_LOW);
   set_PWM_dutycycle(pi, LR, PI_LOW);
   set_PWM_dutycycle(pi, RR, PI_LOW);
   }
 */
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
    //    WHEEL_STOP(pi);
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

    WHEEL_STOP(pi);
    time_sleep(3);

    Fgo();
    time_sleep(3);

   // double sensor[3][100] = {0};
   // int temp =0;
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
                printf(" FRONT : %6dus, Distance : %6.1f cm, sumf = %6.1f\n", dist_tick_, Fdistance, sumf);
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

        //센서 읽기 끝
        //움직이기
        /*센서값기록해둔것들
        sensor[0][temp]=Fdistance;
        sensor[1][temp]=Rdistance;
        sensor[2][temp]=Ldistance;
        temp++;
        if(temp>99){
            for(temp=0;temp<=99;temp++){
                printf("%f\t%f\t%f\t\n",sensor[0][temp], sensor[1][temp], sensor[2][temp]);
            }
            return 0;
        }
        */


        if(Fdistance >= FRONT_WIDTH) F=1;
        else if(Fdistance < FRONT_WIDTH) F=0;

        if(Rdistance >= WIDTH) R=1;
        else if(Rdistance < WIDTH) R=0;

        if(Ldistance >= WIDTH) L=1;
        else if(Ldistance < WIDTH) L=0;
        printf("%f %f %f \n", Fdistance, Rdistance, Ldistance);
        time_sleep(0.05);
        //R=0;F=0;L=0;//지워야돼!!!!!!!!!!!!!
        if(R == 1){
            printf("*****Rturn\n");
            Rturn();
        }else {
            if(F == 1){
                Go(0);

            }else{
                if(L == 1){
                    //(좌회전 && 1칸 직진) && 정지
                    //좌회전

                    //1칸직진

                    //정지
                    printf("*****Lturn\n");
                    printf("\n\nfront %6.1f cm\n\n",Fdistance);
                    Lturn();
                }

                else{
                    //(유턴 && 1칸 직진) && 정지
                    //유턴

                    //1칸 직진

                    //정지
                    printf("*****Uturn\n");
                    Uturn();
                }
            }

        }
    }

}


int Go(double arg){

    int move=0;
    if(arg!=0){
        Fgo();
        time_sleep(arg);
        WHEEL_STOP(pi);
       // time_sleep(0.5);
    }
    else{

    if(Rdistance>Ldistance){
        move = 1;
    }else if(Rdistance<Ldistance){
        move = 2;
    }else if(Rdistance==Ldistance){
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
    }


    return move;


}
void Rturn(){
    Go(0.3);
    state = 3;
    //WHEEL_RIGHT_TRIPLE_ACCEL(pi);
    printf("┌─────┐\n│Rturn│\n└─────┘\n");
    set_PWM_dutycycle(pi, LF,TURN_SPD);
    set_PWM_dutycycle(pi, RR,TURN_SPD);
    set_PWM_dutycycle(pi, LR,PI_LOW);
    set_PWM_dutycycle(pi, RF,PI_LOW);
    time_sleep(TURN_90);
    WHEEL_STOP(pi);
    time_sleep(0.5);
    if(Ldistance < (WIDTH/2)){
        printf("< width/2\n");
        Go(1.5);
    }else{
        printf("> width/2\n");
        Go(1.0);
    }

    state=3;

}
void Lturn(){
    Go(0.3);
    state = 3;
    printf("┌─────┐\n│Lturn│\n└─────┘\n");
    set_PWM_dutycycle(pi, LF,PI_LOW);
    set_PWM_dutycycle(pi, RR,PI_LOW);
    set_PWM_dutycycle(pi, LR,TURN_SPD);
    set_PWM_dutycycle(pi, RF,TURN_SPD);
    time_sleep(TURN_90);
    WHEEL_STOP(pi);
    time_sleep(0.5);
    if(Rdistance < (WIDTH / 2)){
    printf("< width / 2\n");
    Go(1.5);
    }else{
    printf("> witdh / 2\n");
    Go(1.0);
    }
    state=3;

}
void Uturn(){
    printf("┌─────┐\n│Uturn│\n└─────┘\n");
    set_PWM_dutycycle(pi, LF,TURN_SPD);
    set_PWM_dutycycle(pi, RR,TURN_SPD);
    set_PWM_dutycycle(pi, LR,PI_LOW);
    set_PWM_dutycycle(pi, RF,PI_LOW);
    time_sleep(TURN_180);
    WHEEL_STOP(pi);
    time_sleep(1);

}


//Minor func
void Rgo(){//왼쪽 바퀴 속도 빨라지게
    // WHEEL_FORWARD(pi);
    if(state==1) return;
    else{
        state=1;
    set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH+1);
    set_PWM_dutycycle(pi, RF, WHEEL_SPEED_LOW);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RR, PI_LOW);
    printf("┌──┐\n│RF│\n└──┘\n");
    //time_sleep(1);
    printf("=======RF END\n");
    //WHEEL_STOP(pi);
    //time_sleep(0.1);
    }

}
void Lgo(){//오른쪽 바퀴 속도 빠르게
    //66, 60
    if(state==2) return;
    else{
        state=2;
    set_PWM_dutycycle(pi, LF, WHEEL_SPEED_LOW);
    set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH+2);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RR, PI_LOW);
    printf("┌──┐\n│LF│\n└──┘\n");
    //time_sleep(1);
    printf("=======LF END\n");
    //WHEEL_STOP(pi);
    //time_sleep(0.1);
    }
}
void Fgo(){//속도 같게
    if(state==0) return;
    else{
        state = 0;
    set_PWM_dutycycle(pi, LF, WHEEL_SPEED_MIDDLE);
    set_PWM_dutycycle(pi, RF, WHEEL_SPEED_MIDDLE);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RR, PI_LOW);
    printf("┌──┐\n│FF│\n└──┘\n");
   // time_sleep(1);
    //WHEEL_STOP();
    }
}

void WHEEL_STOP(int pi)
{
        set_PWM_dutycycle(pi, LF, PI_LOW);
        set_PWM_dutycycle(pi, RF, PI_LOW);
        set_PWM_dutycycle(pi, LR, PI_LOW);
        set_PWM_dutycycle(pi, RR, PI_LOW);

}


void WHEEL_RIGHT_TRIPLE_ACCEL(int pi)
{
    set_PWM_dutycycle(pi, LF, WHEEL_SPEED_MIDDLE);
        set_PWM_dutycycle(pi, RR, WHEEL_SPEED_MIDDLE);
        set_PWM_dutycycle(pi, LR, PI_LOW);
        set_PWM_dutycycle(pi, RF, PI_LOW);
        printf("Right_Triple_Accel\n");
        time_sleep(0.5);
        WHEEL_STOP(pi);
        time_sleep(2);
}
