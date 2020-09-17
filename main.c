#include "servo.h"
#include <MK64F12.h>
#include "utils.h"
#include "analogtodigital.h"
#include "uart.h"
#include "stdio.h"
#include "stdlib.h"
 
int angle = 0;
const int steps = 30;
int channel_0 = 12; //PTB2 on pinout
int channel_1 = 13; //PTB3 on pinout
int channel_2 = 14; //PTB10 on pinout
int channel_3 = 15; //PTB11 on pinout
double data_array[steps];
 
void start_interrupts(void){
	NVIC_EnableIRQ(FTM0_IRQn);             
}
 
void FTM0_IRQHandler(void){
	FTM0->SC &= 0xBF;
  int a = 0;
  int b = 0;
  int c = 0;
  int d = 0;
               
  a = read_ADC0(channel_0);
  b = read_ADC0(channel_1);
  c = read_ADC1(channel_2);
  d = read_ADC1(channel_3);
  double average = (a+b+c+d)/4.0; //averagge photoresistor values at current angle
  data_array[angle] = average; //put average into aray that contains all gathered data             
               
  angle++;
  move_servo((180/steps)*angle);
  LEDGreen_Toggle();
  FTM0->SC &= 0x7F;
  NVIC_ClearPendingIRQ(FTM0_IRQn);
  FTM0->SC |= 0x4F;
}

char * num_to_str(int num){
	char * str = malloc(sizeof(char)*7);
	sprintf(str, "%d", num);
	return str;
}

int main(){
  LED_Initialize();
  initializeADC0();
  initializeADC1();
  initialize_flextimer();
  start_interrupts();
  while(angle<steps);
  NVIC_DisableIRQ(FTM0_IRQn);
  double min_data_value = data_array[0];
  int min_data_index = 0;
	double total_values = data_array[0]; //holds the total of all the points on the sweep
  for(int i  = 1; i < steps; i++){
		total_values += data_array[i];
		if(data_array[i] < min_data_value){
			min_data_value = data_array[i];
      min_data_index = i;
		}
  }
  move_servo((180/steps)*min_data_index); //move to the angle where there is the most light*/
	double average_total_values = total_values / steps;
	int percentage_better = (average_total_values / min_data_value) * 100;
	char * c = num_to_str(min_data_index * 6); //turns servo angle into char array
  uart_init();
	uart_put("\n\r");
	uart_put("The servo is at position: ");
	uart_put(c);
  uart_put("\n\r");
	char * d = num_to_str(percentage_better); //turns percent increase in light at current position into char array
	uart_put("The light at this position is ");
	uart_put(d);
	uart_put("% stronger than the average light.");
  uart_put("\n\r");
  LEDGreen_On();
	free(c);
	free(d);
  while(1);
  return 0;
}