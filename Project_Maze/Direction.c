#include <stdio.h>

#define WIDTH 15

int main(){
    float Fdistance, Rdistance, Ldistance;
    int min;
    int F;
    int R;
    int L;

    if(Fdistance > WIDTH) F=1;
    else if(Fdistance < WIDTH) F=0;

    if(Rdistance > WIDTH) R=1;
    else if(Rdistance < WIDTH) R=0;

    if(Ldistance > WIDTH) L=1;
    else if(Ldistance < WIDTH) L=0;

    if(R == 1)
        //(우회전 && 1칸 직진) && 정지
        //우회전

        //1칸 직진

        //정지

    else {
        if(F == 1){
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

        }else{
            if(L == 1)
            //(좌회전 && 1칸 직진) && 정지
            //좌회전

            //1칸직진

            //정지

            else{
                //(유턴 && 1칸 직진) && 정지
                //유턴

                //1칸 직진

                //정지

            }
        }
    
    }

}