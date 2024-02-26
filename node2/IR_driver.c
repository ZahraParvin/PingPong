#include <stdint.h>
#include "IR_driver.h"
#include "adc.h"
#include "sam.h"

#define f_length 100
uint16_t IR_rafilter[f_length];
#define IR_THRESHOLD 2000
uint8_t filter_charge = 0;


void IR_init() {
	adc_init();
}


uint16_t filter_IR() {

	for(int i = 0; i<f_length-1;i++){

		IR_rafilter[i+1] = IR_rafilter[i];
	}
	
	IR_rafilter[0] = adc_read();

	uint32_t total = 0;
	for(int j=0; j<f_length;j++){

		total = total + IR_rafilter[j];
	}

	if(filter_charge < f_length){
		filter_charge ++;
		return IR_THRESHOLD;
	}
	else {
		uint16_t average = total / f_length;
		return average;
	}	
}

int game_over() {
	uint16_t ir = filter_IR();
	if(ir < IR_THRESHOLD) {
		//printf("IR value: %d\n\r",ir );
		return 1;
		}
	else return 0;
}

void IR_print(){
	//printf("IR  Filtered value: %d   Blocked: %d   ADC: %d  \n\r", filter_IR(), game_over(), adc_read());
	printf("\n\r ");
	if (game_over())
	{
		
		printf("Blocked! \n\r");
		
	}
	
}
