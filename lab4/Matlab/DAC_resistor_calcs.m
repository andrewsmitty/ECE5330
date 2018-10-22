R1 = 1000;
syms R2;
syms R3;
Vs = 2.64;
VDD = 3.3;

VA = 0.3*Vs;
R2 = solve(par(R1,R3)*VDD/(par(R1,R3)+R2)-VA,R2)

VA2 = 0.7*Vs;
R3 = solve(par(R1,R2)*3.3/(par(R1,R2)+R3)-VA2,R3)
R3 = double(R3)

syms R3;
R2 = solve(par(R1,R3)*VDD/(par(R1,R3)+R2)-VA,R2);
R2 = double(R2)