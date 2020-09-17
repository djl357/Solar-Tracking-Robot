#include "analogtodigital.h"

void initializeADC0(void){
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;	// Enable clock to ADC0
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;	// Enable clock to Port B
	ADC0->CFG1 |= 0xC;  //16 bit resolution, use bus clock 
	ADC0->SC1[0] |= 0x1F; // Disable module
}

unsigned int read_ADC0(unsigned int channel){
	ADC0->SC1[0] = channel; //selects DAD0 channel
	while(ADC0->SC2 & 0x80); //set ADACT bit, there is a conversion in progress
	while((ADC0->SC1[0] & 0x80) == 0); //wait until COCO bit is 1, conversion is then complete
	return ADC0->R[0];
}

void initializeADC1(void){
	SIM->SCGC3 |= SIM_SCGC3_ADC1_MASK;	// Turn on ADC 0 clock
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;	// Turn on PTB0 clock
	ADC1->CFG1 |= 0xC; //16 bit resolution, use bus clock
	ADC1->SC1[0] |= 0x1F; // Disable module
}

unsigned int read_ADC1(unsigned int channel){
	ADC1->SC1[0] = channel; //selects DAD1 channel
	while(ADC1->SC2 & 0x80); //set ADACT bit, there is a conversion in progress
	while((ADC1->SC1[0] & 0x80) == 0); //wait until COCO bit is 1, conversion is then complete
	return ADC1->R[0];
}
