// LEDSW.c
// Runs on TM4C123
// By Kevin Martinez

#include "tm4c123gh6pm.h"
#include "LEDSW.h"

#define allOnboardLEDSWs	0x1F	// PF4,PF3,PF2,PF1,PF0
#define allOnboardLEDs		0x0E	// PF3,PF2,PF1
#define allOnboardSWs		0x11	// PF4,PF0

void LEDSW_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;			// unlock GPIO Port F
	GPIO_PORTF_CR_R |= allOnboardLEDSWs;		// allow changes to PF4-0
	GPIO_PORTF_AMSEL_R &= ~allOnboardLEDSWs;	// 3) disable analog function
	GPIO_PORTF_PCTL_R &= ~0x000FFFFF;			// 4) GPIO clear bit PCTL
	GPIO_PORTF_DIR_R |= allOnboardLEDs;			// 6) output logic
	GPIO_PORTF_AFSEL_R &= ~allOnboardLEDSWs;	// 7) no alternate function
	GPIO_PORTF_DEN_R |= allOnboardLEDSWs;		// 8) enable digital pins
	LED = Dark;									// Turn off all LEDs.
	
	//SWITCHES
	GPIO_PORTF_DIR_R &= ~allOnboardSWs;					// (c) make input logic
	GPIO_PORTF_PUR_R |= allOnboardSWs;					//     enable weak pull-up restistors
	GPIO_PORTF_IS_R &= ~allOnboardSWs;					// (d) edge-sensitive
	GPIO_PORTF_IBE_R &= ~allOnboardSWs;					//     not both edges
	GPIO_PORTF_IEV_R &= ~allOnboardSWs;					//     falling edge event
	GPIO_PORTF_ICR_R = allOnboardSWs;					// (e) clear flags for SW1 and SW2
	GPIO_PORTF_IM_R |= allOnboardSWs;					// (f) arm interrupt for SW1 and SW2
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00400000;	// (g) bits:23-21 for PORTF, set priority to 2
	NVIC_EN0_R |= 0x40000000;							// (h) enable interrupt 30 in NVIC
}