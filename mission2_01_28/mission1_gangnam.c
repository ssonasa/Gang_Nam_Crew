#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include <time.h>

#define BAUD_RATE   240000
#define LOOP      10
#define LED_PINNO   2
typedef long clock_t;

void delay(clock_t n){
    clock_t start=clock();
    while(clock()-start<n);
}

int main(int argc, char* argv[])
{
    FILE *f = fopen("data.txt","w");
  
    int pi;
    int spi;
    uint16_t value;
    uint64_t sum = 0;
    uint16_t avg;
    int channel = 0;
    char snd_buf[3];
    char rcv_buf[LOOP][3];
    uint32_t start_tick, diff_tick;
    float volt;

    if(argc == 2) channel = atoi(argv[1]);

    if((pi = pigpio_start(NULL, NULL)) < 0) {
        fprintf(stderr, "pigpio_start error\n"); 
        return 1;
    }

    if((spi = spi_open(pi,0,BAUD_RATE,  0)) < 0) {
        fprintf(stderr, "spi_open error\n"); 
        return 2;
    }

    set_mode(pi, LED_PINNO, PI_OUTPUT);
   
    clock_t start, end;
    int i;
    double second;

    start=clock();
  //  printf("start: %.3lf\n",start);
    
  while(second<30){
//  for(i=0; i<10; i++){  
        end=clock();
        second=(double)(end-start)/1000000;
        
           printf("time: %.1f, ",second);
           fprintf(f,"%.1f ",second);
        
        avg = sum = 0;
        snd_buf[0] = 0x18 | channel;
        start_tick = get_current_tick(pi);
        
        for(int i = 0 ; i < LOOP ; i++){
            spi_xfer(pi, spi, snd_buf, rcv_buf[i], 3);
            delay(10000);
        }
        diff_tick = get_current_tick(pi) - start_tick;

        for(int i = 0 ; i < LOOP ; i++){
            value = ((rcv_buf[i][1] & 0x3f) << 8) | (rcv_buf[i][2] & 0xff);
            value = value >> 2;
            sum += value;
        }
        avg = sum / LOOP;
        volt=3.3*avg/4095;
        
        fprintf(f, "%.1f\n",volt);
  
        //    printf("avg : %4d\n", avg);
    //    printf("channel-%d : %4d %.1fv", channel, avg, 3.3*avg/4095);
    //    printf("\t%lld sps\n", 1000000LL * LOOP / diff_tick);

        if((3.3*avg/4095)<=1.5){
            printf("%.1fv : LED ON\n", 3.3*avg/4095);
            gpio_write(pi, LED_PINNO, 0);
        }
        else
        {
            printf("%.1fv : LED OFF\n", 3.3*avg/4095);
            gpio_write(pi, LED_PINNO, 1);
        }
        
    }
    fclose(f);
   spi_close(pi, spi);
   pigpio_stop(pi);
   return 0;
}