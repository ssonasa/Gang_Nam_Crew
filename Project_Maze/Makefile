EXAMPLES = DC_Motor.out \
		motor_microwave.out \
		Motor_Ultrasonic.out \
		Ultrasonic.out \
		Gyroscope.out\
		TurnLR.out\

CFLAGS = -Wall -W -Wno-unused-parameter
LIBS = -pthread -lrt -lpigpiod_if2

all : $(EXAMPLES)



DC_Motor.out : DC_Motor.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

motor_microwave.out : motor_microwave.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

Motor_Ultrasonic.out : Motor_Ultrasonic.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

Ultrasonic.out  : Ultrasonic.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

Gyroscope.out : Gyroscope.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@

TurnLR.out : TurnLR.c
	gcc $(CFLAGS) $(LIBS) $^ -o $@
clean : 
	-rm $(EXAMPLES)

