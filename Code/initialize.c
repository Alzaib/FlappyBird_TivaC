#include "initialize.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TExaS.h"
#include <stdbool.h>
#include <stdlib.h>
#include "ADC.h"

// Initializes Port F
void PortF_Init(void){ 
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}


void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
} 


//change gravity by sliding adc
int change_gravity (void){
	int gravity0 = 1;
	if (ADC0_In()<=1023){
		return gravity0 = 1;
	} else if (ADC0_In() > 1023 && ADC0_In() < 3071) {
		return gravity0 = 2;
	} else if (ADC0_In()>3071){
		return gravity0 =3;
	}
}

void PortB_Init(void){ 
  SYSCTL_RCGC2_R |= 0x00000002;     // 1) F clock
  GPIO_PORTB_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTB_CR_R = 0x30;           // allow changes to PF4-0       
  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x30;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTB_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTB_PUR_R = 0x00;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTB_DEN_R = 0x30;          // 7) enable digital pins PF4-PF0        
}
