/*	seven_seg.c
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/12/18
 *
 *	Lab 1b problem 2
 *	Display 2018 on the 7-segment display
 */
	
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "Seven_Segment.h"


int main (void) {

	int delay_cycles = 5000;		// clock cycles inbetween 7-segments
	int display[4] = {2,0,1,8};	// Array of values to be displayed
	int i;
	
	// Need to cycle between the displays quickly
	// and repeatedly
	while(1){
		for(i=0;i<4;i++){
			OpenSevenSegment();
			ShowSevenSegment(3-i,display[i]); // start at 3 and go down to 0
			DrvSYS_Delay(delay_cycles);
			CloseSevenSegment();
		}
	}//end while
} //end main



