#include <stdio.h>
#include <pigpio.h>
#include "74hc595_functions.h"

void dot(int row, int col)
{
    uint8_t row8, col8;
    uint16_t tmp;
    row8 = ~(1 << (8-row));
    col8 = 1 << (8-col);
    tmp = (row8<<8) | col8;
    set16(tmp);
}

int cnt;
void callback(void)
{
//	printf("1000 milliseconds have elapsed\n");
	cnt++;	
}
 
int main()
{
    char ch;
    int k=1, n=0;
    int count=0;

    while(1){
    ch = fgetc(stdin);
    printf("%c",ch);
    if(ch == '\n' ){
    break;
    }
    count++;
    }
   
    uint8_t mat[8][count];

    for(int i=0; i<count;i++){
        mat[0][i] = 0;
    }


    while(1){
    ch = fgetc(stdin);
    printf("%c",ch);
    if(ch == EOF || (k == 8))
        break;
    
    if(ch == ' '){
        mat[k][n] = 0;
        n++;
    }else if(ch == 'o'){
        mat[k][n] = 1;
        n++;
    }else if(ch == '\n' ){
        k++;
        n = 0;
    }
    }
    
    
    int i,j;
    int total_col;
    
    
    total_col = sizeof(mat) / 8;

    int ret;
    ret = init();
    if(ret == 0)
        return 0;

    gpioSetTimerFunc(0, 100, callback);

    while(1)
        for(i = 0 ; i < 8 ; i++)
            for(j = 0 ; j < 8 ; j++)
                if(mat[i][(j+cnt)%total_col] == 1)
                    dot(i+1,j+1);


    release();
    
    
    return 0;
}

