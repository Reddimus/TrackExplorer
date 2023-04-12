// Motors.h
// Runs on TM4C123
// By Kevin Martinez

// configure the system to get its clock from the PLL
#include "PLL.h"

// bus frequency is 400MHz/(SYSDIV x + 1) = y MHz
#define TOTAL_PERIOD (BUS_MHZ_FREQ * 1000)

void Motors_Init(void);
void PWM_Duty(unsigned long duty_L, unsigned long duty_R);
void Car_Dir_Init(void); // Constant definitions based on the following hardware interface:
