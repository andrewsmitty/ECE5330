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
#include "DrvI2S.h"

int ADC_COUNT;
uint32_t ADC_DATA[4];
uint32_t COUNT;
uint32_t TXData;

void I2S_Callback_Rx_Fifo(uint32_t din)
{
	GPC_12 = ~GPC_12;
	uint32_t u32data;
	uint32_t u32dataOut;
	uint16_t u16Left;
	uint16_t u16Right;
	uint8_t u8RxLevel;
	//u32data = I2S->I2SRXFIFO;
	u32data = _DRVI2S_READ_RX_FIFO();
	//u32data = din;
	u16Left = (u32data & 0xFFFF0000) >> 16;
	u16Right = (u32data & 0x0000FFFF);
	u8RxLevel = I2S->I2SSTATUS.RX_LEVEL;
	
	
	
	char display[4][16];
	sprintf(display[0],"FIFO: 0x%x      ",u32data);
	sprintf(display[1],"RxL: 0x%x       ",u16Left);
	sprintf(display[2],"RxR: 0x%x       ",u16Right);
	sprintf(display[3],"Lev: %d       ",u8RxLevel);
	print_lcd(0,display[0]);
	print_lcd(1,display[1]);
	print_lcd(2,display[2]);
	print_lcd(3,display[3]);
	
	/*
	if(COUNT == 200){
		COUNT = 0;
		u32dataOut = 0xFFFFFFFF;
	} else {
		u32dataOut = 0x00000000;
		COUNT++;
	}
	*/
	u32dataOut++;
	
	_DRVI2S_WRITE_TX_FIFO(u32dataOut);
	DrvI2S_ClearRxFIFO();
	DrvI2S_ClearTxFIFO();
	//I2S->I2SCON.CLR_RXFIFO = 1;
	//I2S->I2SCON.CLR_TXFIFO = 1;
	DrvI2S_EnableRx ( );
}

void I2S_Init(uint32_t fs, uint8_t wordWidth, uint8_t audioFormat) 
{
	/*---------------------------------------------------------------------------------------------------------*/
	/* Function: DrvI2S_Open                                                                                   */
	/*                                                                                                         */
	/* Parameters:                                                                                             */
	/*					fs															Audio Codec Sample Rate (in Hz)																 */
	/*          WordWidth		                    Data length - DRVI2S_DATABIT_8  :  8-bit Data                  */
	/*                                                        DRVI2S_DATABIT_16 : 16-bit Data                  */
	/*                                                        DRVI2S_DATABIT_24 : 24-bit Data                  */
	/*                                                        DRVI2S_DATABIT_32 : 32-bit Data                  */			
	/*          u8AudioFormat                   Audio format- DRVI2S_MONO: Mono / DRVI2S_STEREO: Stereo        */	
	/*          S_DRVI2S_DATA_T *sParam - [in]  Data structure to configure                                    */
	/*                                          The setting takes effect when I2S operates as master mode      */
	/* Returns:                                                                                                */
	/*               0      Success                                                                            */
	/*                                                                                                         */
	/* Description:                                                                                            */
	/*          This function is used to initialize I2S and the audio codec																		 */       
	/*---------------------------------------------------------------------------------------------------------*/
	S_DRVI2S_DATA_T st;
	
	st.u32SampleRate = fs;
	st.u8WordWidth = wordWidth;
	st.u8AudioFormat = audioFormat;
	st.u8DataFormat = DRVI2S_FORMAT_MSB;
	st.u8Mode = DRVI2S_MODE_MASTER;
	/* Tx FIFO threshold level is 0 word data */
	st.u8TxFIFOThreshold = DRVI2S_FIFO_LEVEL_WORD_1;
	/* Rx FIFO threshold level is 8 word data 8*/
	st.u8RxFIFOThreshold = DRVI2S_FIFO_LEVEL_WORD_8;

	//DrvI2S_EnableInt(I2S_TX_RIGHT_ZERO_CROSS,I2S_Callback_Right);
	//DrvI2S_EnableInt(I2S_TX_LEFT_ZERO_CROSS,I2S_Callback_Left);
	//DrvSYS_SelectIPClockSource(E_SYS_I2S_CLKSRC,0x02);
	DrvI2S_Open(&st);
	DrvGPIO_InitFunction(E_FUNC_I2S);
	//DrvI2S_SelectClockSource(DRVI2S_HCLK); /* I2S clock source from external 12M */
	/* Enable I2S Rx DMA function */
	//DrvI2S_EnableRxDMA();
	DrvI2S_DisableRxDMA();
	DrvI2S_DisableTxDMA();
	/* Enable I2S Rx function */
	DrvI2S_EnableRx();
	DrvI2S_EnableTx();
	/* Enable I2S and configure its settings */
	DrvI2S_EnableInt(I2S_RX_FIFO_THRESHOLD,I2S_Callback_Rx_Fifo);
	DrvI2S_ClearRxFIFO();
	DrvI2S_ClearTxFIFO();
	
}

