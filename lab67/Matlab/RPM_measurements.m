% compare open-loop target values vs. measured values
clc, clear, close all;

target = 0:150000/8:150000;
measured = [0,10920,20580,47220,74880,89240,100200,124560,137700];

plot(target,measured);
figure(2)
plot(target-measured);
