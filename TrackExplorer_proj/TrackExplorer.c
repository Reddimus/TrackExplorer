// TrackExplorer.c
// Runs on TM4C123
// By Kevin Martinez

// This project uses two hardware PWM to control two DC Motors, 
// ADC to collect analog inputs from three Sharp IR sensors.
// The three Sharp analog IR distance sensors (GP2Y0A21YK0F) are used
// to allowthe robot to navigate through a track with two walls: 
// one mounted looking directly forward to avoid a head-on collision, 
// the other two looking forward to the left and to the right to detect  
// the distances between the car and the two walls. The goal is to 
// control power to each wheel so the left and right distances to the 
// walls are equal.
// If an object is detected too close to the robot, 
// the robot should be able to avoid it.
/*
    ------------------------------------------wall---------
                      /
                     /
                    / 
                   /
         -----------
         |         |
         | Robot   | ---> direction of motion and third sensor
         |         |
         -----------
                   \
                    \
                     \
                      \
    ------------------------------------------wall---------
*/
// PE1 connected to forward facing IR distance sensor
// PE4 connected to right IR distance sensor
// PE5 connected to left IR distance sensor

#include "tm4c123gh6pm.h"
#include "Sensors.h"
#include "Motors.h"
#include "LEDSW.h"
#include "PLL.h"
#include "stdint.h"
#include "Move.h"
#include "MathEqs.h"

// basic functions defined at end of startup.s
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode

// You use datasheet to calculate the following ADC values
// then test your sensors to adjust the values 
#define MAXCM 	80 	// Sharp IR sensors only measure up to 80CM
#define CRASH 	15 	// if there is less than this distance ahead of the robot, it will immediately stop

// with equal power to both motors (LeftH == RightH), the robot still may not drive 
// straight due to mechanical differences in the motors, so bias the left wheel faster 
// or slower than the constant right wheel
#define LEFTPOWER 	0.77 * TOTAL_PERIOD 	// duty cycle of left wheel 
#define RIGHTPOWER 	0.99 * TOTAL_PERIOD 	// duty cycle of right wheel
#define NOPOWER 	1

void System_Init(void);
void steering(uint16_t left_dist, uint16_t ahead_dist, uint16_t right_dist);
void GPIOPortF_Handler(void);

int main(void){
	uint16_t left, right, ahead;
	DisableInterrupts();  // disable interrupts while initializing
	System_Init();
	EnableInterrupts();   // enable after all initialization are done
	LED = Dark;
  	// Calibrate the sensors: read at least 5 times from the sensor 
	// before the car starts to move: this will allow software to filter the sensor outputs.	
	for (char x = 0; x < 10; x++){
		ReadSensorsMedianFilter(&left, &ahead, &right); 	//sensor test
	}
	// start with moving forward, LED green when SW1 is pressed
	GPIOPortF_Handler();
	while(1){
		// choose one of the following three software filter methods
		ReadSensorsMedianFilter(&left, &ahead, &right);
		//ReadSensorsIIRFilter(&ahead, &right, &left);
		//ReadSensorsFIRFilter(&left, &ahead, &right);
		uint16_t l_dist = CalcDist(left), m_dist = CalcDist(ahead), r_dist = CalcDist(right);
		steering(l_dist, m_dist, r_dist);
	}
}

void System_Init(void) {
	PLL_Init(); 		// bus clock at 16 MHz
	Sensors_Init(); 	// inits ADC to sample AIN2 (PE1), AIN9 (PE4), AIN8 (PE5)
	LEDSW_Init(); 		// configure onboard LEDs and push buttons; LEDs = PF321, Onboard Switches = PF40
	Car_Dir_Init(); 	// PB5432 for use with DRV8838 motor driver direction
	Motors_Init(); 		// Initialize signals for the two DC Motors
}

void GPIOPortF_Handler(void){
	if(GPIO_PORTF_RIS_R&SW2){  // SW2 pressed
		GPIO_PORTF_ICR_R = SW2;  // acknowledge flag0
		PWM_Duty(NOPOWER, NOPOWER);
	}
	if(GPIO_PORTF_RIS_R&SW1){  // SW1 pressed
		GPIO_PORTF_ICR_R = SW1;  // acknowledge flag4
		PWM_Duty(LEFTPOWER, RIGHTPOWER);
		LED = Dark;
		LED = Green;
		move_forward();
  }
}

void steering(uint16_t left_dist, uint16_t ahead_dist, uint16_t right_dist){
	LED = Dark;
	// if sensors dist out of range (80 cm) objective reached 
	if (MAXCM <= left_dist && MAXCM <= ahead_dist && MAXCM <= right_dist){
		LED = Blue;
		move_stop();	// open end
	}
	// elif walls are not to close; Case (111)
	else if (CRASH < left_dist && CRASH < ahead_dist && CRASH < right_dist){
		LED = Green;
		move_forward();
	}
	// elif close to wall (CRASH); Case (XXX)
	else {
		LED = Red;
		if (ahead_dist <= CRASH){
			if (right_dist > CRASH) move_r_piv(); 	// if left opening turn right in place
			else move_l_piv(); 			// if right opening turn left in place
		}
		else if (left_dist <= CRASH){
			move_r_turn();
		}
		else{
			move_l_turn();
		}
	}
}
