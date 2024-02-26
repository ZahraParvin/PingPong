/*
 * pwm.h
 *
 * Created: 07.11.2023 12:43:03
 *  Author: jovm
 */ 
#include "sam.h"

#ifndef PWM_H_
#define PWM_H_


#define PIN44 6
#define PIN45 5

void pwm_init();
void pwm_set_duty_cycle(float duty_cycle, unsigned int channel);




#endif /* PWM_H_ */