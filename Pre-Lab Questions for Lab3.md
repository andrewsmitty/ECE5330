__Andrew Smit__

__ECE 5330__

__10/1/18__

# Pre-Lab Questions for Lab3

### 1.  Assume you are using all of the LEDs (RGB and the 4 red LEDs), the 7-segment display, the LCD screen, the keypad, and the external interrupt button. Which GPIO pins are available to be used as an external input. (i.e. determine which GPIO pins are being used by the peripherals listed above, these are unavailable for other use) 

Unavailable Pins:

- RGB LEDs: GPA_12 - GPA_14
- Red LEDs: GPC_12 - GPC_15 
- 7-segment: GPE_0 - GPE_7 and GPC_4 - GPC_7
- External Interrupt Button: GPB15
- LCD: GPD_8 - GPD_11 and GPD_14
- Keypad: GPA_0 - GPA_5

Available Pins:

- GPA_6-GPA_11
- GPB_0-GPB_14
- GPC_0-GPC_3, GPC_8-GPC_11
- GPD_0-GPD_7, GPD_12-GPD_13, GPD_15
- GPE_8-GPE_15

### 2. There are 4 General Purpose I/O External interrupts (source IP is GPIO). List the Interrupt Name and which pins can be used with them (see TRM).  

- EINT0: External Signal interrupt from PB14 pin
- EINT1: External Signal interrupt from PB15 pin
- GPAB: External signal interrupt from PA[15:0]/PB[13:0] pins
- GPCDE: External interrupt from PC[15:0]/PD[15:0]/PE[15:0] pins

### 3. Which driver would be the logical source of interrupt functions for the above interrupts? List one function provided by the driver that would allow you to setup an external interrupt. (see DrvX.c) 

The `DrvGPIO.c` would be the logical source of interrupt functions for the above interrupts. The `DrvGPIO_EnableInt()` function would allow you to setup an external interrupt.

### 4. Write a fragment of code to read an external input from pin GPA_6 and assign the value to LED_0 (GPC_12).  

```c
// Directly
DrvGPIO_Open(E_GPA,6,E_IO_INPUT);
GPC_12 = GPA_6;
```

```c
// Using interrupt
void my_GPA_callback() {
    GPC_12 = GPA_6;
}

DrvGPIO_EnableInt(E_GPA,6,E_IO_BOTH_EDGES,E_MODE_EDGE);
GPIO_SetIntCallback(my_GPA_callback,none);
```

### 5. Write a fragment of c-code to use the keypad to provide the start, stop and reset buttons 

```c
uint8_t key_in;
uint8_t start;
uint8_t stop;
uint8_t reset;
OpenKeyPad();
key_in = ScanKey();
if(key_in != 0) {
    if(key_in == 1) {
        start = 1;
        stop = 0;
        reset = 0;
    } else if(key_in == 2) {
        start = 0;
        stop = 1;
        reset = 0;
    } else if(key_in == 3) {
        start = 0;
        stop = 0;
        reset = 1;
    } else {
        start = 0;
        stop = 0;
        reset = 0;
    }
}
```

### 6. Can this lab be accomplished without using interrupts? 

Yes, but you would have to pole all of your keypad (start, stop, and reset) buttons, as well as all of your other GPIO pins to check if the light path is blocked or unblocked, in a loop. This would be very inefficient, but it would work.

