#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

float Fdistance, Rdistance, Ldistance;
int state;
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

#define DUTYCYCLE(x, range) x / (float)range * 100

#define WAIT_TIME 0.01

#define Front_MIN_LENGTH 8
#define Right_MIN_LENGTH 6
#define Left_MIN_LENGTH 6

#define WHEEL_SPEED_HIGH 55
#define WHEEL_SPEED_MIDDLE 55
#define WHEEL_SPEED_LOW 50
//LF70, RF65일때 120cm 직진 가능

//Turn 상수
#define TURN_90 0.4
#define TURN_45 0.5
#define TURN_180 1.1
#define TURN_SPD 61
/* 속도61, 시간 0.5 = 77.14도 회전
   속도61, 시간 0.6 = 약 88~92도 회전? */

#define WIDTH 25
#define FRONT_WIDTH 15
#define Pulse_Length 10
#define Sleep_Time 0.01

uint32_t start_tick_, dist_tick_;
/*state의 용도
  state가 변할 떄만 바퀴속도 값을 바꿔주기 위하여 있는 변수
  0; Fgo
  1; Rgo
  2; Lgo
  을 의미!!!
 */

//Major func
int Go(double arg, int pi);
void Rturn(int pi);
void Lturn(int pi);
void Uturn(int pi);
void Stop(int arg);
void WHEEL_STOP(int pi);
//Minor func
void Rgo(int pi);
void Lgo(int pi);
void Fgo(int pi);
void WHEEL_RIGHT_TRIPLE_ACCEL(int pi);


void WHEEL_LEFT_TURN(int pi);
void WHEEL_RIGHT_TURN(int pi);
void WHEEL_LEFT_TRIPLE_ACCEL(int pi);
void WHEEL_REVERSE(int pi);
void WHEEL_LEFT_FORWARD(int pi);
void WHEEL_RIGHT_FORWARD(int pi);
void WHEEL_FORWARD(int pi);

//기본 셋팅 함수
void set_Ultrasonic_sensor(int pi);
void set_ServoMotor(int pi);