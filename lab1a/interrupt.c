#include <stdio.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "DrvTIMER.h"

void TMR1_IRQHandler(void){
	GPA_13 = ~GPA_13;
	TIMER1->TISR.TIF = 1;
}