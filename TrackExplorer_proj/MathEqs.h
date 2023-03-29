// MathEqs.h
// Runs on TM4C123
// By Kevin Martinez
//#include <stdlib.h>
#include <stdint.h>
// IR sensor measurements
#define IR09CM            4095  // ADC output for 09cm:3.3 ->   (3.3/3.3)*4095=4095
#define IR15CM            2233  // ADC output for 15cm:1.8v ->  (1.8/3.3)*4095=2233 
#define IR20CM            1724  // ADC output for 20cm:1.39v -> (1.39/3.3)*4095=1724
#define IR30CM            1116  // ADC output for 30cm:0.9v ->  (0.9/3.3)*4095=1116
#define IR40CM            918   // ADC output for 40cm:0.74v -> (0.74/3.3)*4095=918
#define IR80CM            496   // ADC output for 80cm:0.4v ->  (0.4/3.3)*4095=496

uint16_t CalcDist(uint16_t adc_out);