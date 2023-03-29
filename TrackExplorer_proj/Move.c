// Move.c
// Runs on TM4C123
// By Kevin Martinez
#include "tm4c123gh6pm.h"
#include "Motors.h"
#include "Move.h"

// Car movements
void move_forward(void){
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= L_MOTOR;		// enable left wheel
	PWM1_ENABLE_R |= R_MOTOR;		// enable right wheel
}

void move_backward(void){
	WHEEL_DIR = BACKWARD;
	PWM0_ENABLE_R |= L_MOTOR;		// enable left wheel
	PWM1_ENABLE_R |= R_MOTOR;		// enable right wheel
}

// Turn right in place
void move_l_piv(void){
	WHEEL_DIR = LEFTPIVOT;
	PWM0_ENABLE_R |= L_MOTOR;		// enable left wheel
	PWM1_ENABLE_R |= R_MOTOR;		// enable right wheel
}

// Turn right in place
void move_r_piv(void){
	WHEEL_DIR = RIGHTPIVOT;
	PWM0_ENABLE_R |= L_MOTOR;		// enable left wheel
	PWM1_ENABLE_R |= R_MOTOR;		// enable right wheel
}

void move_l_turn(void){
	WHEEL_DIR=FORWARD;
	PWM0_ENABLE_R &= ~L_MOTOR; 	// stop left wheel
	PWM1_ENABLE_R |= R_MOTOR; 	// enable right wheel
}

void move_r_turn(void){
	WHEEL_DIR=FORWARD;
	PWM0_ENABLE_R |= L_MOTOR; 	// enable left wheel
	PWM1_ENABLE_R &= ~R_MOTOR; 	// stop right wheel
}

void move_l_back_turn(void){
	WHEEL_DIR = BACKWARD;
	PWM0_ENABLE_R &= ~L_MOTOR; 	// stop left wheel
	PWM1_ENABLE_R |= R_MOTOR; 	// enable right wheel
}

void move_r_back_turn(void){
	WHEEL_DIR=BACKWARD;
	PWM0_ENABLE_R |= L_MOTOR; 	// enable left wheel
	PWM1_ENABLE_R &= ~R_MOTOR; 	// stop right wheel
}

void move_stop(void){
	PWM0_ENABLE_R &= ~L_MOTOR; 	// stop right wheel
	PWM1_ENABLE_R &= ~R_MOTOR; 	// stop left wheel
}