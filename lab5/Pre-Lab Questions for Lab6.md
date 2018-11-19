__Andrew Smit__

__ECE 5330__

__11/5/18__

# Pre-Lab Questions for Lab6

### 1.  According to the author of “PID without a PhD,” one of the three kinds of control is the most problematic. Which one is it and what makes it difficult to implement?    

The author says the differentiator is the most problematic of the three kinds of control. He says this is the most problematic because of its sensitivity to sampling irregularities, noise, and high frequency oscillations. 

### 2.  In the Technical Reference Manual, read the timer controller sections for the event counting function and the input capture function. (5.10.4.5 and 5.10.4.6)     

#### a. What is the clock frequency rule/guideline if you have disabled the debounce? 

The event count source operating frequency should be less than 1/3 HCLK frequency if counting de-bounce is disabled.

#### b. What is the clock frequency rule if you have enabled the debounce? 

The event count source operating frequency should be less than 1/8 HCLK frequency if counting de-bounce is enabled.

#### c. Why do you think the TDR (register) value would be incorrect if these guidelines are not followed? 

The TDR value would be incorrect if these guidelines are not followed because it will miss events if the event count source operating frequency is too fast. It also takes longer if the debounce is enabled, so the operating frequency has to be even less.

#### d. There are 4 Timers, each timer has a counter input pin and a capture input pin. Using the table 3.4.1.2 in the TRM (or the diagram 3.3.1.4) list the 8 pins used by the 4 timers for the external capture inputs and the event counter inputs. 

T2EX - (LQFP 100-pin 34, LQFP 64-pin 23, PB2)  Timer 2 external capture input pin

T3EX - (LQFP 100-pin 35, LQFT 64-pin 24, PB3) Timer 3 external capture input pin

T1EX - (LQFP 100-pin 47, PE5)  Timer 1 external capture input pin

T0Ex - (LQFP 100-pin 91, LQFT 64-pin 57, LQFP 48-pin 43, PB15) Timer 0 external capture input 

TM3 - (LQFP 100-pin 48, PB11) Timer 3 event counter input

TM2 - (LQFP 100-pin 49, PB10) Timer 2 event counter input

TM1 - (LQFP 100-pin 50, PB9) Timer 1 event counter input

TM0 - (LQFP 100-pin 100, PB8) Timer 0 event counter input

#### e. What other options are there for external interrupts? 

PB14 is an external interrupt1 input pin. PB15 is an external interrupt0 input pin.