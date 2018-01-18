#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "8x8font.h"

#define BOARD_ROW 8     // board의 Row
#define PADDING 1;     // 문자간의 기본간격
#define LENGTH 100;

uint8_t *board[BOARD_ROW];      // board
int     board_size;

// board에 문자 writing, margin-이전 문자와의 간격
void boardWriter(int fontidx, int margin);

int main(void)
{
    char input[10];
    scanf("%s",input);
    for(int i=0; i<10;i++){
        int temp;
        if(input[i]>=48 && input[i] <=57){
            temp = input[i] +4;
        }else if(input[i]>= 65 && input[i]<=90){
            temp = input[i] - 'A';
        }else if(input[i]>=97 && input[i]<=122){
            temp = input[i] - 71;
        }else{
            temp = 62;
        }

        if(temp >= 0 && temp <= 61)
            boardWriter(temp,0);
        else
            continue;
    }
    
    /*
    boardWriter(18,0);  // 문자 a, 앞 여백 10
    boardWriter(26,0);   // 문자 b, 앞 여백 0
    boardWriter(39,0);   // 문자 c, 앞 여백 0
    boardWriter(32,0);   // 문자 d, 앞 여백 0
    boardWriter(7,5);   // 문자 e, 앞 여백 0
    boardWriter(40,0);   // 문자 d, 앞 여백 0
    boardWriter(40,0);   // 문자 d, 앞 여백 0
    boardWriter(39,0);   // 문자 d, 앞 여백 0
*/
    for(int i = 0 ; i < BOARD_ROW ; i++){
        for(int j = 0 ; j < board_size ; j++)
            printf("%c", board[i][j]);
        printf("\n");
    }
    printf("^d");

    return 0;
}

void boardWriter(int fontidx, int margin)
{
    int i, j;
    static int board_offset = 0; // 현재 문자의 offset
    uint8_t mask;
    uint8_t *new_board[BOARD_ROW];

    int new_board_size = board_offset + margin + FONT_COL + PADDING;

    for(i = 0 ; i < BOARD_ROW ; i++){
        new_board[i] = (uint8_t*)calloc(1, new_board_size);
        memset(new_board[i], ' ', new_board_size);
        memcpy(new_board[i], board[i], board_size);
        free(board[i]);
        board[i] = new_board[i];
    }

    board_size = new_board_size;
    board_offset += margin;

    // writing on the board
    for(i = 0 ; i < FONT_ROW ; i++){      // row
        for(j = 0 ; j < FONT_COL ; j++){  // col
            mask = 0b1 << (7-j);
            if((mask & FONT88[fontidx][i]))
                board[i][j+board_offset] = 'o';
            else
                board[i][j+board_offset] = ' ';
        }
    }
    board_offset = board_size;;
}

