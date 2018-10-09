__Andrew Smit__

__ECE 5330__

__9/17/18__

# Pre-Lab Questions for Lab2 

### 1. What is the range of an n-bit A/D converter? 

The range of an n-bit converter is $0$ to $2^n-1$.

### 2. What is the range of NUC140VE3CN converter. What is the resolution if the full-scale input range is 5V? 

The range of the NUC140VE3CN converter is $0$ to $2^{12}-1$. The resolution is $\frac{5}{2^{12}-1}(1000) = 1.22 mV$ 

### 3.Briefly describe 3 operating modes of the ADC in NUC140VE3CN. Briefly describe the input modes.  

Single mode: A/D conversion is performed one time on a specified channel 

Single-cycle scan mode: A/D conversion is performed one cycle on all specified channels with the sequence from the lowest numbered channel to the highest numbered channel 

Continuous scan mode: A/D converter continuously performs Single-cycle scan mode until software stops A/D conversion 

### 4. What are the possible inputs for Channel 7 of the ADC and how do you select them?  

External analog voltage, internal band-gap voltage, and internal temperature sensor output. You can select them with the PRESEL registers. 00 = External analog input, 01 = Internal band-gap voltage, and 10 = internal temperature sensor.

### 5. How are the results accessed, what additional information is provided?  

The results are accessed from the ADDR register associated with the ADC channel (ADDR0, ADDR2, ADDR3, for channels 0, 2, and 3). You are also provided a valid flag and an overrun flag in addition to the results.