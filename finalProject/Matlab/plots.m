clc, clear, close all;

Vs = 2.64
nums = 0:15;
vals = 0:1/15*Vs:Vs
my_vals = [0.03 0.21 0.38 0.56 0.71 0.89 1.06 1.34 1.43 1.62 1.79 1.97 2.13 2.30 2.47 2.64]
plot(nums,vals/Vs)
hold on
plot(nums,my_vals/Vs,'.','MarkerSize',15)
title('DAC Output Voltages')
xlabel('Digital Values')
ylabel('V_A/V_s')
legend('Expected DAC Voltages', 'Measured DAC Voltages','Location','best')
grid()