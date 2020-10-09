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

#define	EYE_LEFT		DDB3
#define	EYE_RIGHT		DDB4

#define	MOUTH_TOP		DDB0
#define	MOUTH_BOTTOM	DDB1
#define	BODY			DDB2

#define TIME_INTERVAL 1

#define TIME_FACTOR(time) (time/TIME_INTERVAL)

#define NORM_LIMIT TIME_FACTOR(1000)

#define SHORT_LIMIT TIME_FACTOR(375)

#define BODY_DARK 0
#define BODY_LEVEL0 30
#define BODY_LEVEL1 25
#define BODY_LEVEL2 20
#define BODY_LEVEL3 15
#define BODY_LEVEL4 10
#define BODY_LEVEL5 5
#define BODY_LEVEL6 2
#define BODY_BRIGHT 1

#define LED_DARK 0
#define LED_BRIGHT 1


// Function 'DoAndCountdown' manipulates LEDS. It returns value which macro PT_WAIT_UNTIL uses as left side condition.
int16_t DoAndCountdown(int16_t initial, int16_t *counter, int16_t bodyLight, int8_t leftEye,int8_t rightEye, int8_t topMouth, int8_t bottomMouth ) {
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
	return *counter;
}

// Protothread procedure 
int proto(struct pt* mlpt, int16_t* i) {
	PT_BEGIN(mlpt); 
		// Lighting Body Up
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_DARK, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL0, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL1, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL2, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL3, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL4, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL5, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL6, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_BRIGHT,LED_BRIGHT ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_BRIGHT,LED_BRIGHT ) == 0);
		// Blinking ...
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT, LED_DARK, LED_DARK,LED_BRIGHT,LED_BRIGHT ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_BRIGHT,LED_BRIGHT ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT, LED_DARK, LED_DARK,LED_BRIGHT,LED_BRIGHT ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_BRIGHT,LED_BRIGHT ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(SHORT_LIMIT, i, BODY_BRIGHT, LED_DARK, LED_DARK,LED_BRIGHT,LED_BRIGHT ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_BRIGHT,LED_BRIGHT ) == 0);
		// Talking
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_BRIGHT,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_DARK,LED_BRIGHT ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_BRIGHT,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_DARK,LED_BRIGHT ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_BRIGHT,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_DARK,LED_BRIGHT ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_BRIGHT, LED_BRIGHT, LED_BRIGHT,LED_BRIGHT,LED_BRIGHT ) == 0);
		// Lighting Body Down
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL6, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL5, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL4, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL3, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL2, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL1, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_LEVEL0, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
		PT_WAIT_UNTIL(mlpt, DoAndCountdown(NORM_LIMIT, i, BODY_DARK, LED_DARK, LED_DARK,LED_DARK,LED_DARK ) == 0);
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
	for(;;) { // main loop
		proto(&lpt,&li);
		_delay_ms(TIME_INTERVAL); 
	}
}
