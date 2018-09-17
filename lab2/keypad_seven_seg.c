/*	keypad_seven_seg.c
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/12/18
 *
 *	Lab 1b problem 4a
 *	Display keypad ID on 7-seg
 */
	
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "Seven_Segment.h"
#include "ScanKey.h"

int main (void) {
	
	int delay_cycles = 5000; // cycle delay
	uint8_t keypad_id; // store value from Scankey
	int display_nums[9]; // array to store values 1-9
	
	// initialize array with numbers 1-9;
	for(int i=0;i<9;i++){
		display_nums[i] = i+1;
	}
	
	while(1){
		keypad_id = Scankey();
		if(keypad_id != 0){
			OpenSevenSegment();
			ShowSevenSegment(3,display_nums[keypad_id-1]); 
			DrvSYS_Delay(delay_cycles);
			//CloseSevenSegment();
		}
		
	}//end while
} //end main



