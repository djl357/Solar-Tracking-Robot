#include "servo.h"
#include "utils.h"

void initialize_flextimer(void){
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK; //enable clock to FTM0
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; //enable PORTA clock
	PORTA->PCR[1] = PORT_PCR_MUX(3); // choose ALT3 (FTM0_CH6)	
	FTM0->SC |= 0x04F; //Use system clock (21MHz) and Prescaler of 128 to get 164 kHz clock, enables interrupts
	FTM0->CONF = 0x1F; //sets NUMTOF[4:0]=31 so TOF flag is set after 31 overflows
	FTM0->MOD = 3200;  //20 millisecond period
	FTM0->CNTIN = 0x0000;
	FTM0->CONTROLS[6].CnSC |= 0x0028; //sets up Edge-Aligned PWM Mode
	FTM0->CONTROLS[6].CnV = 80; //sets initial servo angle to 0 (duty cycle = x/3200)
	delay();
	delay();
	delay();
}

void move_servo(double angle){
	FTM0->CONTROLS[6].CnV = 32 * (2.5 + ((angle/90)*5)); //calculation for period = 20ms
}