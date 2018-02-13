#include "Functions.h"

/*
uint32_t start_tick_, dist_tick_; //초음파 시간을 계산하기 위한 변수
void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick)
{
    
    if (level == PI_HIGH)
        start_tick_ = tick;
    else if (level == PI_LOW)
        dist_tick_ = tick - start_tick_;
}
void set_Ultrasonic_sensor(int pi){
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
}
*/

void set_ServoMotor(int pi){
    int default_range = 100;
	int range;
	set_mode(pi, LF, PI_OUTPUT);
	set_mode(pi, LR, PI_OUTPUT);
	set_mode(pi, RF, PI_OUTPUT);
	set_mode(pi, RR, PI_OUTPUT);

	set_PWM_range(pi, LF, default_range);
	set_PWM_range(pi, LR, default_range);
	set_PWM_range(pi, RF, default_range);
	set_PWM_range(pi, RR, default_range);
	range = get_PWM_range(pi, LF);
}

void WHEEL_STOP(int pi)
{
	set_PWM_dutycycle(pi, LF, PI_LOW);
	set_PWM_dutycycle(pi, RF, PI_LOW);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
}
void WHEEL_FORWARD(int pi)
{
	set_PWM_dutycycle(pi, LF, WHEEL_SPEED_MIDDLE);
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_MIDDLE);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Forward\n");
	time_sleep(3);
	WHEEL_STOP(pi);
	time_sleep(2);
}
void WHEEL_RIGHT_FORWARD(int pi)
{
	set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_LOW);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Right_Forward\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);
}
void WHEEL_LEFT_FORWARD(int pi)
{
	set_PWM_dutycycle(pi, LF, 50);
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Left_Forward\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);
}
void WHEEL_REVERSE(int pi)
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
void WHEEL_RIGHT_TRIPLE_ACCEL(int pi)
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
void WHEEL_LEFT_TRIPLE_ACCEL(int pi)
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
void WHEEL_RIGHT_TURN(int pi)
{
	set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RF, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Right_Trun\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);
}
void WHEEL_LEFT_TURN(int pi)
{
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, LF, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Left_Trun\n");
	time_sleep(1);
	WHEEL_STOP(pi);
	time_sleep(2);
}



