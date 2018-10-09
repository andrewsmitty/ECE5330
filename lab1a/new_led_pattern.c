/*	new_led_pattern
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/10/18
 *
 *  Lab1a problem 2
 *	Create a new pattern on the LEDs, both red and RGB
 */
	
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
int main (void) {
	int32_t time=30000000;
	//unlock the protected registers
	UNLOCKREG();
	//select the 22MHz RC clock
	DrvSYS_SetOscCtrl(E_SYS_OSC22M,1);
	//wait until the clock is stable
	while(DrvSYS_GetChipClockSourceStatus(E_SYS_OSC22M) != 1);
	//HCLK clock source 7: internal 22MHz RC oscillator
	DrvSYS_SelectHCLKSource(7);
	//Lock the protected registers
	LOCKREG();
	//clock frequency is HCLK clock source/(HCLK_N +1)
	//Max divisor value 0xF; Min value is 0x0 - (hence the "+1" above)
	DrvSYS_SetClockDivider(E_SYS_HCLK_DIV,2); //slower clock, 0 is very fast.
	
	while(1){
		/* DrvGPIO.h gives us the option to use the pins/ports directly for this chip */
		/* see line 35 in DrvGPIO.h */
		
		// blue
		GPA_12 = 0;
		GPA_13 = 1;
		GPA_14 = 1;
		
		GPC_12 = 0;
		GPC_13 = 1;
		GPC_14 = 1;
		GPC_15 = 0;
		DrvSYS_Delay(time); //DrvSYS.h line 182, DrvSYS.c line 1310
		
		// green
		GPA_12 = 1;
		GPA_13 = 0;
		GPA_14 = 1;
		
		GPC_12 = 1;
		GPC_13 = 0;
		GPC_14 = 1;
		GPC_15 = 0;
		DrvSYS_Delay(time);
		
		// red
		GPA_12 = 1;
		GPA_13 = 1;
		GPA_14 = 0;
		
		GPC_12 = 1;
		GPC_13 = 1;
		GPC_14 = 0;
		GPC_15 = 0;
		DrvSYS_Delay(time);
		
		// blue and green
		GPA_12 = 0;
		GPA_13 = 0;
		GPA_14 = 1;
		
		GPC_12 = 1;
		GPC_13 = 0;
		GPC_14 = 1;
		GPC_15 = 0;
		DrvSYS_Delay(time);
		
		// blue and red
		GPA_12 = 0;
		GPA_13 = 1;
		GPA_14 = 0;
		
		GPC_12 = 0;
		GPC_13 = 1;
		GPC_14 = 1;
		GPC_15 = 0;
		DrvSYS_Delay(time);
		
		// green and red
		GPA_12 = 1;
		GPA_13 = 0;
		GPA_14 = 0;
		
		GPC_12 = 0;
		GPC_13 = 1;
		GPC_14 = 0;
		GPC_15 = 1;
		DrvSYS_Delay(time);
		
		// blue and green and red
		GPA_12 = 0;
		GPA_13 = 0;
		GPA_14 = 0;
		
		GPC_12 = 0;
		GPC_13 = 0;
		GPC_14 = 1;
		GPC_15 = 1;
		DrvSYS_Delay(time);	
	}//end while
} //end main
