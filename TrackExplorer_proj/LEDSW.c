// LEDSW.c
// Runs on TM4C123
// By Kevin Martinez
#include "tm4c123gh6pm.h"
#include "LEDSW.h"

// LEDs = PF321
// Onboard Switches = PF40
// Used GPIO Port F = PF43210

void LEDSW_Init(void){
	// Port F Initialization
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
	// LEDS init & GPIO Port F switches init 		(PF43210)
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;     // unlock GPIO Port F
	GPIO_PORTF_CR_R |= 0x1F;          // allow changes to PF43210
  GPIO_PORTF_AMSEL_R &= ~0x1F;      // 3) disable analog function PF43210
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF; // 4) GPIO clear bit PCTL
  GPIO_PORTF_DIR_R |= 0x0E;         // 6) PF1-PF3 onboard LED output
  GPIO_PORTF_AFSEL_R &= ~0x1F;      // 7) no alternate function     
  GPIO_PORTF_DEN_R |= 0x1F;         // 8) enable digital pins PF3-PF1
  LED = Dark;                       // Turn off all LEDs.
		
  //SWITCHES
	GPIO_PORTF_DIR_R &= ~0x11; 								// (c) make PF4,0 in (built-in button)
  GPIO_PORTF_PUR_R |= 0x11;                 //     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;                 // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;                //     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;                //     PF4,PF0 falling edge event
  GPIO_PORTF_ICR_R = 0x11;                  // (e) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;                  // (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00400000; // (g) bits:23-21 for PORTF, set priority to 2
  NVIC_EN0_R |= 0x40000000; // (h) enable interrupt 30 in NVIC
}