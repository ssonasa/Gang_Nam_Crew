EXAMPLES = DC_Motor.out \
		Motor_Ultrasonic.out \
		Ultrasonic.out \
		stop.out\
		TurnLR.out\
		test.out\

CFLAGS = -Wall -W -Wno-unused-parameter
LIBS = -pthread -lrt -lpigpiod_if2

all : $(EXAMPLES)



DC_Motor.out : DC_Motor.c
	gcc $(CFLAGS) $(LIBS) $^ Functions.c  -o $@

Motor_Ultrasonic.out : Motor_Ultrasonic.c
	gcc $(CFLAGS) $(LIBS) $^ Functions.c -o $@

Ultrasonic.out  : Ultrasonic.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

stop.out : stop.c
	gcc $(CFLAGS) $(LIBS) $^ Functions.c -o $@

TurnLR.out : TurnLR.c
	gcc $(CFLAGS) $(LIBS) $^ Functions.c -o $@

test.out : test.c
	gcc $(CFLAGS) $(LIBS) $^ Functions.c -o $@
clean : 
	-rm $(EXAMPLES)

