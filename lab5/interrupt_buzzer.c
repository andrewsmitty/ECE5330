/*	interrupt_buzzer.c
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/12/18
 *
 *	Lab 1b problem 3
 *	Use the interrupt button to sound the buzzer
 */
	
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"


int main (void) {

	while(1){
		GPB_11 = GPB_15; // tie speaker to interrupt button
	}//end while
} //end main



