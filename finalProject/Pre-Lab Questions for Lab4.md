__Andrew Smit__

__ECE 5330__

__10/15/18__

# Pre-Lab Questions for Lab4

### 1.  Based on the electrical characteristics of the NUC140 (see the TRM), given that VDD=4.5V what is the source current for (PA, PB, PC, PD, PE) when in quasibidirectional mode?  

The source current is a minimum of -300 $\mu$A, typically = -370 $\mu$A, and a max of -450 $\mu$A given that $V_{DD} = 4.5V$ and in quasibidirectional mode.

### What is the sink current (VDD=4.5V) for (PA, PB, PC, PD, PE) in quasi-bidirectional mode? 

The sink current is a minimum of 10 mA, typically 16 mA, and a max of 20 mA given that $V_{DD} = 4.5V$ and in quasibidirectional mode.

### 2. How much current can the ports source and sink in the push-pull mode when Vdd=4.5 V.   

The source is a minimum of -20 mA, typically -24mA, and max of -28mA. The sink is the same as the quasi-bidirectional mode which is a minimum of 10 mA, typically 16mA, and max of 20 mA.

### 3. Derive the values for R2 and R3 in Figure 1 as defined previously in the lab handout. If you do this via Matlab or Maple, please include the results and the code used.  

```matlab
% Find R2 and R3 values
R1 = 1000;
syms R2;
syms R3;
Vs = 2.64;
VDD = 3.3;

VA = 0.3*Vs;
R3 = solve(par(R1,R3)*VDD/(par(R1,R3)+R2)-VA,R3);

VA2 = 0.7*Vs;
R2 = solve(par(R1,R2)*3.3/(par(R1,R2)+R3)-VA2,R2);
R2 = double(R2)

syms R3;
R3 = solve(par(R1,R3)*VDD/(par(R1,R3)+R2)-VA,R3);
R3 = double(R3)
```

This Matlab code solves for $R_2$ and $R_3$. It gives values of $R_2=833.33\Omega$ and $R_3=357.143\Omega$. The following code checks these values to make sure the appropriate $V_A$ voltages. 

```matlab
R1 = 1000;
R2 = 833.3333;
R3 = 357.1429;

VDD = 3.3;
Vs = 2.64;

VA0 = par(par(R1,R2),R3)*0/Vs
VA1 = par(R1,R3)*VDD/(par(R1,R3)+R2)/Vs
VA2 = par(R1,R2)*VDD/(par(R1,R2)+R3)/Vs
VA3 = R1*VDD/(par(R2,R3)+R1)/Vs
```

The output window shows $V_{A0} = 0.0$, $V_{A1}=0.30$, $V_{A2}=0.70$, and $V_{A3}=1.0$

