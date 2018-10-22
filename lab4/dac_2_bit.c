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
uint8_t OUTPUT_COUNT;
int UPDOWN;
uint32_t ADC_DATA[4];

// Timer Interrupt Service Routine
void TMR0_IRQHandler(void){
	GPA_13 = ~GPA_13;
	GPA_14 = ~GPA_14;
	TIMER0->TISR.TIF = 1;
}

void TMR1_IRQHandler(void){
	int port_0, port_1, port_2, port_3;
	char display[2][16];
	port_0 = (OUTPUT_COUNT & 0b00000001);
	port_1 = (OUTPUT_COUNT & 0b00000010) >> 1;
	port_2 = (OUTPUT_COUNT & 0b00000100) >> 2;
	port_3 = (OUTPUT_COUNT & 0b00001000) >> 3;
	sprintf(display[0],"Count: %d    ",OUTPUT_COUNT);
	sprintf(display[1],"Ports: 0b%d%d%d%d   ",port_3,port_2,port_1,port_0);
	GPA_0 = port_0;
	GPA_1 = port_1;
	GPA_2 = port_2;
	GPA_3 = port_3;
	GPC_12 = ~GPC_12;
	
	print_lcd(2,display[0]);
	print_lcd(3,display[1]);
	
	if(OUTPUT_COUNT >= 3 || OUTPUT_COUNT == 0){
		UPDOWN = ~UPDOWN;
	} 
	if(UPDOWN == 1) {
		OUTPUT_COUNT++;
	} else {
		OUTPUT_COUNT--;
	}
	TIMER1->TISR.TIF = 1;
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
		char display[2][16];
		// sprintf(display[0],"ADC data!   ");
		sprintf(display[0],"hex:   0x%x",sum);
		sprintf(display[1],"V:     %1.4fV   ",ADC_DATA_FLOAT);
		print_lcd(0,display[0]);
		print_lcd(1,display[1]);
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
	NVIC_EnableIRQ(TMR1_IRQn); // enable NVIC
	
	SYSCLK->CLKSEL1.TMR0_S = 7; // external 22 MHz clock
	SYSCLK->CLKSEL1.TMR1_S = 7; // external 22 MHz clock
	SYSCLK->APBCLK.TMR0_EN = 1;
	SYSCLK->APBCLK.TMR1_EN = 1;
	
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
	
	TIMER1->TCSR.MODE = 1; // periodic mode
	TIMER1->TCSR.PRESCALE = 250; // prescale
	TIMER1->TCMPR = 220000; // TMCP value
	TIMER1->TCSR.IE = 1; //interrupt enable
	TIMER1->TISR.TIF = 1; // clear interrupt flag
	TIMER1->TCSR.TDR_EN = 1; // enable data register function
	TIMER1->TCSR.CRST = 1; // reset timer
	TIMER1->TCSR.CEN = 1; // enable timer 1
	TIMER1->TCSR.TDR_EN = 1; // enable data register function
	TIMER1->TCSR.CTB = 0; // Make TDR the event count

	Initial_panel();
	clr_all_panel();

	// initialize LEDs
	GPA_13 = 1;
	GPA_14 = 0;
	GPC_12 = 0;
	
	// initialize output ports
	DrvGPIO_Open(E_GPA, 0, E_IO_OUTPUT); // port 0
	DrvGPIO_Open(E_GPA, 1, E_IO_OUTPUT); // port 1
	UPDOWN = 1;
	OUTPUT_COUNT = 1;
	
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
