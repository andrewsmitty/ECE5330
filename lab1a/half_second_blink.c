/*	half_second_blink
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/12/18
 *
 *	make the LEDs blink with a frequency of 1/2 s
 *  using a timer/interrupts
 */
	
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "DrvTIMER.h"

// Timer Interrupt Service Routine
//void TMR0_IRQHandler(void){
//	GPA_13 = ~GPA_13;
//	TIMER1->TISR.TIF = 1;
//}

void TMR0_Callback(void) {
	GPA_13 = ~GPA_13;
	DrvTIMER_ClearIntFlag(E_TMR0);
}


int main (void) {
	//unlock the protected registers
	//UNLOCKREG();
	//select the 22MHz RC clock
	//DrvSYS_SetOscCtrl(E_SYS_OSC22M,1);
	//wait until the clock is stable
	//while(DrvSYS_GetChipClockSourceStatus(E_SYS_OSC22M) != 1);
	//HCLK clock source 7: internal 22MHz RC oscillator
	//DrvSYS_SelectHCLKSource(0);
	//Lock the protected registers
	//LOCKREG();
	
	//DrvTIMER_Init();
	//clock frequency is HCLK clock source/(HCLK_N +1)
	//Max divisor value 0xF; Min value is 0x0 - (hence the "+1" above)
	//DrvSYS_SetClockDivider(E_SYS_HCLK_DIV,2); //slower clock, 0 is very fast.
	
	NVIC_EnableIRQ(TMR0_IRQn); // enable NVIC
	
	//SYSCLK->CLKSEL1.TMR0_S = 0; // external 12 MHz clock
	//SYSCLK->APBCLK.TMR0_EN = 1;
	
	// timer setting0
	//TIMER0->TCSR.MODE = 1; // periodic mode
	//TIMER0->TCSR.PRESCALE = 120; // prescale
	//TIMER0->TCMPR = 49587; // TMCP value
	//TIMER0->TCSR.IE = 1; //interrupt enable
	//TIMER0->TISR.TIF = 1; // clear interrupt flag
	//TIMER0->TCSR.TDR_EN = 1; // enable data register function
	//TIMER0->TCSR.CRST = 1; // reset timer
	//TIMER0->TCSR.CEN = 1; // enable timer 1
	//TIMER0->TCSR.TDR_EN = 1; // enable data register function
	//TIMER0->TCSR.CTB = 0; // Make TDR the event count
	
	DrvSYS_SelectIPClockSource(E_SYS_TMR0_CLKSRC,0);
	DrvTIMER_Init();
	DrvTIMER_Open(E_TMR0,2,E_PERIODIC_MODE);
	DrvTIMER_SetTimerEvent(E_TMR0,1,(TIMER_CALLBACK)TMR0_Callback,1);
	DrvTIMER_EnableInt(E_TMR0);
	DrvTIMER_ClearIntFlag(E_TMR0);
	DrvTIMER_Start(E_TMR0);
	
	
	// initialize LEDs
	GPA_13 = 1;
	while(1){
		/* DrvGPIO.h gives us the option to use the pins/ports directly for this chip */
		/* see line 35 in DrvGPIO.h */
	
		
		// blue
		/*
		GPA_12 = 0;
		GPA_13 = 1;
		GPA_14 = 1;
		
		GPC_12 = 0;
		GPC_13 = 1;
		GPC_14 = 1;
		GPC_15 = 0;
		DrvSYS_Delay(10000000); //DrvSYS.h line 182, DrvSYS.c line 1310
		
		// green
		GPA_12 = 1;
		GPA_13 = 0;
		GPA_14 = 1;
		
		GPC_12 = 1;
		GPC_13 = 0;
		GPC_14 = 1;
		GPC_15 = 0;
		DrvSYS_Delay(10000000);
		
		// red
		GPA_12 = 1;
		GPA_13 = 1;
		GPA_14 = 0;
		
		GPC_12 = 1;
		GPC_13 = 1;
		GPC_14 = 0;
		GPC_15 = 0;
		DrvSYS_Delay(10000000);
		
		// blue and green
		GPA_12 = 0;
		GPA_13 = 0;
		GPA_14 = 1;
		
		GPC_12 = 1;
		GPC_13 = 0;
		GPC_14 = 1;
		GPC_15 = 0;
		DrvSYS_Delay(10000000);
		
		// blue and red
		GPA_12 = 0;
		GPA_13 = 1;
		GPA_14 = 0;
		
		GPC_12 = 0;
		GPC_13 = 1;
		GPC_14 = 1;
		GPC_15 = 0;
		DrvSYS_Delay(10000000);
		
		// green and red
		GPA_12 = 1;
		GPA_13 = 0;
		GPA_14 = 0;
		
		GPC_12 = 0;
		GPC_13 = 1;
		GPC_14 = 0;
		GPC_15 = 1;
		DrvSYS_Delay(10000000);
		
		// blue and green and red
		GPA_12 = 0;
		GPA_13 = 0;
		GPA_14 = 0;
		
		GPC_12 = 0;
		GPC_13 = 0;
		GPC_14 = 1;
		GPC_15 = 1;
		DrvSYS_Delay(10000000);	
		*/
	}//end while
} //end main



