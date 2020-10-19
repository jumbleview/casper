/*
**  Casper Ghost Halloween Decoration
*/

#include <avr/io.h>
//  CKDIV8 cleared. 8 MHz; if F_CPU   1000000UL // CKDIV8 set. 1 MHz
#define 	F_CPU   8000000UL 
#include <util/delay.h>
#include "../pt-1.4/pt.h" // http://dunkels.com/adam/pt/

#define PinOutHigh(bit) PORTB |= (1 << (bit))

#define PinOutLow(bit) PORTB &= ~(1 << (bit))

#define	EYE_LEFT		PB3
#define	EYE_RIGHT		PB4

#define	MOUTH_TOP		PB0
#define	MOUTH_BOTTOM	PB1
#define	BODY			PB2

#define TIME_INTERVAL 1

#define TIME_FACTOR(time) (time/TIME_INTERVAL)

#define NORM_LIMIT TIME_FACTOR(1000)

#define SHORT_LIMIT TIME_FACTOR(375)

#define DYNAMIC_LIMIT TIME_FACTOR(250)

#define BODY_DARK 0
#define BODY_LEVEL0 37
#define BODY_LEVEL1 25
#define BODY_LEVEL2 20
#define BODY_LEVEL3 15
#define BODY_LEVEL4 10
#define BODY_LEVEL5 5
#define BODY_LEVEL6 2
#define BODY_BRIGHT 1

#define LED_DARK 0
#define LED_BRIGHT 1


// BodyControlAndCountdown supposed to be called out of PW_WAIT_UNTIL macros. It is called out of macros multiple time till counter=0. Each invocation may turn
// LED strips on/off controlling brightness of the strip.
int16_t BodyControlAndCountdown(int16_t initial, int16_t *counter, int16_t bodyLight) {
	int16_t mod = 1;
	if (bodyLight!=0) {
		mod = *counter%bodyLight;
	}
	if (*counter == 0) {
		*counter = initial;
	}
	--(*counter);
	if (mod == 0) {
		PinOutHigh(BODY);
	} else {
		PinOutLow(BODY);
	}
	return *counter;
}

// EyeControl turns LEDs statically. It may be called from protothread routing but not out of  PW_WAIT_UNTIL macros
void EyeControl(int8_t leftEye,int8_t rightEye) {
	if (leftEye) {
		PinOutHigh(EYE_LEFT);
		} else {
		PinOutLow(EYE_LEFT);
	}
	if (rightEye) {
		PinOutHigh(EYE_RIGHT);
		} else {
		PinOutLow(EYE_RIGHT);
	}
}

// EyeControl turns LEDs statically. It may be called from protothread routing but not out of PW_WAIT_UNTIL macros
void MouthControl(int8_t topMouth, int8_t bottomMouth ) {
	if (topMouth) {
		PinOutHigh(MOUTH_TOP);
		} else {
		PinOutLow(MOUTH_TOP);
	}
	if (bottomMouth) {
		PinOutHigh(MOUTH_BOTTOM);
		} else {
		PinOutLow(MOUTH_BOTTOM);
	}
}



// Protothread procedure 
int proto(struct pt* mlpt, int16_t* i, int16_t* j) {
	PT_BEGIN(mlpt); 
		// Everything is dark
		EyeControl(LED_DARK,LED_DARK);
		MouthControl(LED_DARK,LED_DARK);
		PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(NORM_LIMIT, i, BODY_DARK) == 0);
		// Eyes and Mouth are bright, body is dark
		EyeControl(LED_BRIGHT,LED_BRIGHT);
		MouthControl(LED_BRIGHT,LED_BRIGHT);
		PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(NORM_LIMIT, i, BODY_DARK) == 0);
		// Gradually increase brightness of body
		for (*j = BODY_LEVEL0; *j >= 1; (*j)-=2 ) {
				PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(DYNAMIC_LIMIT, i, *j) == 0);
		}
		// Blinking (3 times)
		for (*j = 0; *j < 3; ++(*j)) {
			EyeControl(LED_DARK,LED_DARK);
			PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT) == 0);
			EyeControl(LED_BRIGHT,LED_BRIGHT);
			PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(NORM_LIMIT, i, BODY_BRIGHT) == 0);
		}

		// Talking and blinking (5 times)
		for (*j = 0; *j < 5; ++(*j)) {
			MouthControl(LED_DARK,LED_BRIGHT);
			PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT) == 0);
			MouthControl(LED_DARK,LED_DARK);
			PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT) == 0);
			MouthControl(LED_DARK,LED_BRIGHT);
			PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT) == 0);
			MouthControl(LED_BRIGHT,LED_BRIGHT);
			EyeControl(LED_DARK,LED_DARK);
			PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT) == 0);
			EyeControl(LED_BRIGHT,LED_BRIGHT);
			MouthControl(LED_BRIGHT,LED_DARK);
			PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT) == 0);
			MouthControl(LED_DARK,LED_DARK);
			PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT) == 0);
		}
		EyeControl(LED_DARK,LED_DARK);
		// Gradually decrease brightness of body
		for (*j = BODY_BRIGHT; *j <= BODY_LEVEL0; (*j)+=2){
			PT_WAIT_UNTIL(mlpt, BodyControlAndCountdown(DYNAMIC_LIMIT, i, *j) == 0);
		}

		PT_YIELD(mlpt);
	PT_END(mlpt);
}


void AllLow() {
	PinOutLow(DDB0);
	PinOutLow(DDB1);
	PinOutLow(DDB2);
	PinOutLow(DDB3);
	PinOutLow(DDB4);
}

void AllHigh() {
	PinOutHigh(DDB0);
	PinOutHigh(DDB1);
	PinOutHigh(DDB2);
	PinOutHigh(DDB3);
	PinOutHigh(DDB4);
}

void initialize() {
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4);
	AllHigh();
	_delay_ms(1000.0);
	AllLow();
	_delay_ms(1000.0);
}

int casper(void) {
	initialize();
	struct pt lpt; // protothread descriptor

	PT_INIT(&lpt);
	int16_t li = 0; // function counter: left
	int16_t lj = 0; // function counter: left
	for(;;) { // main loop
		proto(&lpt,&li, &lj);
		_delay_ms(TIME_INTERVAL); 
	}
}
