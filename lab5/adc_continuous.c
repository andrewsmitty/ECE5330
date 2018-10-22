/*	Continuous Mode ADC
 * 	ECE 5330
 *	Written by: Andrew Smit
 *	9/17/18
 *
 */
	
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "LCD_Driver.h"
#include "DrvADC.h"

int ADC_COUNT;
uint32_t ADC_DATA[4];

// Timer Interrupt Service Routine
void TMR0_IRQHandler(void){
	GPA_13 = ~GPA_13;
	GPA_14 = ~GPA_14;
	TIMER0->TISR.TIF = 1;
}

void ADCCallback(uint32_t u32UserData) {
	ADC_DATA[ADC_COUNT] = DrvADC_GetConversionData(7);
	int count = ADC_COUNT;
	uint32_t current_data = ADC_DATA[0];
	uint8_t maxpoint;
	uint8_t minpoint;
	uint32_t sum = 0;
	int div_count = 0;
	if(ADC_COUNT == 3){
		
		// find minimum index
		for(int i=0; i<4; i++){
			if(ADC_DATA[i] < current_data) {
				current_data = ADC_DATA[i];
				minpoint = i;
			}
		}
		
		current_data = ADC_DATA[0];
		
		// find maximum index
		for(int i=0; i<4; i++){
			if(ADC_DATA[i] > current_data) {
				current_data = ADC_DATA[i];
				maxpoint = i;
			}
		}
		
		// average other two values
		for(int i=0; i<4; i++){
			if(i != minpoint && i != maxpoint) {
				sum += ADC_DATA[i];
				div_count++;
			}
		}
		
		sum = sum/div_count;
		
		double ADC_DATA_FLOAT = 3.3/4095*(sum);
		char display[4][16];
		sprintf(display[0],"ADC data!");
		sprintf(display[1],"hex:  %x",sum);
		//sprintf(display[2],"dec: %d",sum);
		sprintf(display[2],"V:    %1.4fV",ADC_DATA_FLOAT);
		sprintf(display[3],"full: %3.2f", ADC_DATA_FLOAT/3.3*100);
		clr_all_panel();
		DrvSYS_Delay(1000);
		for(int i = 0; i<4; i++){
			print_lcd(i,display[i]);
		}
		DrvSYS_Delay(100000);
		
		ADC_COUNT = 0;
	} else {
		ADC_DATA[ADC_COUNT] = DrvADC_GetConversionData(7);
		ADC_COUNT++;
	}
	_DRVADC_CLEAR_ADC_INT_FLAG();
}

int main (void) {
	
	uint32_t u32UserData;
	
	//UNLOCKREG();
	//SYSCLK->PWRCON.OSC22M_EN=1;
	//while((SYSCLK->CLKSTATUS.OSC22M_STB)!=1);
	//SYSCLK->CLKSEL0.HCLK_S=7;
	//LOCKREG();
	
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

	Initial_panel();
	clr_all_panel();

	
	// initialize LEDs
	GPA_13 = 1;
	GPA_14 = 0;
	
	//DrvSYS_Delay(100000);
	// Initialize/setup ADC
	DrvADC_Open(ADC_SINGLE_END, ADC_CONTINUOUS_OP, 0x80, 7, 1);
	ADC->ADCHER.PRESEL = 0b00;
	DrvADC_EnableADCInt(ADCCallback, u32UserData);
	ADC_COUNT = 0;
	DrvADC_StartConvert();
	//DrvSYS_Delay(100000);
	
	while(1){
		//DrvSYS_Delay(20000000); //DrvSYS.h line 182, DrvSYS.c line 1310
		//DrvADC_StartConvert();
	}//end while
} //end main
