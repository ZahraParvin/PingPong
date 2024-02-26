/*
 * motor.h
 *
 * Created: 14.11.2023 15:30:03
 *  Author: jovm
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>



typedef enum {
	LEFT,
	RIGHT
}motor_direction;

void motor_init();
uint32_t motor_read_encoder();
uint32_t motor_read_position();


void motor_set_speed(int v,motor_direction d);

static void motor_set_direction(motor_direction d);

void pid_controller(uint32_t position);

void solenoid_init(void);

#endif /* MOTOR_H_ */