/*
 * pwm.c
 *
 * Created: 07.11.2023 12:42:51
 *  Author: jovm
 */
#include "pwm.h"
#include "sam.h"


#define MCK         84E6

#define PERIOD_A    20E-3
#define DIVA        84
#define CLK_A       1E6
#define CPRDA       (int) (PERIOD_A * MCK / DIVA)
#define DIVB        84
#define CLK_B       1E6


void pwm_init() {
	PIOC->PIO_ABSR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;
	PIOC->PIO_PDR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;

	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PWM << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);

	PWM->PWM_ENA = PWM_ENA_CHID5 | PWM_ENA_CHID6;

	PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(DIVA);
	PWM->PWM_CLK |= PWM_CLK_PREB(0) | PWM_CLK_DIVB(DIVB);

	PWM->PWM_CH_NUM[PIN45].PWM_CMR = PWM_CMR_CPRE_CLKA;
	PWM->PWM_CH_NUM[PIN44].PWM_CMR = PWM_CMR_CPRE_CLKB;
	
	PWM->PWM_CH_NUM[PIN45].PWM_CPRD = PWM_CPRD_CPRD(CPRDA);
	PWM->PWM_CH_NUM[PIN44].PWM_CPRD = PWM_CPRD_CPRD(0);
}


void pwm_set_duty_cycle(float duty_cycle, unsigned int channel) {
	if ((duty_cycle < 0.045) || (duty_cycle > 0.105)) {
		return;
	}

	if (channel == PIN45) {
		int CDTY = 0;
		CDTY = (int) (PWM->PWM_CH_NUM[channel].PWM_CPRD*(1 - duty_cycle));
		PWM->PWM_CH_NUM[channel].PWM_CDTY = PWM_CDTY_CDTY(CDTY);
	}
}
