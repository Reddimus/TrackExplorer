// Motors.c
// Runs on TM4C123
// By Kevin Martinez
#include "tm4c123gh6pm.h"
#include "Motors.h"

void Motor_0_Init(void);
void Motor_1_Init(void);

void Motors_Init(void){
	// Activate PWM Module 1 & 2
	SYSCTL_RCGCPWM_R |= 0x03;
	SYSCTL_RCC_R &= ~0x001E0000; // Clear any previous PWM divider values
	Motor_0_Init(); 	// Hardware PWM0 output for left motor (PB6)
	Motor_1_Init(); 	// Hardware PWM1 output for right motor (PA7)
}

// PB6 left motor Hardware PWM0
void Motor_0_Init(void){
	// Initialize Port B 6
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
	GPIO_PORTB_AFSEL_R |= 0x40;	// enable alt funct: PB6 for PWM
	GPIO_PORTB_PCTL_R &= ~0x0F000000; 	// PWM to be used on PB6
	GPIO_PORTB_PCTL_R |= 0x04000000; 		// PB6 = PWM0
	GPIO_PORTB_DR8R_R |= 0x40;					// 8 mA drive
	GPIO_PORTB_DEN_R |= 0x40;						// enable digital I/O
	// Initialize PWM M0_0
	PWM0_0_CTL_R = 0;	// re-loading down-counting mode
	PWM0_0_GENA_R |= 0xC8;	// low on LOAD, high on CMPA down
	PWM0_0_LOAD_R = TOTAL_PERIOD - 1;	// cycles needed to count down to 0
	PWM0_0_CMPA_R = 0;	// count value when output rises
	
	PWM0_0_CTL_R |= 0x00000001;	// Enable PWM0 Generator 0 in Countdown mode
	PWM0_ENABLE_R &= ~0x00000001;	// Disable PB7:PWM0 output 1 on initialization
}

// PA7 right motor Hardware PWM1
void Motor_1_Init(void){
	// Initialize Port A 7
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;	// Activate A clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)==0){};
	}
	GPIO_PORTA_AFSEL_R |= 0x80;	// enable alt funct: PA7 for PWM
	GPIO_PORTA_PCTL_R &= ~0xF0000000; 	// PWM to be used on PA7
	GPIO_PORTA_PCTL_R |= 0x50000000; 		// PA7 = PWM1
	GPIO_PORTA_DR8R_R |= 0x80;					// 8 mA drive
	GPIO_PORTA_DEN_R |= 0x80;						// enable digital I/O
	// Initialize PWM M1_1
	PWM1_1_CTL_R = 0;	// re-loading down-counting mode
	PWM1_1_GENB_R |= 0xC08;// low on LOAD, high on CMPB down
	PWM1_1_LOAD_R = TOTAL_PERIOD - 1;	// cycles needed to count down to 0
	PWM1_1_CMPB_R = 0;	// count value when output rises
	
	PWM1_1_CTL_R |= 0x00000001;	// Enable PWM1 Generator 1 in Countdown mode
	PWM1_ENABLE_R &= ~0x00000008;	// Disable PWM1 output 3 for PA 7
}

// Dependency: Motors_Init() 	// initialize PWM1&0
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB76 by changing the CMP registers
void PWM_Duty(unsigned long duty_L, unsigned long duty_R){
	PWM0_0_CMPA_R = duty_L - 1;	// PB6 count value when output rises
	PWM1_1_CMPB_R = duty_R - 1;	// PA7 count value when output rises
}


// PB45 = Direction pins for left motor
// PB23 = Direction pins for right motor
// Initializes PB5432 for use with DRV8838 motor driver direction
void Car_Dir_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
		
	GPIO_PORTB_AMSEL_R &= ~0x3C; // disable analog function
	GPIO_PORTB_AFSEL_R &= ~0x3C; // no alternate function
  	GPIO_PORTB_PCTL_R &= ~0x00FFFF00;	// GPIO clear bit PCTL 
	GPIO_PORTB_DIR_R |= 0x3C; // output on pin(s)
  	GPIO_PORTB_DEN_R |= 0x3C;	// enable digital I/O on pin(s)
}
