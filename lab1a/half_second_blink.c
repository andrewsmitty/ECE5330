/*	half_second_blink
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/12/18
 *
 *	Lab 1a problem 3
 *	make the LEDs blink with a frequency of 1/2 s
 *  using a timer/interrupts
 */
	
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
//#include "DrvTIMER.h"

// Timer Interrupt Service Routine
void TMR0_IRQHandler(void){
	GPA_13 = ~GPA_13;
	GPA_14 = ~GPA_14;
	TIMER0->TISR.TIF = 1;
}


int main (void) {
	
	NVIC_EnableIRQ(TMR0_IRQn); // enable NVIC
	
	SYSCLK->CLKSEL1.TMR0_S = 7; // external 22 MHz clock
	SYSCLK->APBCLK.TMR0_EN = 1;
	
	// timer settings
	TIMER0->TCSR.MODE = 1; // periodic mode
	TIMER0->TCSR.PRESCALE = 199; // prescale
	TIMER0->TCMPR = 55000; // TMCP value
	TIMER0->TCSR.IE = 1; //interrupt enable
	TIMER0->TISR.TIF = 1; // clear interrupt flag
	TIMER0->TCSR.TDR_EN = 1; // enable data register function
	TIMER0->TCSR.CRST = 1; // reset timer
	TIMER0->TCSR.CEN = 1; // enable timer 1
	TIMER0->TCSR.TDR_EN = 1; // enable data register function
	TIMER0->TCSR.CTB = 0; // Make TDR the event count
	
	// initialize LEDs
	GPA_13 = 1;
	GPA_14 = 0;
	while(1){
	}//end while
} //end main



