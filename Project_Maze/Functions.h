#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>

//모터 좌,우 +,- 해당 gpio핀 번호
#define LF 17
#define LR 22
#define RF 24
#define RR 23

//바퀴 속도
#define WHEEL_SPEED_HIGH 100
#define WHEEL_SPEED_MIDDLE 100
#define WHEEL_SPEED_LOW 58

#define DUTYCYCLE(x, range) x / (float)range * 100

//초음파 센서 gpio핀 번호
#define FRONT_TRIG_PINNO 16
#define FRONT_ECHO_PINNO 13
#define RIGHT_TRIG_PINNO 19
#define RIGHT_ECHO_PINNO 6
#define LEFT_TRIG_PINNO 21
#define LEFT_ECHO_PINNO 20

//초음파센서 최소거리
#define Front_MIN_LENGTH 8
#define Right_MIN_LENGTH 6
#define Left_MIN_LENGTH 6

#define Pulse_Length 10
#define Sleep_Time 0.05

#define WIDTH 15


//기본 셋팅 함수
//void set_Ultrasonic_sensor(int pi);
void set_ServoMotor(int pi);

//바퀴 굴림 함수
void WHEEL_STOP(int pi);
void WHEEL_FORWARD(int pi);
void WHEEL_RIGHT_FORWARD(int pi);
void WHEEL_LEFT_FORWARD(int pi);
void WHEEL_REVERSE(int pi);
void WHEEL_RIGHT_TRIPLE_ACCEL(int pi);
void WHEEL_LEFT_TRIPLE_ACCEL(int pi);
void WHEEL_RIGHT_TURN(int pi);
void WHEEL_LEFT_TURN(int pi);

//void trigger(void);
//void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick);
