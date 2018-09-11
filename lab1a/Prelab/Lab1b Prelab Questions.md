__Andrew Smit__

__ECE 5330__

__9/10/18__

# Lab1b Prelab Questions



## 1. Identify pins associated with peripherals & if pins are inputs/outputs 

#### – 7-Segment displays (4 displays), Keypad, Interrupt button, Buzzer, LEDs RGB, LEDs red (4 red LEDS) 

#### – Using schematics or technical reference manual (TRM) 

- The 7-segment displays are controlled with the row pins GPE_0 - GPE_7 and the column pins GPC_4 - GPC 7. All are outputs.
- The keypad is controlled with pins GPA0-GPA5. All of them are inouts. You will either make the rows (GPA3-GPA5) inputs and columns (GPA0-GPA2) outputs or vice versa.
- The interrupt button is controlled with pin GPB15. It is an input.
- The buzzer is controlled by pin GPB11. It is an output.
- The RGB LEDs are controlled by pins GPA12-GPA14. They are outputs.
- The Red LEDs are controlled by the pins GPC12-GPC15. They are outputs.



## 2. What prevents from displaying 2 different numbers on 7 segment displays simultaneously? 

#### – Explain 

#### – How to display numbers simultaneously (pseudo code). 

You can't write to two different 7 segment displays simultaneously because the displays share the same write pins. You have to enable the displays one at a time and write to them individually. 

Pseudocode:

```
Set enable pin for first 7-segment display
Write to first 7-segment display
Disable the enable pin for the first 7-segment
Set the enable pin for the second 7-segment display
Write to the second 7-segment display
```



## 3. Compare 7-segment display & keypad 

#### – Similarities 

They both use a row and column construct.

They both use multiplexing.

They both use GPIO pins.

#### – Difference 

They keypad is considered an input whereas the display is considered an output.

The keypad uses both input and output pins while the 7-segment display

#### – How to make keypad work (Pseudo code) 

```c
void delay(void)
{
	int j;
	for(j=0;j<1000;j++);
}

void OpenKeyPad(void)
{
	uint8_t i;
	/* Initial key pad */
	for(i=0;i<6;i++)
	DrvGPIO_Open(E_GPA, i, E_IO_QUASI);
}

uint8_t Scankey(void)
{
	uint8_t act[4]={0x3b, 0x3d, 0x3e};    
	uint8_t i,temp,pin;

	for(i=0;i<3;i++)
	{
		temp=act[i];
		for(pin=0;pin<6;pin++)
		{
			if((temp&0x01)==0x01)
				DrvGPIO_SetBit(E_GPA,pin);
			else
				DrvGPIO_ClrBit(E_GPA,pin);
			temp>>=1;
		}
		delay();
		if(DrvGPIO_GetBit(E_GPA,3)==0)
			return(i+1);
		if(DrvGPIO_GetBit(E_GPA,4)==0)
			return(i+4);
		if(DrvGPIO_GetBit(E_GPA,5)==0)
			return(i+7);
	}
		return 0;
}
```

## 4. Which folder (CMSIS, Library, or Source) 

#### – To put drivers for LCD display & 7 Segment display  

The LCD and 7 segment drivers go in the Library