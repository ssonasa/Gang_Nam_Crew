#include "myFunctions.h"

void cb_func_echo(int pi, unsigned gpio, unsigned level, uint32_t tick)
{
	if (level == PI_HIGH)
		start_tick_ = tick;
	else if (level == PI_LOW)
		dist_tick_ = tick - start_tick_;
}

void set_Ultrasonic_sensor(int pi)
{
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
}

void set_ServoMotor(int pi)
{
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

int Go(double arg, int pi)
{

    int move = 0;
    if (arg != 0)
    {
        Fgo(pi);
        time_sleep(arg);
        WHEEL_STOP(pi);
        // time_sleep(0.5);
    }
    else
    {
        if (Rdistance > Ldistance)
        {
            move = 1;
        }
        else if (Rdistance < Ldistance)
        {
            move = 2;
        }
        else if (Rdistance == Ldistance)
        {
            move = 3;
        }

        switch (move)
        {
        case 0:

            return -1;
        case 1: //오른쪽 벽에서 멀어질 때
            Rgo(pi);
            break;
        case 2: //오른쪽 벽에 가까워졌을 때
            Lgo(pi);
            break;
        case 3: //거리가 적절할 때
            Fgo(pi);
            break;
        default:

            return -1;
        }
    }

    return move;
}
void Rturn(int pi)
{
    Go(0.15, pi);
    state = 3;
    //WHEEL_RIGHT_TRIPLE_ACCEL(pi);
    printf("┌─────┐\n│Rturn│\n└─────┘\n");
    printf("┌─────┐┌─────┐\n│LF=%d││RR=%d│\n└─────┘└─────┘\n",TURN_SPD,TURN_SPD);
    set_PWM_dutycycle(pi, LF, TURN_SPD);
    set_PWM_dutycycle(pi, RR, TURN_SPD);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RF, PI_LOW);
    time_sleep(TURN_90);
    WHEEL_STOP(pi);
    time_sleep(0.5);
    if (Ldistance < (WIDTH / 2))
    {
        printf("< width/2\n");
        Go(1.5, pi);
    }
    else
    {
        printf("> width/2\n");
        Go(1.0, pi);
    }
    state = 3;
}
void Lturn(int pi)
{
    Go(0.15, pi);
    state = 3;
    printf("┌─────┐\n│Lturn│\n└─────┘\n");
    printf("┌─────┐┌─────┐\n│LR=%d││RF=%d│\n└─────┘└─────┘\n",TURN_SPD,TURN_SPD);
    set_PWM_dutycycle(pi, LF, PI_LOW);
    set_PWM_dutycycle(pi, RR, PI_LOW);
    set_PWM_dutycycle(pi, LR, TURN_SPD);
    set_PWM_dutycycle(pi, RF, TURN_SPD);
    time_sleep(TURN_90);
    WHEEL_STOP(pi);
    time_sleep(0.5);
    if (Rdistance < (WIDTH / 2))
    {
        printf("< width / 2\n");
        Go(1.5, pi);
    }
    else
    {
        printf("> witdh / 2\n");
        Go(1.0, pi);
    }
    state = 3;
}
void Uturn(int pi)
{
    printf("┌─────┐\n│Uturn│\n└─────┘\n");
    printf("┌─────┐┌─────┐\n│LF=%d││RR=%d│\n└─────┘└─────┘\n",TURN_SPD,TURN_SPD);
    set_PWM_dutycycle(pi, LF, TURN_SPD);
    set_PWM_dutycycle(pi, RR, TURN_SPD);
    set_PWM_dutycycle(pi, LR, PI_LOW);
    set_PWM_dutycycle(pi, RF, PI_LOW);
    time_sleep(TURN_180);
    WHEEL_STOP(pi);
    time_sleep(1);
}

//Minor func
void Rgo(int pi)
{ //왼쪽 바퀴 속도 빨라지게
    // WHEEL_FORWARD(pi);
    if (state == 1)
        return;
    else
    {
        state = 1;
        set_PWM_dutycycle(pi, LF, WHEEL_SPEED_HIGH);
        set_PWM_dutycycle(pi, RF, WHEEL_SPEED_LOW);
        set_PWM_dutycycle(pi, LR, PI_LOW);
        set_PWM_dutycycle(pi, RR, PI_LOW);
        printf("┌──┐\n│RF│\n└──┘\n");
        printf("┌─────┐┌─────┐\n│LF=%d││RF=%d│\n└─────┘└─────┘\n",WHEEL_SPEED_HIGH,WHEEL_SPEED_LOW);
        //time_sleep(1);
        printf("=======RF END\n");
        //WHEEL_STOP(pi);
        //time_sleep(0.1);
    }
}
void Lgo(int pi)
{ //오른쪽 바퀴 속도 빠르게
    //66, 60
    if (state == 2)
        return;
    else
    {
        state = 2;
        set_PWM_dutycycle(pi, LF, WHEEL_SPEED_LOW);
        set_PWM_dutycycle(pi, RF, WHEEL_SPEED_HIGH+3);
        set_PWM_dutycycle(pi, LR, PI_LOW);
        set_PWM_dutycycle(pi, RR, PI_LOW);
        printf("┌──┐\n│LF│\n└──┘\n");
        printf("┌─────┐┌─────┐\n│LF=%d││RF=%d│\n└─────┘└─────┘\n",WHEEL_SPEED_LOW,WHEEL_SPEED_HIGH);
        //time_sleep(1);
        printf("=======LF END\n");
        //WHEEL_STOP(pi);
        //time_sleep(0.1);
    }
}
void Fgo(int pi)
{ //속도 같게
    if (state == 0)
        return;
    else
    {
        state = 3;
        set_PWM_dutycycle(pi, LF, WHEEL_SPEED_MIDDLE);
        set_PWM_dutycycle(pi, RF, WHEEL_SPEED_MIDDLE);
        set_PWM_dutycycle(pi, LR, PI_LOW);
        set_PWM_dutycycle(pi, RR, PI_LOW);
        printf("┌──┐\n│FF│\n└──┘\n");
        printf("┌─────┐┌─────┐\n│LF=%d││RF=%d│\n└─────┘└─────┘\n",WHEEL_SPEED_MIDDLE,WHEEL_SPEED_MIDDLE);
        // time_sleep(1);
        //WHEEL_STOP();
    }
}
void WHEEL_FORWARD(int pi)
{
	set_PWM_dutycycle(pi, LF, WHEEL_SPEED_MIDDLE);
	set_PWM_dutycycle(pi, RF, WHEEL_SPEED_MIDDLE);
	set_PWM_dutycycle(pi, LR, PI_LOW);
	set_PWM_dutycycle(pi, RR, PI_LOW);
	printf("Forward\n");
	time_sleep(4000);
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
