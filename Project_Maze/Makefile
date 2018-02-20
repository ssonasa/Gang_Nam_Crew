EXAMPLES = DC_Motor.out \
		Motor_Ultrasonic.out \
		Ultrasonic.out \
		stop.out\
		TurnLR.out\
		test.out\
		maze4.out\
		maze3.out\
	

CFLAGS = -Wall -W -Wno-unused-parameter
LIBS = -pthread -lrt -lpigpiod_if2

all : $(EXAMPLES)



DC_Motor.out : DC_Motor.c
	gcc $(CFLAGS) $(LIBS) $^ myFunctions.c  -o $@

Motor_Ultrasonic.out : Motor_Ultrasonic.c
	gcc $(CFLAGS) $(LIBS) $^ myFunctions.c -o $@

Ultrasonic.out  : Ultrasonic.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

stop.out : stop.c
	gcc $(CFLAGS) $(LIBS) $^ myFunctions.c -o $@

TurnLR.out : TurnLR.c
	gcc $(CFLAGS) $(LIBS) $^ myFunctions.c -o $@

test.out : test.c
	gcc $(CFLAGS) $(LIBS) $^ myFunctions.c -o $@

maze4.out : maze4.c
	gcc $(CFLAGS) $(LIBS) $^ Functions.c -o $@

maze3.out : maze3.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

clean : 
	-rm $(EXAMPLES)

