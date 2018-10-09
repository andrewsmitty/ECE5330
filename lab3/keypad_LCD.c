/*	keypad_LCD.c
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/12/18
 *
 *	Lab 1b problem 4b
 *	Display keypad ID on LCD
 */
	
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "LCD_Driver.h"
#include "ScanKey.h"

int main (void) {
	
	uint8_t keypad_id; // store value from Scankey
	uint8_t prev_id; // store the previous Scankey value
	char display_nums[9][9]; // 2D array to store values 1-9 in char arrays
	
	// initialize array with numbers 1-9
	// put in spaces so that each number has its own spot on the LCD
	// "1        "
	// " 2       "
	// ...
	// "       8 "
	// "        9"
	for(int i=0;i<9;i++){
		sprintf(display_nums[i],"         ");
		sprintf(&display_nums[i][i],"%d",i+1);
	}
	
	Initial_panel();
	clr_all_panel();
	
	while(1){
		keypad_id = Scankey(); // read in keypad_id from Scankey
		// check to see if a button was pressed
		// also check to see if the button is being held
		if(keypad_id != 0 && keypad_id != prev_id){ 
			clr_all_panel();
			// write number to all 4 lines
			print_lcd(0,display_nums[keypad_id-1]);
			print_lcd(1,display_nums[keypad_id-1]);
			print_lcd(2,display_nums[keypad_id-1]);
			print_lcd(3,display_nums[keypad_id-1]);
			prev_id = keypad_id;
		}
		
	}//end while
} //end main



