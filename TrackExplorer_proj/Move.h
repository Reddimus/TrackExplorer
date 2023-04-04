// Move.h
// Runs on TM4C123
// By Kevin Martinez
/*
Move file is a list of functions dedicated to all possible movements on a 2 motored car. 
By changing directions and or turning on/ off 1 or 2 motors you get 9 possible movements.
*/
#include "Motors.h"

#define WHEEL_DIR (*((volatile unsigned long *)0x400050F0)) // PB5432 are the four direction pins for L298

// PB5432 are used for direction control on DRV8838.
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.
#define FORWARD 0x28
#define BACKWARD 0x3C
#define LEFTPIVOT 0x18
#define RIGHTPIVOT 0x24

#define L_MOTOR 0x00000001
#define R_MOTOR 0x00000008

void move_forward(void);
void move_backward(void);
void move_l_piv(void);
void move_r_piv(void);
void move_l_turn(void);
void move_r_turn(void);
void move_l_back_turn(void);
void move_r_back_turn(void);
void move_stop(void);
