/*	Single Mode ADC
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/17/18
 *
 */
	
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "ScanKey.h"
#include "LCD_Driver.h"

uint8_t start;
uint8_t reset;
uint32_t count;

// Timer Interrupt Service Routine
void TMR0_IRQHandler(void){
	GPA_13 = ~GPA_13;
	GPA_14 = ~GPA_14;
	TIMER0->TISR.TIF = 1;
}

void my_GPA_callback() {
	char display_nums[2][15]; // 2D array to store values 1-9 in char arrays
	
	if(start == 1) {
		count++;
	} else if (reset == 1) {
		count = 0;
	}
	
	sprintf(display_nums[0],"count: %d    ",count);
	if(start == 1){
		sprintf(display_nums[1],"status: started");
	} else {
		sprintf(display_nums[1],"status: stopped");
	}
	
	clr_all_panel();
	print_lcd(0,display_nums[0]);
	print_lcd(1,display_nums[1]);

}

void stopButtonIntCallback() {
	char display_nums[15];
	start = 0;
	sprintf(display_nums,"status: stopped  ");
	print_lcd(1,display_nums);
}

int main (void) {
	
	uint32_t u32UserData;
	uint8_t key_in;
	
	NVIC_EnableIRQ(TMR0_IRQn); // enable NVIC
	
	SYSCLK->CLKSEL1.TMR0_S = 7; // external 22 MHz clock
	SYSCLK->APBCLK.TMR0_EN = 1;
	
	// timer settings
	TIMER0->TCSR.MODE = 1; // periodic mode
	TIMER0->TCSR.PRESCALE = 199; // prescale
	TIMER0->TCMPR = 110000; // TMCP value
	TIMER0->TCSR.IE = 1; //interrupt enable
	TIMER0->TISR.TIF = 1; // clear interrupt flag
	TIMER0->TCSR.TDR_EN = 1; // enable data register function
	TIMER0->TCSR.CRST = 1; // reset timer
	TIMER0->TCSR.CEN = 1; // enable timer 1
	TIMER0->TCSR.TDR_EN = 1; // enable data register function
	TIMER0->TCSR.CTB = 0; // Make TDR the event count
	
	// initialize LEDs
	GPC_12 = 1;
	GPA_13 = 1;
	GPA_14 = 0;
	
	count = 0;
	start = 0;
	reset = 0;
	Initial_panel();
	clr_all_panel();
	
	char display_nums[2][15]; // 2D array to store values 1-9 in char arrays

	sprintf(display_nums[0],"count: %d    ",count);
	if(start == 1){
		sprintf(display_nums[1],"status: started");
	} else {
		sprintf(display_nums[1],"status: stopped");
	}
	
	print_lcd(0,display_nums[0]);
	print_lcd(1,display_nums[1]);

	DrvGPIO_EnableInt(E_GPA,6,E_IO_FALLING,E_MODE_EDGE);
	DrvGPIO_SetIntCallback(my_GPA_callback,0);
	DrvGPIO_EnableDebounce(E_GPA, 6);
	DrvGPIO_EnableEINT1(E_IO_FALLING, E_MODE_EDGE, stopButtonIntCallback);
	
	while(1){
		GPC_12 = GPA_6;
		OpenKeyPad();
		key_in = Scankey();
		if(key_in != 0) {
				if(key_in == 1) {
						// Start
						start = 1;
						reset = 0;
						sprintf(display_nums[1],"status: started  ");
						print_lcd(1,display_nums[1]);
				} else if(key_in == 2) {
						reset = 1;
						start = 0;
						count = 0;
						clr_all_panel();
						sprintf(display_nums[0],"count: %d  ",count);
						print_lcd(0,display_nums[0]);
						sprintf(display_nums[1],"status: stopped  ");
						print_lcd(1,display_nums[1]);
				} else {
						reset = 0;
				}
		}

	}//end while
} //end main
