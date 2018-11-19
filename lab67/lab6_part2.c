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
#include "DrvUART.h"

uint32_t COUNT;
float SLOPE;
int32_t setRpm;
float prevrpm;
int DUTY;

// Timer Interrupt Service Routine
void TMR0_IRQHandler(void){
	char display[1][40];
	float rpm;
	GPA_14 = ~GPA_14;

	//rpm = 0.1*(float)COUNT*60.0+0.9*prevrpm;
	rpm = 0.5*(float)COUNT*60.0 + 0.5*prevrpm;
	//rpm = (float)COUNT*60.0/2.0;
	if(rpm > 40000) {
		rpm = 40000;
	}
	
	sprintf(display[0],"%6d, %6d, %6d           ",(int)rpm,setRpm,DUTY);
	display[0][28] = 0x0d;
	display[0][29] = 0x0b;
	DrvUART_Write(UART_PORT0, display[0], 30);
	prevrpm = rpm;
	COUNT = 0;
	
	TIMER0->TISR.TIF = 1;
}

void TMR1_IRQHandler(void){
	GPC_12 = ~GPC_12;
	DrvADC_StartConvert();
	TIMER1->TISR.TIF = 1;
}

void RpmCounterCallback() {
	COUNT++;
};

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

void init_UART() {
	STR_UART_T sParam;
	sParam.u32BaudRate = 9600;
	sParam.u8cDataBits = DRVUART_DATABITS_8;
	sParam.u8cStopBits = DRVUART_STOPBITS_1;
	sParam.u8cParity = DRVUART_PARITY_NONE;
	sParam.u8cRxTriggerLevel = DRVUART_FIFO_1BYTES;
	
	// Setup UART
	DrvSYS_SelectIPClockSource(E_SYS_UART_CLKSRC,0);
	DrvGPIO_InitFunction(E_FUNC_UART0);
	DrvUART_Open (UART_PORT0, &sParam);
}

int main (void) {
	uint32_t duty;
	uint8_t keypad_id; // store value from Scankey
	
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
	
	SYSCtl_Init();
	Initial_panel();
	clr_all_panel();

	// initialize LEDs
	GPA_14 = 0;
	
	COUNT = 0;
	PWM_Init();
	
	GPC_12 = 1;
	GPC_13 = 1;
	GPC_14 = 1;
	GPC_15 = 1;
	
	init_UART();
	
	char text[30];
	
	sprintf(text,"Lab 6");
	text[6] = 0x0d;
	text[7] = 0x0b;

	for(int i = 0; i < 20; i++)
		DrvSYS_Delay(10000000);

	DrvUART_Write(UART_PORT0, text, 8); 
	
	sprintf(text,"Measured\tTarget\tDuty");
	text[28] = 0x0d;
	text[29] = 0x0b;
	DrvUART_Write(UART_PORT0, text, 30); 
	
	//DrvGPIO_EnableDebounce(E_GPB, 14);
	DrvGPIO_EnableEINT0(E_IO_FALLING, E_MODE_EDGE, RpmCounterCallback);
	DrvGPIO_EnableDebounce(E_GPB, 14);
	
	//SLOPE = 31.5110;
	//SLOPE = 11.1637;
	//SLOPE = 8.3180;
	SLOPE = 10.943;
	setRpm = 0;
	prevrpm = 0;
	while(1){
		keypad_id = Scankey();
		
		if(keypad_id != 0){ 
			/*
			if(keypad_id == 1) {
				PWMA->CMR3 = 0x0000; // 0.0%
				setRpm = 0;
			} else if( keypad_id == 2) {
				PWMA->CMR3 = 0x01FF; // 12.5%
				setRpm = 5000;
			} else if( keypad_id == 3) {
				PWMA->CMR3 = 0x03FF; // 25.0%
				setRpm = 10000;
			} else if( keypad_id == 4) {
				PWMA->CMR3 = 0x05FF; // 37.5%
				setRpm = 15000;
			} else if( keypad_id == 5) {
				PWMA->CMR3 = 0x07FF; // 50.0%
				setRpm = 20000;
			} else if( keypad_id == 6) {
				PWMA->CMR3 = 0x09FF; // 62.5%
				setRpm = 25000;
			} else if( keypad_id == 7) {
				PWMA->CMR3 = 0x0bFF; // 75.0%
				setRpm = 30000;
			} else if( keypad_id == 8) {
				PWMA->CMR3 = 0x0DFF; // 87.5%
				setRpm = 35000;
			} else if( keypad_id == 9){
				PWMA->CMR3 = 0x0FFF; // 100% 
				setRpm = 40000;
			} else {
			  PWMA->CMR3 = 0x0000;
				setRpm = 0;
			}
			*/
			
			if(keypad_id-1 > 0)
				setRpm = (keypad_id-1)*5000;
			else
				setRpm = 0;
			prevrpm = setRpm;
			DUTY = (int)((float)(setRpm/SLOPE));
			PWMA->CMR3 = DUTY;
			
		}
	}//end while
} //end main
