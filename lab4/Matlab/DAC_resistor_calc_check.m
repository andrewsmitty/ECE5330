R1 = 1000;
R2 = 833.33;
R3 = 357.1429;

VDD = 3.3;
Vs = 2.64;

VA0 = par(par(R1,R2),R3)*0/Vs
VA1 = par(R1,R3)*VDD/(par(R1,R3)+R2)/Vs
VA2 = par(R1,R2)*VDD/(par(R1,R2)+R3)/Vs
VA3 = R1*VDD/(par(R2,R3)+R1)/Vs