#ifndef LEDSW_H
#define LEDSW_H

#define LED (*((volatile unsigned long *)0x40025038))  // use onboard three LEDs: PF321

// Color    LED(s) PortF        LED(s)
#define Dark    	0x00    //  ---
#define Red     	0x02    //  R--
#define Blue    	0x04    //  --B
#define Green   	0x08    //  -G-
#define Yellow  	0x0A    //  RG-
#define Cran        0x0C    //  -GB
#define White   	0x0E    //  RGB
#define Purple  	0x06    //  R-B

#define SW1 0x10 //left switch
#define SW2 0x01 //right switch

void LEDSW_Init(void);

#endif