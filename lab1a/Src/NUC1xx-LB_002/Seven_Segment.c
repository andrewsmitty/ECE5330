#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSYS.h"
#include "Seven_Segment.h"

#define SEG_N0   0x82 
#define SEG_N1   0xEE 
#define SEG_N2   0x07 
#define SEG_N3   0x46 
#define SEG_N4   0x6A  
#define SEG_N5   0x52 
#define SEG_N6   0x12 
#define SEG_N7   0xE6 
#define SEG_N8   0x02 
#define SEG_N9   0x62
#define SEG_N10	 0x22
#define SEG_N11  0x1A
#define SEG_N12  0x93
#define SEG_N13  0x0E
#define SEG_N14  0x13
#define SEG_N15  0x33

uint8_t SEG_BUF[16]={SEG_N0, SEG_N1, SEG_N2, SEG_N3, SEG_N4, SEG_N5, SEG_N6, SEG_N7, SEG_N8, SEG_N9, SEG_N10, SEG_N11, SEG_N12, SEG_N13, SEG_N14, SEG_N15}; 
void OpenSevenSegment(void)
{
	DrvGPIO_Open(E_GPC, 4, E_IO_OUTPUT);
	DrvGPIO_Open(E_GPC, 5, E_IO_OUTPUT);
	DrvGPIO_Open(E_GPC, 6, E_IO_OUTPUT);
	DrvGPIO_Open(E_GPC, 7, E_IO_OUTPUT);
//	GPIO_SetMode(PC, BIT5, GPIO_PMD_OUTPUT);
//	GPIO_SetMode(PC, BIT6, GPIO_PMD_OUTPUT);
//	GPIO_SetMode(PC, BIT7, GPIO_PMD_OUTPUT);
	GPC_4=0;
	GPC_5=0;
	GPC_6=0;
	GPC_7=0;
	DrvGPIO_Open(E_GPE, 0, E_IO_QUASI);
	DrvGPIO_Open(E_GPE, 1, E_IO_QUASI);
	DrvGPIO_Open(E_GPE, 2, E_IO_QUASI);
	DrvGPIO_Open(E_GPE, 3, E_IO_QUASI);
	DrvGPIO_Open(E_GPE, 4, E_IO_QUASI);
	DrvGPIO_Open(E_GPE, 5, E_IO_QUASI);
	DrvGPIO_Open(E_GPE, 6, E_IO_QUASI);
	DrvGPIO_Open(E_GPE, 7, E_IO_QUASI);
	DrvGPIO_Open(E_GPE, 8, E_IO_QUASI);
	DrvGPIO_Open(E_GPE, 9, E_IO_QUASI);
//	GPIO_SetMode(PE, BIT0, GPIO_PMD_QUASI);
//	GPIO_SetMode(PE, BIT1, GPIO_PMD_QUASI);
//	GPIO_SetMode(PE, BIT2, GPIO_PMD_QUASI);
//	GPIO_SetMode(PE, BIT3, GPIO_PMD_QUASI);	
//	GPIO_SetMode(PE, BIT4, GPIO_PMD_QUASI);
//	GPIO_SetMode(PE, BIT5, GPIO_PMD_QUASI);
//	GPIO_SetMode(PE, BIT6, GPIO_PMD_QUASI);
//	GPIO_SetMode(PE, BIT7, GPIO_PMD_QUASI);
  GPE_0=0;
  GPE_1=0;
  GPE_2=0;
	GPE_3=0;
	GPE_4=0;
	GPE_5=0;
	GPE_6=0;
	GPE_7=0;
}

void ShowSevenSegment(uint8_t no, uint8_t number)
{
  uint8_t temp,i;
	temp=SEG_BUF[number];
	
	for(i=0;i<8;i++)
	    {
		if((temp&0x01)==0x01)		   		   
			 switch(i) {
				 case 0: GPE_0=1; break;
				 case 1: GPE_1=1; break;
				 case 2: GPE_2=1; break;
				 case 3: GPE_3=1; break;
				 case 4: GPE_4=1; break;
				 case 5: GPE_5=1; break;
				 case 6: GPE_6=1; break;
				 case 7: GPE_7=1; break;
			   }		
		   else
				 switch(i) {
				 case 0: GPE_0=0; break;
				 case 1: GPE_1=0; break;
				 case 2: GPE_2=0; break;
				 case 3: GPE_3=0; break;
				 case 4: GPE_4=0; break;
				 case 5: GPE_5=0; break;
				 case 6: GPE_6=0; break;
				 case 7: GPE_7=0; break;
				 }	  
		   temp=temp>>1;
		}
			switch(no) {
				case 0: GPC_4=1; break;
				case 1: GPC_5=1; break;
				case 2: GPC_6=1; break;
				case 3: GPC_7=1; break;
			}
}

void CloseSevenSegment(void)
{
	GPC_4=0;	
	GPC_5=0;	
	GPC_6=0;	
	GPC_7=0;
}

//#define SEG_N0   0x82 
//#define SEG_N1   0xEE 
//#define SEG_N2   0x07 
//#define SEG_N3   0x46 
//#define SEG_N4   0x6A  
//#define SEG_N5   0x52 
//#define SEG_N6   0x12 
//#define SEG_N7   0xE6 
//#define SEG_N8   0x02 
//#define SEG_N9   0x62  

//unsigned char SEG_BUF[10]={SEG_N0, SEG_N1, SEG_N2, SEG_N3, SEG_N4, SEG_N5, SEG_N6, SEG_N7, SEG_N8, SEG_N9}; 

//void show_seven_segment(unsigned char no, unsigned char number)
//{
//    unsigned char temp,i;
//	temp=SEG_BUF[number];
//	
//	for(i=0;i<8;i++)
//	    {
//		if((temp&0x01)==0x01)		   		   
//		   DrvGPIO_SetBit(E_GPE,i);
//		   else
//		   DrvGPIO_ClrBit(E_GPE,i);		  
//		   temp=temp>>1;
//		}
//		DrvGPIO_SetBit(E_GPC,4+no);	

//}

//void close_seven_segment(void)
//{
//	DrvGPIO_ClrBit(E_GPC,4);	
//    DrvGPIO_ClrBit(E_GPC,5);	
//	DrvGPIO_ClrBit(E_GPC,6);	
//	DrvGPIO_ClrBit(E_GPC,7);

//}
