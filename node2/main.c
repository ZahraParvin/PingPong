/*
 * GccApplication8.c
 *
 * Created: 01.11.2023 12:37:40
 * Author : jovm
 */ 


#include "sam.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "printf-stdarg.h"
#include "uart.h"
#include "pwm.h"
#include "adc.h"
#include "IR_driver.h"
#include "motor.h"
#include "dac.h"
#include "test_motor.h"


#include <stdint.h>






int main(void){
    /* Initialize the SAM system */
    SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; // watchdog
	//DACC->DACC_WPMR = DACC_WPMR_WPKEY(0xE8);// disable write protection
	configure_uart();
	can_init_def_tx_rx_mb(ATSAM_CAN_BR);
	printf("starting \n\r");
	led_set_d1();
	led_set_d2();
	pwm_init();
	IR_init();
	
// 	tdac_init();
// 	tmotor_init();
// 	tmotor_enable();
// 	tsolenoid_init();
	

	while(1){
		IR_print();
	}

}


// 	CAN_MESSAGE can_msg;
// 	uint8_t mes = can_receive(&can_msg, 0);
// 	printf("%d \n\r", can_msg.data[0]);
// 	//float duty_cycle = 6;//can_msg.data[0]*0.03 + 4.5;
// 	printf("");
// 	//pwm_set_duty_cycle(duty_cycle, PIN44);
// 	pwm_set_duty_cycle(can_msg.data[0]);