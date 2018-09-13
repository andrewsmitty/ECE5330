/*	hello_world.c
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/12/18
 *
 *	Lab 1a problem 1d
 *	print hello world to the LCD
 * 
 */
	
#include <stdio.h>
#include "NUC1xx.h"
//#include "DrvSYS.h"
//#include "DrvGPIO.h"
#include "LCD_Driver.h"

// Timer Interrupt Service Routine


int main (void) {
	Initial_panel();
	clr_all_panel();
	
	char s0[15];
	char s1[15];
	char s2[15];
	char s3[15];
	sprintf(s0, "hello world    ");
	sprintf(s1, " hello world   ");
	sprintf(s2, "   hello world ");
	sprintf(s3, "    hello world");
	print_lcd(0,s0);
	print_lcd(1,s1);
	print_lcd(2,s2);
	print_lcd(3,s3);
	
	
	// initialize LEDs
	while(1){

	}//end while
} //end main



