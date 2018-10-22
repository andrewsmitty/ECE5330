__Andrew Smit__

__ECE 5330__

__10/22/18__

# Pre-Lab Questions for Lab5

### 1.  Describe the 3 basic components of a PID and give an example of how each is used   

A PID is a proportional-integral-derivative controller. The three parts are the proportional (gain) part, an integrator, and a differentiator. 

The proportional part is the error signal (error = desired - current) multiplied by a constant and fed out to the drive. The proportional part is used to scale the error in a system and control the sensitivity. An example of this could be a control system for a robotic arm. If we know the proportion between the current position of the arm and the desired position, then we can multiply the error proportionally and it will move the arm to the desired position.

The integral term is the sum of past errors, so adding the past errors will eventually drive the output closer to the desired output. A good example of this is a thermostat. If the temperature is below the desired temperature, then the integrator will accumulate the error between the two and start bringing up the temperature slowly. If the temperature is too high, then the integrator will accumulate error and bring the temperature down slowly until it reaches 0.

The differentiator is used is uses the rate of change of the error to predict future behavior of the system. The differentiator cannot bring the error to 0 by itself, but it can help the system bring the error to 0 much more quickly when used with the proportional and integrator components. An example of this could be in a cruise control for a car. We can set the desired speed of the car, and if the car slows down, the integrator will speed the car up to the desired speed. If the car is going too fast, then the integrator will bring the speed back down to the desired speed. The integrator is slow though, so if we want the system to respond more quickly, the differentiator can be used to speed up the car or slow it down much more quickly and get to the desired speed.

### 2.  From the PWM section of the TRM, list the steps of the PWM Timer Start procedure    

1. Setup clock source divider select register (CSR) 
2. Setup prescaler (PPR) 
3. Setup inverter on/off, dead zone generator on/off, auto-reload/one-shot mode and Stop PWM-timer (PCR) 
4. Setup comparator register (CMR) for setting PWM duty. 
5. Setup PWM down-counter register (CNR) for setting PWM period. 
6. Setup interrupt enable register (PIER) (option) 
7. Setup corresponding GPIO pins as PWM function (enable POE and disable CAPENR) for the corresponding PWM channel. 
8. Enable PWM timer start running (Set CHxEN = 1 in PCR) 

### 3. Here’s a C-programming question. Let’s say your code has defined a 16-bit signed integer, and reads a 12-bit value from the ADC. What is the maximum value that your code can multiply the ADC value by before storing the result in the 16-bit signed, integer variable, without losing any bits?   

The max value that your code can multiply the ADC value by before storing the result in the 16-bit signed, integer variable, without losing any bits is 8. This is because we lose 1 bit in the 16-bit value because of the sign. Therefore we essentially have 15 bits to store the value without the sign. 

$2^{12}G_{max}=2^{15}$. 

$G_{max}=2^{15-12}=2^3=8​$.

### Now, suppose that for whatever reason you need to multiply the ADC value by 74 but that you also need to divide it by a normalization factor of 165. Write the code, including the variable and constant (if any) definitions, to perform this calculation with minimal loss of accuracy in the final, signed, integer formatted answer.   

If we multiply the ADC value by 74 directly, then we will get overflow because this is greater than 8. However, if we divide by the normalization factor before multiplying, then we will not overflow the 16 bit signed integer register.

```C
uint12_t adc_val;
int16_t adc_store;
int16_t adc_norm;
int16_t adc_scale;
adc_store = adc_val;
adc_norm = adc_val/165;
adc_scale = adc_norm*74;
```

