__Andrew Smit__

__ECE 5330__

__9/10/18__

# Lab1a Prelab Questions



## 1. Nu-LB-NUC140 board has 5 clock options 

### – Provide name, frequency, internal/external 



·         32.768 kHz external low speed crystal

·         4~24 kHz external high speed crystal

·         Programmable PLL FOUT with source that consists of external 4~24 MHz high speed crystal and internal 22.1184 MHz high speed oscillator

·         22.1184 MHz internal high speed oscillator

·         10 kHz internal low speed oscillator

 

## 2. How many independent sets of timers supported 

### – List Timer operation modes (4) on Nu-LB-NUC140

 

·         There are 4 sets of 32-bit timers with 24-bit up-timer and 8-bit pre-scale counter

·         One-Shot mode

·         Periodic Mode

·         Toggle Mode

·         Continuous Counting Mode

 

## 3. Which timer mode that works best 

### – For a periodic interrupt of ½ s 

 

Continuous Counting Mode would work best for a periodic interrupt of 1/2 s.

 

## 4. How to calculate a ½ s “time out period” 

### – For flashing LED 



$Period_{time-out}=\frac{1}{f_{clock}}*(1+prescale_{8-bit})*TCMP_{24_bit}$

$TCMP_{24-bit}=Period_{time-out}f_{clock}/(1+prescale)$

In order to calculate a 1/2 s time-out period, we can use the equations above. Let's assume we are using a 12 MHz clock. Let's also choose a prescale value of 120. This means that $TCMP_{24-bit}=(0.5)(12e^{-6})/(1+120)=49587$. Therefore, we can get a 1/2 s time-out period by using 12 MHz clock, setting the prescale value to 120, and setting the $TMCP_{24-bit}$ value to 49587.

 

## 5. What drivers (from BSP) needed 

### – To support code in Q4

TIMER/WDT Driver in file `DrvTimer.c` is needed.