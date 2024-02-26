/*
 * joystick.h
 *
 * Created: 27.10.2023 16:06:59
 *  Author: jovm
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

typedef struct {
	int x_zero;
	int y_zero;
	int x;
	int y;
	int per_x;
	int per_y;
// 	int min_x;
// 	int min_y;
} pos_t;

typedef struct {
	int left;
	int right;
} slider_t;

typedef enum {
	LEFT, RIGHT, UP, DOWN, NEUTRAL
} joy_direction;

volatile int pos_x;
volatile int pos_y;
volatile pos_t pos_joy;
volatile slider_t slider;
volatile joy_direction joy_d;

void adc_init();

int joy_percent(int value, int zero);

uint8_t read_right_button(void);

void control_refresh ();


void pos_calibrate();
pos_t pos_read();

joy_direction pos_direction (pos_t position);



void run_multiboard(void);



#endif /* JOYSTICK_H_ */
