clc, clear, close all;
% In order to get equal spacing, we need R5 = 2R4 = 2R3 = 2R2
% So we just need to solve for R2

Vs = 2.64
R1 = 1000;
syms R2;
R3 = 2*R2;
R4 = 4*R2;
R5 = 8*R2;

% 1111
RA = R1;
RB = par(par(par(R2,R3),R4),R5);
VA16 = 1.0*Vs;
R2 = solve(RA*3.3/(RB+RA)-VA16,R2);
R2 = double(R2)
R3 = 2*R2
R4 = 2*R3
R5 = 2*R4

VA = zeros(1,16);

% 0000
VA(1) = par(par(par(par(R1,R2),R3),R4),R5)*0;

% 0001
RA = par(par(par(R1,R2),R3),R4);
RB = R5;
VA(2) = RA*3.3/(RB+RA);

% 0010
RA = par(par(par(R1,R2),R3),R5);
RB = R4;
VA(3) = RA*3.3/(RB+RA);

% 0011
RA = par(par(R1,R2),R3);
RB = par(R4,R5);
VA(4) = RA*3.3/(RB+RA);

% 0100
RA = par(par(par(R1,R2),R4),R5);
RB = R3;
VA(5) = RA*3.3/(RB+RA);

% 0101
RA = par(par(R1,R2),R4);
RB = par(R3,R5);
VA(6) = RA*3.3/(RB+RA);

% 0110
RA = par(par(R1,R2),R5);
RB = par(R3,R4);
VA(7) = RA*3.3/(RB+RA);

% 0111
RA = par(R1,R2);
RB = par(par(R3,R4),R5);
VA(8) = RA*3.3/(RB+RA);

% 1000
RA = par(par(par(R1,R3),R4),R5);
RB = R2;
VA(9) = RA*3.3/(RB+RA);

% 1001
RA = par(par(R1,R3),R4);
RB = par(R2,R5);
VA(10) = RA*3.3/(RB+RA);

% 1010
RA = par(par(R1,R3),R5);
RB = par(R2,R4);
VA(11) = RA*3.3/(RB+RA);

% 1011
RA = par(R1,R3);
RB = par(par(R2,R4),R5);
VA(12) = RA*3.3/(RB+RA);

% 1100
RA = par(par(R1,R4),R5);
RB = par(R2,R3);
VA(13) = RA*3.3/(RB+RA);

% 1101
RA = par(R1,R4);
RB = par(par(R2,R3),R5);
VA(14) = RA*3.3/(RB+RA);

% 1110
RA = par(R1,R5);
RB = par(par(R2,R3),R4);
VA(15) = RA*3.3/(RB+RA);

% 1111
RA = R1;
RB = par(par(par(R2,R3),R4),R5);
VA(16) = RA*3.3/(RB+RA);

VA
nums = 0:15;
plot(nums,VA/Vs)
grid()