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
#include "DrvPWM.h"
#include "ScanKey.h"

int ADC_COUNT;
uint32_t NO_LED;
uint32_t FULL_LED;
uint32_t DISPLAY_COUNT;
uint16_t TARGET;
uint16_t PWM_OUT;
uint16_t PWM;
float PWM_OUT_PERCENT;
uint32_t ADC_DATA[4];
uint32_t COUNT;
int INIT_DONE;

// Timer Interrupt Service Routine
void TMR0_IRQHandler(void){
	// GPA_13 = ~GPA_13;
	GPA_14 = ~GPA_14;
	TIMER0->TISR.TIF = 1;
}

void TMR1_IRQHandler(void){
	GPC_12 = ~GPC_12;
	DrvADC_StartConvert();
	TIMER1->TISR.TIF = 1;
}


void ADCCallback(uint32_t u32UserData) {
	ADC_DATA[ADC_COUNT] = DrvADC_GetConversionData(6);
	float TARGET_PERCENT = ((float)TARGET-(float)NO_LED)/((float)FULL_LED-(float)NO_LED)*100.0;
	float ADC_DATA_PERCENT;
	float ERROR_PERCENT;
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
		// sprintf(display[0],"ADC data!   ");
		// sprintf(display[0],"hex:   0x%x",sum);
		ADC_DATA_PERCENT = ((float)sum-(float)NO_LED)/((float)FULL_LED-(float)NO_LED)*100.0;
		ERROR_PERCENT = TARGET_PERCENT - ADC_DATA_PERCENT;
		
		if(ERROR_PERCENT > 100.0){
			ERROR_PERCENT = 100.0;
		} else if(ERROR_PERCENT < -100.0) {
			ERROR_PERCENT = -100.0;
		}
		
		PWM_OUT_PERCENT = (PWM_OUT_PERCENT + ERROR_PERCENT);
		
		if(PWM_OUT_PERCENT > 100.0){
			PWM_OUT_PERCENT = 100.0;
		} else if(PWM_OUT_PERCENT < 0) {
			PWM_OUT_PERCENT = 0.0;
		}
		
		PWM_OUT = (int)(PWM_OUT_PERCENT/100.0*4095);
		//PWMA->CMR3 = PWM_OUT;//duty
		//float PWM = ((float)TARGET-(float)NO_LED)/(FULL_LED-NO_LED)*4095;
		if(INIT_DONE == 1)
			PWMA->CMR3 = PWM_OUT;
		
		sprintf(display[0], "LIGHT: %d%%    ",PWM*100/4095);
		//sprintf(display[0], "ADCIN: %d    ",sum);
		//sprintf(display[1], "NOLED: %d     ",NO_LED);
		//sprintf(display[2], "FULLLED: %d    ",FULL_LED);
		//sprintf(display[1], "V: %1.5f     ",ADC_DATA_FLOAT);
		sprintf(display[1],"ADC: %3.2f%%   ",ADC_DATA_PERCENT);
		//sprintf(display[2],"ERR: %3.2f   ",ERROR_PERCENT);
		sprintf(display[2],"PWM: %d   ",PWM_OUT);
		sprintf(display[3],"DUTY: %3.2f%%   ",PWM_OUT_PERCENT);
		if(DISPLAY_COUNT > 600){
			DISPLAY_COUNT = 0;
			print_lcd(0,display[0]);
			print_lcd(1,display[1]);
			print_lcd(2,display[2]);
			print_lcd(3,display[3]);
		}
		
		ADC_COUNT = 0;
	} else {
		ADC_DATA[ADC_COUNT] = DrvADC_GetConversionData(6);
		ADC_COUNT++;
		DISPLAY_COUNT++;
	}
	_DRVADC_CLEAR_ADC_INT_FLAG();
}

void SYSCtl_Init()
{
	/* Unlock protected registers */
	DrvSYS_UnlockProtectedReg();

	/* Select the osc clock source */
	DrvSYS_SetOscCtrl(E_SYS_XTL12M, 1);

	/* Select the HCLK source */
	DrvSYS_SelectHCLKSource(0);

	/* Set the HCLK clock divider */
	DrvSYS_SetClockDivider(E_SYS_HCLK_DIV, 0);

	/* Select the clock source */
	DrvSYS_SelectSysTickSource(0);

	DrvSYS_LockProtectedReg();
}

