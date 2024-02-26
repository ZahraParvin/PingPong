/*
 * dac.c
 *
 * Created: 14.11.2023 15:53:05
 *  Author: jovm
 */ 
#include "dac.h"
#include "sam.h"
#include <stdio.h>



void dac_init() {
	PMC->PMC_PCER1 |= PMC_PCER1_PID38;
	DACC->DACC_IER |= DACC_IER_TXRDY;
	DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1;
	DACC->DACC_CHER |= DACC_CHER_CH1;
}

void dac_write(int data) {
	while (!(DACC->DACC_ISR | DACC_ISR_TXRDY)){;}
	DACC->DACC_CDR = (uint32_t) data;
	//printf("data: %u \n\r", data);
}
