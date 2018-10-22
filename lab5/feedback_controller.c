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

int ADC_COUNT;
uint32_t ADC_DATA[4];
uint32_t COUNT;

// Timer Interrupt Service Routine
void TMR0_IRQHandler(void){
	// GPA_13 = ~GPA_13;
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

void DRVPWM_PwmIRQHandler() {
	char display[2][16];
	COUNT++;
	sprintf(display[0],"Count: %d",COUNT);
	print_lcd(3,display[0]);
	DrvPWM_ClearInt(DRVPWM_CAP3);
}

void PWM_Init()
{
	S_DRVPWM_TIME_DATA_T sPt;
	uint8_t u8Timer;
	DrvPWM_Open();
	 
	sPt.u8Mode = DRVPWM_AUTO_RELOAD_MODE;
	sPt.u8HighPulseRatio = 99;
	sPt.i32Inverter = 1;
	sPt.u8ClockSelector = DRVPWM_CLOCK_DIV_1;
	sPt.u8PreScale = 22;
	sPt.u32Duty = 10;
	sPt.u32Frequency = 0;
	u8Timer = DRVPWM_TIMER3;//DRVPWM_TIMER3;
	
	DrvPWM_SelectClockSource(u8Timer, DRVPWM_INTERNAL_22M );
	DrvPWM_SetTimerClk(u8Timer, &sPt);
	DrvPWM_SetTimerIO(u8Timer,1);
	// DrvPWM_EnableInt(u8Timer, DRVPWM_CAP_ALL_INT, DRVPWM_PwmIRQHandler);
	DrvPWM_Enable(u8Timer,1);
	//DrvPWM_Open();
	
}

int main (void) {
	uint32_t duty;
	uint32_t u32UserData;
	int32_t PWMTimerIsEn;
	char display[2][16];
	
	//NVIC_EnableIRQ(TMR0_IRQn); // enable NVIC
	
	//SYSCLK->CLKSEL1.TMR0_S = 7; // external 22 MHz clock
	//SYSCLK->APBCLK.TMR0_EN = 1;
	
	// timer settings
	//TIMER0->TCSR.MODE = 1; // periodic mode
	//TIMER0->TCSR.PRESCALE = 199; // prescale
	//TIMER0->TCMPR = 110000; // TMCP value
	//TIMER0->TCSR.IE = 1; //interrupt enable
	//TIMER0->TISR.TIF = 1; // clear interrupt flag
	//TIMER0->TCSR.TDR_EN = 1; // enable data register function
	//TIMER0->TCSR.CRST = 1; // reset timer
	//TIMER0->TCSR.CEN = 1; // enable timer 1
	//TIMER0->TCSR.TDR_EN = 1; // enable data register function
	//TIMER0->TCSR.CTB = 0; // Make TDR the event count
	SYSCtl_Init();
	Initial_panel();
	clr_all_panel();

	// initialize LEDs
	//GPA_13 = 1;
	//GPA_14 = 1;
	GPC_12 = 0;
	
	//DrvGPIO_Open(E_GPA, 15, E_IO_OUTPUT); // port 0
	//SYS->GPAMFP.PWM3_I2SMCLK = 0;
	//GPA_15 = 0;
	
	// Initialize PWM
	//SYSCLK->APBCLK.PWM23_EN = 1;
	//SYSCLK->CLKSEL1.PWM23_S = DRVPWM_EXT_12M;
	//DrvPWM_SelectClockSource(DRVPWM_TIMER3,DRVPWM_EXT_12M);
	
	//PWMA->PPR.CP23 = 22;			// prescaler
	//PWMA->CSR.CSR3 = 1;				// Clock Divider Channel 3
	//PWMA->PCR.CH3INV = 0; 		// Don't invert channel 3
	//PWMA->PCR.CH3MOD = 1; 		// Channel 3 auto-reload mode
	//PWMA->CMR3 = 200;					// Channel 3 comparator register
	//PWMA->CNR3 = 10000;				// Channel 3 down-counter register
	//PWMA->PIER.PWMIE3 = 0; 		// Channel 3 interrupt enable
	//PWMA->POE.PWM3 = 1;				// ENABLE output on PWM3
	//PWMA->PCR.CH3EN = 1;			// enable channel 3
	//PWMA->CCR2.CAPCH3EN = 0; 	// disable capture funtion on channel 3
	
	COUNT = 0;
	PWM_Init();
	
	// initialize output ports
	// DrvPWM_SelectClockSource(DRVPWM_TIMER1,DRVPWM_EXT_12M);
	// DrvGPIO_Open(E_GPA, 15, E_IO_OUTPUT); // port 0
	// DrvPWM_SetTimerClk()
	
	//DrvSYS_Delay(100000);
	// Initialize/setup ADC
	
	DrvADC_Open(ADC_SINGLE_END, ADC_CONTINUOUS_OP, 0x80, 7, 1);
	ADC->ADCHER.PRESEL = 0b00;
	DrvADC_EnableADCInt(ADCCallback, u32UserData);
	ADC_COUNT = 0;
	//DrvADC_StartConvert();
	DrvSYS_Delay(10);

	DrvSYS_Delay(1000000);
	
	// Check to see if timer is enabled
	
	PWMTimerIsEn = DrvPWM_IsTimerEnabled(DRVPWM_TIMER3);
	
	sprintf(display[0],"PWMT3: %d",PWMTimerIsEn);
	print_lcd(2,display[0]);
	
	while(1){
		//DrvADC_StartConvert();
	}//end while
} //end main