void PWM_Init()
{
	DrvPWM_Open();
  DrvPWM_SelectClockSource(DRVPWM_TIMER3,DRVPWM_INTERNAL_22M);
  DrvGPIO_InitFunction(E_FUNC_PWM3);
  PWMA->PCR.CH3EN = 0;//timer off
  PWMA->PPR.CP23 = 1;//prescale divider is 1
  PWMA->CSR.CSR3 = 0;//input divided by two (p242) (11Mhz)
  PWMA->PCR.CH3INV = 0;//inverter disabled
  PWMA->PCR.CH3MOD = 1;//auto-reload
  PWMA->PPR.DZI23 = 0x00;//no deadzone generator
  PWMA->CMR3 = 0x0000;//duty
  PWMA->CNR3 = 0x0FFF;//period
  PWMA->POE.PWM3 = 1;//GPA_15 pin is PWMA
  PWMA->PCR.CH3EN = 1;//timer enabledu
}

void led_init() {
	INIT_DONE = 0;
	PWMA->CMR3 = 0x0000;
	for(int i = 0; i < 20; i++)
		DrvSYS_Delay(10000000);
	NO_LED = ADC_DATA[0];
	PWMA->CMR3 = 0x0FFF;
	for(int i = 0; i < 20; i++)
		DrvSYS_Delay(10000000);
	FULL_LED = ADC_DATA[0];
	for(int i = 0; i < 20; i++)
		DrvSYS_Delay(10000000);
	INIT_DONE = 1;
}

int main (void) {
	uint32_t duty;
	uint32_t u32UserData;
	int32_t PWMTimerIsEn;
	uint8_t keypad_id; // store value from Scankey
	char display[2][16];
	
	NVIC_EnableIRQ(TMR0_IRQn); // enable NVIC
	//NVIC_EnableIRQ(TMR1_IRQn); // enable NVIC
	
	SYSCLK->CLKSEL1.TMR0_S = 7; // external 22 MHz clock
	SYSCLK->APBCLK.TMR0_EN = 1;
	//SYSCLK->CLKSEL1.TMR1_S = 7; // external 22 MHz clock
	//SYSCLK->APBCLK.TMR1_EN = 1;
	
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
	
	//TIMER1->TCSR.MODE = 1; // periodic mode
	//TIMER1->TCSR.PRESCALE = 50; // prescale
	//TIMER1->TCMPR = 100000; // TMCP value
	//TIMER1->TCSR.IE = 1; //interrupt enable
	//TIMER1->TISR.TIF = 1; // clear interrupt flag
	//TIMER1->TCSR.TDR_EN = 1; // enable data register function
	//TIMER1->TCSR.CRST = 1; // reset timer
	//TIMER1->TCSR.CEN = 1; // enable timer 1
	//TIMER1->TCSR.TDR_EN = 1; // enable data register function
	//TIMER1->TCSR.CTB = 0; // Make TDR the event count
	
	SYSCtl_Init();
	Initial_panel();
	clr_all_panel();

	// initialize LEDs
	//GPA_13 = 1;
	//GPA_14 = 1;
	GPA_14 = 0;
	
	COUNT = 0;
	DISPLAY_COUNT = 0;
	PWM_Init();
	
	// DrvSYS_Delay(100000);
	// Initialize/setup ADC
	
	DrvADC_Open(ADC_SINGLE_END, ADC_CONTINUOUS_OP, 0x40, 7, 255);
	//ADC->ADCHER.PRESEL = 0b00;
	DrvADC_EnableADCInt(ADCCallback, u32UserData);
	ADC_COUNT = 0;
	DrvADC_StartConvert();
	
	PWM_OUT = 0x0000;
	PWM_OUT_PERCENT = 0;
	led_init();
	
	// Check to see if timer is enabled
	
	//PWMTimerIsEn = DrvPWM_IsTimerEnabled(DRVPWM_TIMER3);
	
	//sprintf(display[0],"PWMT3: %d",PWMTimerIsEn);
	//print_lcd(2,display[0]);
	

	
	while(1){
		keypad_id = Scankey();
		if(keypad_id != 0){ 
			if(keypad_id == 1) {
				TARGET = 0x0FFF;
				PWM = 0x0FFF;
			} else if( keypad_id == 2) {
				TARGET = NO_LED +((FULL_LED - NO_LED)*3/4);
				PWM = 0x0bFF;
			} else if( keypad_id == 3) {
				TARGET = NO_LED +((FULL_LED - NO_LED)*1/2);//duty
				PWM = 0x07FF;
			} else if( keypad_id == 4) {
				TARGET = NO_LED +((FULL_LED - NO_LED)*1/4);
				PWM = 0x03FF;
			} else if( keypad_id == 5) {
				TARGET = NO_LED +((FULL_LED - NO_LED)*0);
				PWM = 0x0000;
			} else {
				TARGET = 0x0000;
			} 
			PWM_OUT_PERCENT = 0;
		}
		
		//DrvADC_StartConvert();
	}//end while
} //end main
