#ifndef ANALOGTODIGITAL_H_
#define ANALOGTODIGITAL_H_

#include "MK64F12.h"

void initializeADC0(void);
unsigned int read_ADC0(unsigned int channel);
void initializeADC1(void);
unsigned int read_ADC1(unsigned int channel);

#endif