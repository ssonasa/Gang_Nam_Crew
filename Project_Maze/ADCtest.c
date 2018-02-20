#include "Nano120_System.h"

#define SIGNAL_UV_START 0x1

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

void Thread(void const *argument); // thread function
osThreadId tid_Thread;             // thread id
osThreadDef(Thread, osPriorityNormal, 1, 0);
// thread object

void US_Thread(void const *argument);
osThreadId tid_USThread; // thread id
osThreadDef(US_Thread, osPriorityHigh, 1, 0);

void Timer_Callback(void const *argument); // thread function
osTimerId timer0;                          // thread id
osTimerDef(timer0_handler, Timer_Callback);

int Init_Thread(void)
{

    printf("Init_Thread\n");
    tid_Thread = osThreadCreate(osThread(Thread), NULL);
    tid_USThread = osThreadCreate(osThread(US_Thread), NULL);

    timer0 = osTimerCreate(osTimer(timer0_handler), osTimerPeriodic, (void *)0);

    return (0);
}
void Timer_Callback(void const *argument)
{

    switch ((uint32_t)argument)
    {

    case 0:
        printf("Argument:0\n");
        osSignalSet(tid_USThread, SIGNAL_UV_START);
        break;
    case 1:
        printf("Argument:Default\n");
        break;
    }
}

int distance = 0;

void US_Thread(void const *argument)
{

    printf_m("US_Thread Start\n");
    while (1)
    {
        osSignalWait(SIGNAL_UV_START, osWaitForever);
        distance = US_4PIN_Distance();
        printf_m("Distance: %d\n", distance);
    }
}
void Thread(void const *argument)
{

    osTimerStart(timer0, 3000); //3ÃÊ¿¡ ÇÑ¹ø¾¿ µ¿ÀÛÇÏ°Ú´Ù.

    printf_m("Thread Start\n");
    while (1)
    {
        if (distance < 50)
        {
            printf_m("Motor STOP...\n");
        }
        else if (distance < 200)
        {
            printf_m("Motor Slow...\n");
        }
        else
        {
            printf_m("Motor Running...\n");
        }
        osDelay(1000);
    }
}