// Timer Interrupt Service Routine
void TMR0_IRQHandler(void){
	// GPA_13 = ~GPA_13;
	//uint32_t u32data;
	//char display[3][16];
	//u32data = _DRVI2S_READ_RX_FIFO();
	//u32data = _DRVI2S_READ_RX_FIFO_LEVEL();
	//u32data = I2S->I2SRXFIFO;
	//sprintf(display[0],"Rx: %d",u32data);
	//print_lcd(3,display[0]);
	
	GPA_14 = ~GPA_14;
	TIMER0->TISR.TIF = 1;
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



int main (void) {
	char display[4][16];

	SYSCtl_Init();
	
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
	GPA_14 = 1;
	GPC_12 = 1;
	GPC_13 = 1;
	GPC_14 = 1;
	GPC_15 = 1;
	
	I2S_Init(1,DRVI2S_DATABIT_8,DRVI2S_STEREO);
	
	uint32_t u32clock;
	u32clock = DrvI2S_GetMCLKFreq ( ); /* Get I2S MCLK clock frequency */
	uint32_t u32sclock;
	u32sclock = DrvI2S_GetSourceClockFreq( ); /* Get I2S source clock frequency */
	uint32_t u32bclock;
	u32bclock = DrvI2S_GetBCLKFreq ( ); /* Get I2S BCLK clock frequency */

	sprintf(display[0],"Clk: %d",u32clock);
	sprintf(display[1],"sClk: %d",u32sclock);
	sprintf(display[2],"bClk: %d",u32bclock);
	print_lcd(0,display[0]);
	print_lcd(1,display[1]);
	print_lcd(2,display[2]);
	
	TXData = 0;
	
	for(int i = 0; i<10; i++)
		DrvSYS_Delay(300000);
		
	I2S->I2SCON.I2SEN = 1;
	
	while(1){
		//GPC_15 = GPA_8; // SDA
		//GPC_14 = GPA_9; // SCK
		//GPC_13 = GPA_15; // I2S_MCLK
		//GPC_15 = GPC_2; // ADC
		//GPC_14 = GPC_3; // DAC
		//GPC_13 = GPA_15; // I2S_MCLK
		//GPC_13 = GPC_1; // BCLK
		//GPC_15 = !I2S->I2SSTATUS.RXEMPTY;
		//GPC_14 = !I2S->I2SSTATUS.RXFULL;
		//GPC_13 = !I2S->I2SSTATUS.RXUDF;
		//GPC_13 = !I2S->I2SSTATUS.I2SRXINT;
		//GPC_14 = !I2S->I2SSTATUS.I2STXINT;
		//GPC_15 = !I2S->I2SSTATUS.I2SINT;
		//if(I2S->I2SSTATUS.RXFULL)
		//	I2S->I2SCON.CLR_RXFIFO = 1;
		//DrvADC_StartConvert();
	}//end while
} //end main
